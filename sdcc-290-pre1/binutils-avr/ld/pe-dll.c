/* Routines to help build PEI-format DLLs (Win32 etc)
   Copyright (C) 1998, 1999 Free Software Foundation, Inc.
   Written by DJ Delorie <dj@cygnus.com>

   This file is part of GLD, the Gnu Linker.

   GLD is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GLD is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GLD; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"
#include "libiberty.h"

#include <time.h>
#include <ctype.h>

#include "ld.h"
#include "ldexp.h"
#include "ldlang.h"
#include "ldwrite.h"
#include "ldmisc.h"
#include "ldgram.h"
#include "ldmain.h"
#include "ldemul.h"
#include "coff/internal.h"
#include "../bfd/libcoff.h"
#include "deffile.h"
#include "pe-dll.h"

/************************************************************************

 This file turns a regular Windows PE image into a DLL.  Because of
 the complexity of this operation, it has been broken down into a
 number of separate modules which are all called by the main function
 at the end of this file.  This function is not re-entrant and is
 normally only called once, so static variables are used to reduce
 the number of parameters and return values required.

 See also: ld/emultempl/pe.em

 ************************************************************************/

/* for emultempl/pe.em */

def_file *pe_def_file = 0;
int pe_dll_export_everything = 0;
int pe_dll_do_default_excludes = 1;
int pe_dll_kill_ats = 0;
int pe_dll_stdcall_aliases = 0;

/************************************************************************

 static variables and types

 ************************************************************************/

static bfd_vma image_base;

static bfd *filler_bfd;
static struct sec *edata_s, *reloc_s;
static unsigned char *edata_d, *reloc_d;
static size_t edata_sz, reloc_sz;

typedef struct {
  char *target_name;
  char *object_target;
  unsigned int imagebase_reloc;
  int pe_arch;
  int bfd_arch;
  int underscored;
} pe_details_type;

#define PE_ARCH_i386	1

static pe_details_type pe_detail_list[] = {
  {
    "pei-i386",
    "pe-i386",
    7 /* R_IMAGEBASE */,
    PE_ARCH_i386,
    bfd_arch_i386,
    1
  },
  { NULL, NULL, 0, 0, 0, 0 }
};

static pe_details_type *pe_details;

#define U(str) (pe_details->underscored ? "_" str : str)

void
pe_dll_id_target (target)
     const char *target;
{
  int i;
  for (i=0; pe_detail_list[i].target_name; i++)
    if (strcmp (pe_detail_list[i].target_name, target) == 0)
      {
	pe_details = pe_detail_list+i;
	return;
      }
  einfo (_("%XUnsupported PEI architecture: %s\n"), target);
  exit (1);
}

/************************************************************************

 Helper functions for qsort.  Relocs must be sorted so that we can write
 them out by pages.

 ************************************************************************/

typedef struct {
  bfd_vma vma;
  char type;
  short extra;
} reloc_data_type;

static int
reloc_sort (va, vb)
     const void *va, *vb;
{
  bfd_vma a = ((reloc_data_type *) va)->vma;
  bfd_vma b = ((reloc_data_type *) vb)->vma;
  return (a > b) ? 1 : ((a < b) ? -1 : 0);
}

static int
pe_export_sort (va, vb)
     const void *va, *vb;
{
  def_file_export *a = (def_file_export *) va;
  def_file_export *b = (def_file_export *) vb;
  return strcmp (a->name, b->name);
}

/************************************************************************

 Read and process the .DEF file

 ************************************************************************/

/* These correspond to the entries in pe_def_file->exports[].  I use
   exported_symbol_sections[i] to tag whether or not the symbol was
   defined, since we can't export symbols we don't have. */

static bfd_vma *exported_symbol_offsets;
static struct sec **exported_symbol_sections;

static int export_table_size;
static int count_exported;
static int count_exported_byname;
static int count_with_ordinals;
static const char *dll_name;
static int min_ordinal, max_ordinal;
static int *exported_symbols;

typedef struct exclude_list_struct
  {
    char *string;
    struct exclude_list_struct *next;
  }
exclude_list_struct;
static struct exclude_list_struct *excludes = 0;

void
pe_dll_add_excludes (new_excludes)
     const char *new_excludes;
{
  char *local_copy;
  char *exclude_string;

  local_copy = xstrdup (new_excludes);

  exclude_string = strtok (local_copy, ",:");
  for (; exclude_string; exclude_string = strtok (NULL, ",:"))
    {
      struct exclude_list_struct *new_exclude;

      new_exclude = ((struct exclude_list_struct *)
		     xmalloc (sizeof (struct exclude_list_struct)));
      new_exclude->string = (char *) xmalloc (strlen (exclude_string) + 1);
      strcpy (new_exclude->string, exclude_string);
      new_exclude->next = excludes;
      excludes = new_exclude;
    }

  free (local_copy);
}

static int
auto_export (d, n)
     def_file *d;
     const char *n;
{
  int i;
  struct exclude_list_struct *ex;
  for (i = 0; i < d->num_exports; i++)
    if (strcmp (d->exports[i].name, n) == 0)
      return 0;
  if (pe_dll_do_default_excludes)
    {
      if (strcmp (n, "DllMain@12") == 0)
	return 0;
      if (strcmp (n, "DllEntryPoint@0") == 0)
	return 0;
      if (strcmp (n, "impure_ptr") == 0)
	return 0;
    }
  for (ex = excludes; ex; ex = ex->next)
    if (strcmp (n, ex->string) == 0)
      return 0;
  return 1;
}

static void
process_def_file (abfd, info)
     bfd *abfd ATTRIBUTE_UNUSED;
     struct bfd_link_info *info;
{
  int i, j;
  struct bfd_link_hash_entry *blhe;
  bfd *b;
  struct sec *s;
  def_file_export *e=0;

  if (!pe_def_file)
    pe_def_file = def_file_empty ();

  /* First, run around to all the objects looking for the .drectve
     sections, and push those into the def file too */

  for (b = info->input_bfds; b; b = b->link_next)
    {
      s = bfd_get_section_by_name (b, ".drectve");
      if (s)
	{
	  int size = bfd_get_section_size_before_reloc (s);
	  char *buf = xmalloc (size);
	  bfd_get_section_contents (b, s, buf, 0, size);
	  def_file_add_directive (pe_def_file, buf, size);
	  free (buf);
	}
    }

  /* Now, maybe export everything else the default way */

  if (pe_dll_export_everything || pe_def_file->num_exports == 0)
    {
      for (b = info->input_bfds; b; b = b->link_next)
	{
	  asymbol **symbols;
	  int nsyms, symsize;

	  symsize = bfd_get_symtab_upper_bound (b);
	  symbols = (asymbol **) xmalloc (symsize);
	  nsyms = bfd_canonicalize_symtab (b, symbols);

	  for (j = 0; j < nsyms; j++)
	    {
	      if ((symbols[j]->flags & (BSF_FUNCTION | BSF_GLOBAL))
		  == (BSF_FUNCTION | BSF_GLOBAL))
		{
		  const char *sn = symbols[j]->name;
		  if (*sn == '_')
		    sn++;
		  if (auto_export (pe_def_file, sn))
		    def_file_add_export (pe_def_file, sn, 0, -1);
		}
	    }
	}
    }

#undef NE
#define NE pe_def_file->num_exports

  /* Canonicalize the export list */

  if (pe_dll_kill_ats)
    {
      for (i = 0; i < NE; i++)
	{
	  if (strchr (pe_def_file->exports[i].name, '@'))
	    {
	      /* This will preserve internal_name, which may have been pointing
	         to the same memory as name, or might not have */
	      char *tmp = xstrdup (pe_def_file->exports[i].name);
	      *(strchr (tmp, '@')) = 0;
	      pe_def_file->exports[i].name = tmp;
	    }
	}
    }

  if (pe_dll_stdcall_aliases)
    {
      for (i = 0; i < NE; i++)
	{
	  if (strchr (pe_def_file->exports[i].name, '@'))
	    {
	      char *tmp = xstrdup (pe_def_file->exports[i].name);
	      *(strchr (tmp, '@')) = 0;
	      if (auto_export (pe_def_file, tmp))
		def_file_add_export (pe_def_file, tmp,
				     pe_def_file->exports[i].internal_name, -1);
	      else
		free (tmp);
	    }
	}
    }

  e = pe_def_file->exports; /* convenience, but watch out for it changing */

  exported_symbol_offsets = (bfd_vma *) xmalloc (NE * sizeof (bfd_vma));
  exported_symbol_sections = (struct sec **) xmalloc (NE * sizeof (struct sec *));

  memset (exported_symbol_sections, 0, NE * sizeof (struct sec *));
  max_ordinal = 0;
  min_ordinal = 65536;
  count_exported = 0;
  count_exported_byname = 0;
  count_with_ordinals = 0;

  qsort (pe_def_file->exports, NE, sizeof (pe_def_file->exports[0]), pe_export_sort);
  for (i = 0, j = 0; i < NE; i++)
    {
      if (i > 0 && strcmp (e[i].name, e[i - 1].name) == 0)
	{
	  /* This is a duplicate */
	  if (e[j - 1].ordinal != -1
	      && e[i].ordinal != -1
	      && e[j - 1].ordinal != e[i].ordinal)
	    {
	      /* xgettext:c-format */
	      einfo (_("%XError, duplicate EXPORT with oridinals: %s (%d vs %d)\n"),
		     e[j - 1].name, e[j - 1].ordinal, e[i].ordinal);
	    }
	  else
	    {
	      /* xgettext:c-format */
	      einfo (_("Warning, duplicate EXPORT: %s\n"),
		     e[j - 1].name);
	    }
	  if (e[i].ordinal)
	    e[j - 1].ordinal = e[i].ordinal;
	  e[j - 1].flag_private |= e[i].flag_private;
	  e[j - 1].flag_constant |= e[i].flag_constant;
	  e[j - 1].flag_noname |= e[i].flag_noname;
	  e[j - 1].flag_data |= e[i].flag_data;
	}
      else
	{
	  if (i != j)
	    e[j] = e[i];
	  j++;
	}
    }
  pe_def_file->num_exports = j;	/* == NE */

  for (i = 0; i < NE; i++)
    {
      char *name = (char *) xmalloc (strlen (pe_def_file->exports[i].internal_name) + 2);
      if (pe_details->underscored)
	{
	  *name = '_';
	  strcpy (name + 1, pe_def_file->exports[i].internal_name);
	}
      else
	strcpy (name, pe_def_file->exports[i].internal_name);

      blhe = bfd_link_hash_lookup (info->hash,
				   name,
				   false, false, true);

      if (blhe && (blhe->type == bfd_link_hash_defined))
	{
	  count_exported++;
	  if (!pe_def_file->exports[i].flag_noname)
	    count_exported_byname++;
	  exported_symbol_offsets[i] = blhe->u.def.value;
	  exported_symbol_sections[i] = blhe->u.def.section;
	  if (pe_def_file->exports[i].ordinal != -1)
	    {
	      if (max_ordinal < pe_def_file->exports[i].ordinal)
		max_ordinal = pe_def_file->exports[i].ordinal;
	      if (min_ordinal > pe_def_file->exports[i].ordinal)
		min_ordinal = pe_def_file->exports[i].ordinal;
	      count_with_ordinals++;
	    }
	}
      else if (blhe && blhe->type == bfd_link_hash_undefined)
	{
	  /* xgettext:c-format */
	  einfo (_("%XCannot export %s: symbol not defined\n"),
		 pe_def_file->exports[i].internal_name);
	}
      else if (blhe)
	{
	  /* xgettext:c-format */
	  einfo (_("%XCannot export %s: symbol wrong type (%d vs %d)\n"),
		 pe_def_file->exports[i].internal_name,
		 blhe->type, bfd_link_hash_defined);
	}
      else
	{
	  /* xgettext:c-format */
	  einfo (_("%XCannot export %s: symbol not found\n"),
		 pe_def_file->exports[i].internal_name);
	}
      free (name);
    }
}

/************************************************************************

 Build the bfd that will contain .edata and .reloc sections

 ************************************************************************/

static void
build_filler_bfd (include_edata)
     int include_edata;
{
  lang_input_statement_type *filler_file;
  filler_file = lang_add_input_file ("dll stuff",
				     lang_input_file_is_fake_enum,
				     NULL);
  filler_file->the_bfd = filler_bfd = bfd_create ("dll stuff", output_bfd);
  if (filler_bfd == NULL
      || !bfd_set_arch_mach (filler_bfd,
			     bfd_get_arch (output_bfd),
			     bfd_get_mach (output_bfd)))
    {
      einfo ("%X%P: can not create BFD %E\n");
      return;
    }

  if (include_edata)
    {
      edata_s = bfd_make_section_old_way (filler_bfd, ".edata");
      if (edata_s == NULL
	  || !bfd_set_section_flags (filler_bfd, edata_s,
				     (SEC_HAS_CONTENTS
				      | SEC_ALLOC
				      | SEC_LOAD
				      | SEC_KEEP
				      | SEC_IN_MEMORY)))
	{
	  einfo ("%X%P: can not create .edata section: %E\n");
	  return;
	}
      bfd_set_section_size (filler_bfd, edata_s, edata_sz);
    }

  reloc_s = bfd_make_section_old_way (filler_bfd, ".reloc");
  if (reloc_s == NULL
      || !bfd_set_section_flags (filler_bfd, reloc_s,
				 (SEC_HAS_CONTENTS
				  | SEC_ALLOC
				  | SEC_LOAD
				  | SEC_KEEP
				  | SEC_IN_MEMORY)))
    {
      einfo ("%X%P: can not create .reloc section: %E\n");
      return;
    }
  bfd_set_section_size (filler_bfd, reloc_s, 0);

  ldlang_add_file (filler_file);
}

/************************************************************************

 Gather all the exported symbols and build the .edata section

 ************************************************************************/

static void
generate_edata (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
{
  int i, next_ordinal;
  int name_table_size = 0;
  const char *dlnp;

  /* First, we need to know how many exported symbols there are,
     and what the range of ordinals is. */

  if (pe_def_file->name)
    {
      dll_name = pe_def_file->name;
    }
  else
    {
      dll_name = abfd->filename;
      for (dlnp = dll_name; *dlnp; dlnp++)
	{
	  if (*dlnp == '\\' || *dlnp == '/' || *dlnp == ':')
	    dll_name = dlnp + 1;
	}
    }

  if (count_with_ordinals && max_ordinal > count_exported)
    {
      if (min_ordinal > max_ordinal - count_exported + 1)
	min_ordinal = max_ordinal - count_exported + 1;
    }
  else
    {
      min_ordinal = 1;
      max_ordinal = count_exported;
    }
  export_table_size = max_ordinal - min_ordinal + 1;

  exported_symbols = (int *) xmalloc (export_table_size * sizeof (int));
  for (i = 0; i < export_table_size; i++)
    exported_symbols[i] = -1;

  /* Now we need to assign ordinals to those that don't have them */
  for (i = 0; i < NE; i++)
    {
      if (exported_symbol_sections[i])
	{
	  if (pe_def_file->exports[i].ordinal != -1)
	    {
	      int ei = pe_def_file->exports[i].ordinal - min_ordinal;
	      int pi = exported_symbols[ei];
	      if (pi != -1)
		{
		  /* xgettext:c-format */
		  einfo (_("%XError, oridinal used twice: %d (%s vs %s)\n"),
			 pe_def_file->exports[i].ordinal,
			 pe_def_file->exports[i].name,
			 pe_def_file->exports[pi].name);
		}
	      exported_symbols[ei] = i;
	    }
	  name_table_size += strlen (pe_def_file->exports[i].name) + 1;
	}
    }

  next_ordinal = min_ordinal;
  for (i = 0; i < NE; i++)
    if (exported_symbol_sections[i])
      if (pe_def_file->exports[i].ordinal == -1)
	{
	  while (exported_symbols[next_ordinal - min_ordinal] != -1)
	    next_ordinal++;
	  exported_symbols[next_ordinal - min_ordinal] = i;
	  pe_def_file->exports[i].ordinal = next_ordinal;
	}

  /* OK, now we can allocate some memory */

  edata_sz = (40		/* directory */
	      + 4 * export_table_size	/* addresses */
	      + 4 * count_exported_byname	/* name ptrs */
	      + 2 * count_exported_byname	/* ordinals */
	      + name_table_size + strlen (dll_name) + 1);
}

static void
fill_edata (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
{
  int i, hint;
  unsigned char *edirectory;
  unsigned long *eaddresses;
  unsigned long *enameptrs;
  unsigned short *eordinals;
  unsigned char *enamestr;
  time_t now;

  time (&now);

  edata_d = (unsigned char *) xmalloc (edata_sz);

  /* Note use of array pointer math here */
  edirectory = edata_d;
  eaddresses = (unsigned long *) (edata_d + 40);
  enameptrs = eaddresses + export_table_size;
  eordinals = (unsigned short *) (enameptrs + count_exported_byname);
  enamestr = (char *) (eordinals + count_exported_byname);

#define ERVA(ptr) (((unsigned char *)(ptr) - edata_d) + edata_s->output_section->vma - image_base)

  memset (edata_d, 0, 40);
  bfd_put_32 (abfd, now, edata_d + 4);
  if (pe_def_file->version_major != -1)
    {
      bfd_put_16 (abfd, pe_def_file->version_major, edata_d + 8);
      bfd_put_16 (abfd, pe_def_file->version_minor, edata_d + 10);
    }
  bfd_put_32 (abfd, ERVA (enamestr), edata_d + 12);
  strcpy (enamestr, dll_name);
  enamestr += strlen (enamestr) + 1;
  bfd_put_32 (abfd, min_ordinal, edata_d + 16);
  bfd_put_32 (abfd, export_table_size, edata_d + 20);
  bfd_put_32 (abfd, count_exported_byname, edata_d + 24);
  bfd_put_32 (abfd, ERVA (eaddresses), edata_d + 28);
  bfd_put_32 (abfd, ERVA (enameptrs), edata_d + 32);
  bfd_put_32 (abfd, ERVA (eordinals), edata_d + 36);

  /* Ok, now for the filling in part */
  hint = 0;
  for (i = 0; i < export_table_size; i++)
    {
      int s = exported_symbols[i];
      if (s != -1)
	{
	  struct sec *ssec = exported_symbol_sections[s];
	  unsigned long srva = (exported_symbol_offsets[s]
				+ ssec->output_section->vma
				+ ssec->output_offset);

	  bfd_put_32 (abfd, srva - image_base, (void *) (eaddresses + i));
	  if (!pe_def_file->exports[s].flag_noname)
	    {
	      char *ename = pe_def_file->exports[s].name;
	      bfd_put_32 (abfd, ERVA (enamestr), (void *) enameptrs);
	      strcpy (enamestr, ename);
	      enamestr += strlen (enamestr) + 1;
	      bfd_put_16 (abfd, i, (void *) eordinals);
	      enameptrs++;
	      pe_def_file->exports[s].hint = hint++;
	    }
	  eordinals++;
	}
    }
}

/************************************************************************

 Gather all the relocations and build the .reloc section

 ************************************************************************/

static void
generate_reloc (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{

  /* for .reloc stuff */
  reloc_data_type *reloc_data;
  int total_relocs = 0;
  int i;
  unsigned long sec_page = (unsigned long) (-1);
  unsigned long page_ptr, page_count;
  int bi;
  bfd *b;
  struct sec *s;

  total_relocs = 0;
  for (b = info->input_bfds; b; b = b->link_next)
    for (s = b->sections; s; s = s->next)
      total_relocs += s->reloc_count;

  reloc_data = (reloc_data_type *) xmalloc (total_relocs * sizeof (reloc_data_type));

  total_relocs = 0;
  bi = 0;
  for (bi = 0, b = info->input_bfds; b; bi++, b = b->link_next)
    {
      arelent **relocs;
      int relsize, nrelocs, i;

      for (s = b->sections; s; s = s->next)
	{
	  unsigned long sec_vma = s->output_section->vma + s->output_offset;
	  asymbol **symbols;
	  int nsyms, symsize;

	  /* if it's not loaded, we don't need to relocate it this way */
	  if (!(s->output_section->flags & SEC_LOAD))
	    continue;

	  /* I don't know why there would be a reloc for these, but I've
	     seen it happen - DJ */
	  if (s->output_section == &bfd_abs_section)
	    continue;

	  if (s->output_section->vma == 0)
	    {
	      /* Huh?  Shouldn't happen, but punt if it does */
	      einfo ("DJ: zero vma section reloc detected: `%s' #%d f=%d\n",
		     s->output_section->name, s->output_section->index,
		     s->output_section->flags);
	      continue;
	    }

	  symsize = bfd_get_symtab_upper_bound (b);
	  symbols = (asymbol **) xmalloc (symsize);
	  nsyms = bfd_canonicalize_symtab (b, symbols);

	  relsize = bfd_get_reloc_upper_bound (b, s);
	  relocs = (arelent **) xmalloc ((size_t) relsize);
	  nrelocs = bfd_canonicalize_reloc (b, s, relocs, symbols);

	  for (i = 0; i < nrelocs; i++)
	    {
	      if (!relocs[i]->howto->pc_relative
		  && relocs[i]->howto->type != pe_details->imagebase_reloc)
		{
		  bfd_vma sym_vma;
		  struct symbol_cache_entry *sym = *relocs[i]->sym_ptr_ptr;
		  sym_vma = (relocs[i]->addend
			     + sym->value
			     + sym->section->vma
			     + sym->section->output_offset
			     + sym->section->output_section->vma);
		  reloc_data[total_relocs].vma = sec_vma + relocs[i]->address;
		  switch (relocs[i]->howto->bitsize*1000
			  + relocs[i]->howto->rightshift)
		    {
		    case 32000:
		      reloc_data[total_relocs].type = 3;
		      total_relocs++;
		      break;
		    default:
		      /* xgettext:c-format */
		      einfo (_("%XError: %d-bit reloc in dll\n"),
			     relocs[i]->howto->bitsize);
		      break;
		    }
		}
	    }
	  free (relocs);
	  /* Warning: the allocated symbols are remembered in BFD and reused
	     later, so don't free them! */
	  /* free (symbols); */
	}
    }

  /* At this point, we have total_relocs relocation addresses in
     reloc_addresses, which are all suitable for the .reloc section.
     We must now create the new sections. */

  qsort (reloc_data, total_relocs, sizeof (*reloc_data), reloc_sort);

  for (i = 0; i < total_relocs; i++)
    {
      unsigned long this_page = (reloc_data[i].vma >> 12);
      if (this_page != sec_page)
	{
	  reloc_sz = (reloc_sz + 3) & ~3;	/* 4-byte align */
	  reloc_sz += 8;
	  sec_page = this_page;
	}
      reloc_sz += 2;
    }
  reloc_sz = (reloc_sz + 3) & ~3;	/* 4-byte align */

  reloc_d = (unsigned char *) xmalloc (reloc_sz);

  sec_page = (unsigned long) (-1);
  reloc_sz = 0;
  page_ptr = (unsigned long) (-1);
  page_count = 0;
  for (i = 0; i < total_relocs; i++)
    {
      unsigned long rva = reloc_data[i].vma - image_base;
      unsigned long this_page = (rva & ~0xfff);
      if (this_page != sec_page)
	{
	  while (reloc_sz & 3)
	    reloc_d[reloc_sz++] = 0;
	  if (page_ptr != (unsigned long) (-1))
	    bfd_put_32 (abfd, reloc_sz - page_ptr, reloc_d + page_ptr + 4);
	  bfd_put_32 (abfd, this_page, reloc_d + reloc_sz);
	  page_ptr = reloc_sz;
	  reloc_sz += 8;
	  sec_page = this_page;
	  page_count = 0;
	}
      bfd_put_16 (abfd, (rva & 0xfff) + (reloc_data[i].type<<12),
		  reloc_d + reloc_sz);
      reloc_sz += 2;
      if (reloc_data[i].type == 4)
	{
	  bfd_put_16 (abfd, reloc_data[i].extra, reloc_d + reloc_sz);
	  reloc_sz += 2;
	}
      page_count++;
    }
  while (reloc_sz & 3)
    reloc_d[reloc_sz++] = 0;
  if (page_ptr != (unsigned long) (-1))
    bfd_put_32 (abfd, reloc_sz - page_ptr, reloc_d + page_ptr + 4);
  while (reloc_sz < reloc_s->_raw_size)
    reloc_d[reloc_sz++] = 0;
}

/************************************************************************

 Given the exiting def_file structure, print out a .DEF file that
 corresponds to it.

 ************************************************************************/

static void
quoteput (s, f, needs_quotes)
     char *s;
     FILE * f;
     int needs_quotes;
{
  char *cp;
  for (cp = s; *cp; cp++)
    if (*cp == '\''
	|| *cp == '"'
	|| *cp == '\\'
	|| isspace ((unsigned char) *cp)
	|| *cp == ','
	|| *cp == ';')
      needs_quotes = 1;
  if (needs_quotes)
    {
      putc ('"', f);
      while (*s)
	{
	  if (*s == '"' || *s == '\\')
	    putc ('\\', f);
	  putc (*s, f);
	  s++;
	}
      putc ('"', f);
    }
  else
    fputs (s, f);
}

void
pe_dll_generate_def_file (pe_out_def_filename)
     const char *pe_out_def_filename;
{
  int i;
  FILE *out = fopen (pe_out_def_filename, "w");
  if (out == NULL)
    {
      /* xgettext:c-format */
      einfo (_("%s: Can't open output def file %s\n"),
	     program_name, pe_out_def_filename);
    }

  if (pe_def_file)
    {
      if (pe_def_file->name)
	{
	  if (pe_def_file->is_dll)
	    fprintf (out, "LIBRARY ");
	  else
	    fprintf (out, "NAME ");
	  quoteput (pe_def_file->name, out, 1);
	  if (pe_data (output_bfd)->pe_opthdr.ImageBase)
	    fprintf (out, " BASE=0x%lx",
		     (unsigned long) pe_data (output_bfd)->pe_opthdr.ImageBase);
	  fprintf (out, "\n");
	}

      if (pe_def_file->description)
	{
	  fprintf (out, "DESCRIPTION ");
	  quoteput (pe_def_file->description, out, 1);
	  fprintf (out, "\n");
	}

      if (pe_def_file->version_minor != -1)
	fprintf (out, "VERSION %d.%d\n", pe_def_file->version_major,
		 pe_def_file->version_minor);
      else if (pe_def_file->version_major != -1)
	fprintf (out, "VERSION %d\n", pe_def_file->version_major);

      if (pe_def_file->stack_reserve != -1 || pe_def_file->heap_reserve != -1)
	fprintf (out, "\n");

      if (pe_def_file->stack_commit != -1)
	fprintf (out, "STACKSIZE 0x%x,0x%x\n",
		 pe_def_file->stack_reserve, pe_def_file->stack_commit);
      else if (pe_def_file->stack_reserve != -1)
	fprintf (out, "STACKSIZE 0x%x\n", pe_def_file->stack_reserve);
      if (pe_def_file->heap_commit != -1)
	fprintf (out, "HEAPSIZE 0x%x,0x%x\n",
		 pe_def_file->heap_reserve, pe_def_file->heap_commit);
      else if (pe_def_file->heap_reserve != -1)
	fprintf (out, "HEAPSIZE 0x%x\n", pe_def_file->heap_reserve);

      if (pe_def_file->num_section_defs > 0)
	{
	  fprintf (out, "\nSECTIONS\n\n");
	  for (i = 0; i < pe_def_file->num_section_defs; i++)
	    {
	      fprintf (out, "    ");
	      quoteput (pe_def_file->section_defs[i].name, out, 0);
	      if (pe_def_file->section_defs[i].class)
		{
		  fprintf (out, " CLASS ");
		  quoteput (pe_def_file->section_defs[i].class, out, 0);
		}
	      if (pe_def_file->section_defs[i].flag_read)
		fprintf (out, " READ");
	      if (pe_def_file->section_defs[i].flag_write)
		fprintf (out, " WRITE");
	      if (pe_def_file->section_defs[i].flag_execute)
		fprintf (out, " EXECUTE");
	      if (pe_def_file->section_defs[i].flag_shared)
		fprintf (out, " SHARED");
	      fprintf (out, "\n");
	    }
	}

      if (pe_def_file->num_exports > 0)
	{
	  fprintf (out, "\nEXPORTS\n\n");
	  for (i = 0; i < pe_def_file->num_exports; i++)
	    {
	      def_file_export *e = pe_def_file->exports + i;
	      fprintf (out, "    ");
	      quoteput (e->name, out, 0);
	      if (e->internal_name && strcmp (e->internal_name, e->name))
		{
		  fprintf (out, " = ");
		  quoteput (e->internal_name, out, 0);
		}
	      if (e->ordinal != -1)
		fprintf (out, " @%d", e->ordinal);
	      if (e->flag_private)
		fprintf (out, " PRIVATE");
	      if (e->flag_constant)
		fprintf (out, " CONSTANT");
	      if (e->flag_noname)
		fprintf (out, " NONAME");
	      if (e->flag_data)
		fprintf (out, " DATA");

	      fprintf (out, "\n");
	    }
	}

      if (pe_def_file->num_imports > 0)
	{
	  fprintf (out, "\nIMPORTS\n\n");
	  for (i = 0; i < pe_def_file->num_imports; i++)
	    {
	      def_file_import *im = pe_def_file->imports + i;
	      fprintf (out, "    ");
	      if (im->internal_name
		  && (!im->name || strcmp (im->internal_name, im->name)))
		{
		  quoteput (im->internal_name, out, 0);
		  fprintf (out, " = ");
		}
	      quoteput (im->module->name, out, 0);
	      fprintf (out, ".");
	      if (im->name)
		quoteput (im->name, out, 0);
	      else
		fprintf (out, "%d", im->ordinal);
	      fprintf (out, "\n");
	    }
	}
    }
  else
    fprintf (out, _("; no contents available\n"));

  if (fclose (out) == EOF)
    {
      /* xgettext:c-format */
      einfo (_("%P: Error closing file `%s'\n"), pe_out_def_filename);
    }
}

/************************************************************************

 Generate the import library

 ************************************************************************/

static asymbol **symtab;
static int symptr;
static int tmp_seq;
static const char *dll_filename;
static char *dll_symname;

#define UNDSEC (asection *) &bfd_und_section

static asection *
quick_section(abfd, name, flags, align)
     bfd *abfd;
     const char *name;
     int flags;
     int align;
{
  asection *sec;
  asymbol *sym;

  sec = bfd_make_section_old_way (abfd, name);
  bfd_set_section_flags (abfd, sec, flags
				  | SEC_ALLOC
				  | SEC_LOAD
				  | SEC_KEEP
			 );
  bfd_set_section_alignment (abfd, sec, align);
  /* remember to undo this before trying to link internally! */
  sec->output_section = sec;

  sym = bfd_make_empty_symbol (abfd);
  symtab[symptr++] = sym;
  sym->name = sec->name;
  sym->section = sec;
  sym->flags = BSF_LOCAL;
  sym->value = 0;

  return sec;
}

static void
quick_symbol (abfd, n1, n2, n3, sec, flags, addr)
     bfd *abfd;
     char *n1;
     char *n2;
     char *n3;
     asection *sec;
     int flags;
     int addr;
{
  asymbol *sym;
  char *name = (char *) xmalloc (strlen (n1) + strlen (n2) + strlen (n3) + 1);
  strcpy (name, n1);
  strcat (name, n2);
  strcat (name, n3);
  sym = bfd_make_empty_symbol (abfd);
  sym->name = name;
  sym->section = sec;
  sym->flags = flags;
  sym->value = addr;
  symtab[symptr++] = sym;
}

static arelent *reltab = 0;
static int relcount = 0, relsize = 0;

static void
quick_reloc (abfd, address, which_howto, symidx)
     bfd *abfd;
     int address;
     int which_howto;
     int symidx;
{
  if (relcount >= (relsize-1))
    {
      relsize += 10;
      if (reltab)
	reltab = (arelent *) xrealloc (reltab, relsize * sizeof (arelent));
      else
	reltab = (arelent *) xmalloc (relsize * sizeof (arelent));
    }
  reltab[relcount].address = address;
  reltab[relcount].addend = 0;
  reltab[relcount].howto = bfd_reloc_type_lookup (abfd, which_howto);
  reltab[relcount].sym_ptr_ptr = symtab + symidx;
  relcount++;
}

static void
save_relocs (asection *sec)
{
  int i;
  sec->relocation = reltab;
  sec->reloc_count = relcount;
  sec->orelocation = (arelent **) xmalloc ((relcount+1) * sizeof (arelent *));
  for (i=0; i<relcount; i++)
    sec->orelocation[i] = sec->relocation + i;
  sec->orelocation[relcount] = 0;
  sec->flags |= SEC_RELOC;
  reltab = 0;
  relcount = relsize = 0;
}

/*
 *	.section	.idata$2
 *	.global		__head_my_dll
 * __head_my_dll:
 *	.rva		hname
 *	.long		0
 *	.long		0
 *	.rva		__my_dll_iname
 *	.rva		fthunk
 *
 *	.section	.idata$5
 *	.long		0
 * fthunk:
 *
 *	.section	.idata$4
 *	.long		0
 * hname:
 */

static bfd *
make_head (parent)
     bfd *parent;
{
  asection *id2, *id5, *id4;
  unsigned char *d2, *d5, *d4;
  char *oname;
  bfd *abfd;

  oname = (char *) xmalloc (20);
  sprintf (oname, "d%06d.o", tmp_seq);
  tmp_seq++;

  abfd = bfd_create (oname, parent);
  bfd_find_target (pe_details->object_target, abfd);
  bfd_make_writable (abfd);

  bfd_set_format (abfd, bfd_object);
  bfd_set_arch_mach (abfd, pe_details->bfd_arch, 0);

  symptr = 0;
  symtab = (asymbol **) xmalloc (6 * sizeof (asymbol *));
  id2 = quick_section (abfd, ".idata$2", SEC_HAS_CONTENTS, 2);
  id5 = quick_section (abfd, ".idata$5", SEC_HAS_CONTENTS, 2);
  id4 = quick_section (abfd, ".idata$4", SEC_HAS_CONTENTS, 2);
  quick_symbol (abfd, U("_head_"), dll_symname, "", id2, BSF_GLOBAL, 0);
  quick_symbol (abfd, U(""), dll_symname, "_iname", UNDSEC, BSF_GLOBAL, 0);

  /* OK, pay attention here.  I got confused myself looking back at
     it.  We create a four-byte section to mark the beginning of the
     list, and we include an offset of 4 in the section, so that the
     pointer to the list points to the *end* of this section, which is
     the start of the list of sections from other objects. */

  bfd_set_section_size (abfd, id2, 20);
  d2 = (unsigned char *) xmalloc (20);
  id2->contents = d2;
  memset (d2, 0, 20);
  d2[0] = d2[16] = 4; /* reloc addend */
  quick_reloc (abfd,  0, BFD_RELOC_RVA, 2);
  quick_reloc (abfd, 12, BFD_RELOC_RVA, 4);
  quick_reloc (abfd, 16, BFD_RELOC_RVA, 1);
  save_relocs (id2);

  bfd_set_section_size (abfd, id5, 4);
  d5 = (unsigned char *) xmalloc (4);
  id5->contents = d5;
  memset (d5, 0, 4);

  bfd_set_section_size (abfd, id4, 4);
  d4 = (unsigned char *) xmalloc (4);
  id4->contents = d4;
  memset (d4, 0, 4);

  bfd_set_symtab (abfd, symtab, symptr);

  bfd_set_section_contents (abfd, id2, d2, 0, 20);
  bfd_set_section_contents (abfd, id5, d5, 0, 4);
  bfd_set_section_contents (abfd, id4, d4, 0, 4);
  
  bfd_make_readable (abfd);
  return abfd;
}

/*
 *	.section	.idata$4
 *	.long		0
 *	.section	.idata$5
 *	.long		0
 *	.section	idata$7
 *	.global		__my_dll_iname
 *__my_dll_iname:
 *	.asciz		"my.dll"
 */

static bfd *
make_tail (parent)
     bfd *parent;
{
  asection *id4, *id5, *id7;
  unsigned char *d4, *d5, *d7;
  int len;
  char *oname;
  bfd *abfd;

  oname = (char *) xmalloc (20);
  sprintf (oname, "d%06d.o", tmp_seq);
  tmp_seq++;

  abfd = bfd_create (oname, parent);
  bfd_find_target (pe_details->object_target, abfd);
  bfd_make_writable (abfd);

  bfd_set_format (abfd, bfd_object);
  bfd_set_arch_mach (abfd, pe_details->bfd_arch, 0);

  symptr = 0;
  symtab = (asymbol **) xmalloc (5 * sizeof (asymbol *));
  id4 = quick_section (abfd, ".idata$4", SEC_HAS_CONTENTS, 2);
  id5 = quick_section (abfd, ".idata$5", SEC_HAS_CONTENTS, 2);
  id7 = quick_section (abfd, ".idata$7", SEC_HAS_CONTENTS, 2);
  quick_symbol (abfd, U(""), dll_symname, "_iname", id7, BSF_GLOBAL, 0);

  bfd_set_section_size (abfd, id4, 4);
  d4 = (unsigned char *) xmalloc (4);
  id4->contents = d4;
  memset (d4, 0, 4);

  bfd_set_section_size (abfd, id5, 4);
  d5 = (unsigned char *) xmalloc (4);
  id5->contents = d5;
  memset (d5, 0, 4);

  len = strlen (dll_filename)+1;
  if (len & 1)
    len ++;
  bfd_set_section_size (abfd, id7, len);
  d7 = (unsigned char *) xmalloc (len);
  id7->contents = d7;
  strcpy (d7, dll_filename);

  bfd_set_symtab (abfd, symtab, symptr);

  bfd_set_section_contents (abfd, id4, d4, 0, 4);
  bfd_set_section_contents (abfd, id5, d5, 0, 4);
  bfd_set_section_contents (abfd, id7, d7, 0, len);

  bfd_make_readable (abfd);
  return abfd;
}

/*
 *	.text
 *	.global		_function
 *	.global		___imp_function
 *	.global		__imp__function
 *_function:
 *	jmp		*__imp__function:
 *
 *	.section	idata$7
 *	.long		__head_my_dll
 *
 *	.section	.idata$5
 *___imp_function:
 *__imp__function:
 *iat?
 *	.section	.idata$4
 *iat?
 *	.section	.idata$6
 *ID<ordinal>:
 *	.short		<hint>
 *	.asciz		"function" xlate? (add underscore, kill at)
 */

static unsigned char jmp_ix86_bytes[] = {
  0xff, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90
};


static bfd *
make_one (exp, parent)
     def_file_export *exp;
     bfd *parent;
{
  asection *tx, *id7, *id5, *id4, *id6;
  unsigned char *td, *d7, *d5, *d4, *d6;
  int len;
  char *oname;
  bfd *abfd;
  unsigned char *jmp_bytes;
  int jmp_byte_count;

  switch (pe_details->pe_arch)
    {
    case PE_ARCH_i386:
      jmp_bytes = jmp_ix86_bytes;
      jmp_byte_count = sizeof (jmp_ix86_bytes);
      break;
    }

  oname = (char *) xmalloc (20);
  sprintf (oname, "d%06d.o", tmp_seq);
  tmp_seq++;

  abfd = bfd_create (oname, parent);
  bfd_find_target (pe_details->object_target, abfd);
  bfd_make_writable (abfd);

  bfd_set_format (abfd, bfd_object);
  bfd_set_arch_mach (abfd, pe_details->bfd_arch, 0);

  symptr = 0;
  symtab = (asymbol **) xmalloc (10 * sizeof (asymbol *));
  tx  = quick_section (abfd, ".text",    SEC_CODE|SEC_HAS_CONTENTS, 2);
  id7 = quick_section (abfd, ".idata$7", SEC_HAS_CONTENTS, 2);
  id5 = quick_section (abfd, ".idata$5", SEC_HAS_CONTENTS, 2);
  id4 = quick_section (abfd, ".idata$4", SEC_HAS_CONTENTS, 2);
  id6 = quick_section (abfd, ".idata$6", SEC_HAS_CONTENTS, 2);
  quick_symbol (abfd, U(""), exp->internal_name, "", tx, BSF_GLOBAL, 0);
  quick_symbol (abfd, U("_head_"), dll_symname, "", UNDSEC, BSF_GLOBAL, 0);
  quick_symbol (abfd, U("__imp_"), exp->internal_name, "", id5, BSF_GLOBAL, 0);
  quick_symbol (abfd, U("_imp__"), exp->internal_name, "", id5, BSF_GLOBAL, 0);

  bfd_set_section_size (abfd, tx, jmp_byte_count);
  td = (unsigned char *) xmalloc (jmp_byte_count);
  tx->contents = td;
  memcpy (td, jmp_bytes, jmp_byte_count);
  switch (pe_details->pe_arch)
    {
    case PE_ARCH_i386:
      quick_reloc (abfd, 2, BFD_RELOC_32, 2);
      break;
    }
  save_relocs (tx);

  bfd_set_section_size (abfd, id7, 4);
  d7 = (unsigned char *) xmalloc (4);
  id7->contents = d7;
  memset (d7, 0, 4);
  quick_reloc (abfd, 0, BFD_RELOC_RVA, 6);
  save_relocs (id7);

  bfd_set_section_size (abfd, id5, 4);
  d5 = (unsigned char *) xmalloc (4);
  id5->contents = d5;
  memset (d5, 0, 4);
  if (exp->flag_noname)
    {
      d5[0] = exp->ordinal;
      d5[1] = exp->ordinal >> 8;
      d5[3] = 0x80;
    }
  else
    {
      quick_reloc (abfd, 0, BFD_RELOC_RVA, 4);
      save_relocs (id5);
    }

  bfd_set_section_size (abfd, id4, 4);
  d4 = (unsigned char *) xmalloc (4);
  id4->contents = d4;
  memset (d4, 0, 4);
  if (exp->flag_noname)
    {
      d5[0] = exp->ordinal;
      d5[1] = exp->ordinal >> 8;
      d5[3] = 0x80;
    }
  else
    {
      quick_reloc (abfd, 0, BFD_RELOC_RVA, 4);
      save_relocs (id4);
    }

  if (exp->flag_noname)
    {
      len = 0;
      bfd_set_section_size (abfd, id6, 0);
    }
  else
    {
      len = strlen (exp->name) + 3;
      if (len & 1)
	len++;
      bfd_set_section_size (abfd, id6, len);
      d6 = (unsigned char *) xmalloc (len);
      id6->contents = d6;
      memset (d6, 0, len);
      d6[0] = exp->hint & 0xff;
      d6[1] = exp->hint >> 8;
      strcpy (d6+2, exp->name);
    }

  bfd_set_symtab (abfd, symtab, symptr);

  bfd_set_section_contents (abfd, tx, td, 0, jmp_byte_count);
  bfd_set_section_contents (abfd, id7, d7, 0, 4);
  bfd_set_section_contents (abfd, id5, d5, 0, 4);
  bfd_set_section_contents (abfd, id4, d4, 0, 4);
  if (!exp->flag_noname)
    bfd_set_section_contents (abfd, id6, d6, 0, len);

  bfd_make_readable (abfd);
  return abfd;
}

void
pe_dll_generate_implib (def, impfilename)
     def_file *def;
     const char *impfilename;
{
  int i;
  bfd *ar_head;
  bfd *ar_tail;
  bfd *outarch;
  bfd *head = 0;

  dll_filename = def->name;
  if (dll_filename == 0)
    {
      dll_filename = dll_name;
      for (i=0; impfilename[i]; i++)
	if (impfilename[i] == '/' || impfilename[i] == '\\')
	  dll_filename = impfilename+1;
    }
  dll_symname = xstrdup (dll_filename);
  for (i=0; dll_symname[i]; i++)
    if (!isalnum ((unsigned char) dll_symname[i]))
      dll_symname[i] = '_';

  unlink (impfilename);

  outarch = bfd_openw (impfilename, 0);

  if (!outarch)
    {
      /* xgettext:c-format */
      einfo (_("%XCan't open .lib file: %s\n"), impfilename);
      return;
    }

  /* xgettext:c-format */
  einfo (_("Creating library file: %s\n"), impfilename);
  
  bfd_set_format (outarch, bfd_archive);
  outarch->has_armap = 1;

  /* Work out a reasonable size of things to put onto one line. */

  ar_head = make_head (outarch);

  for (i = 0; i<def->num_exports; i++)
    {
      /* The import library doesn't know about the internal name */
      char *internal = def->exports[i].internal_name;
      bfd *n;
      def->exports[i].internal_name = def->exports[i].name;
      n = make_one (def->exports+i, outarch);
      n->next = head;
      head = n;
      def->exports[i].internal_name = internal;
    }

  ar_tail = make_tail (outarch);

  if (ar_head == NULL || ar_tail == NULL)
    return;

  /* Now stick them all into the archive */

  ar_head->next = head;
  ar_tail->next = ar_head;
  head = ar_tail;

  if (! bfd_set_archive_head (outarch, head))
    einfo ("%Xbfd_set_archive_head: %s\n", bfd_errmsg (bfd_get_error ()));
  
  if (! bfd_close (outarch))
    einfo ("%Xbfd_close %s: %s\n", impfilename, bfd_errmsg (bfd_get_error ()));

  while (head != NULL)
    {
      bfd *n = head->next;
      bfd_close (head);
      head = n;
    }
}

static void
add_bfd_to_link (abfd, name, link_info)
     bfd *abfd;
     char *name;
     struct bfd_link_info *link_info;
{
  lang_input_statement_type *fake_file;
  fake_file = lang_add_input_file (name,
				   lang_input_file_is_fake_enum,
				   NULL);
  fake_file->the_bfd = abfd;
  ldlang_add_file (fake_file);
  if (!bfd_link_add_symbols (abfd, link_info))
    einfo ("%Xaddsym %s: %s\n", name, bfd_errmsg (bfd_get_error ()));
}

void
pe_process_import_defs (output_bfd, link_info)
     bfd *output_bfd;
     struct bfd_link_info *link_info;
{
  def_file_module *module;
  pe_dll_id_target(bfd_get_target (output_bfd));

  if (!pe_def_file)
    return;

  for (module = pe_def_file->modules; module; module = module->next)
    {
      int i, do_this_dll;

      dll_filename = module->name;
      dll_symname = xstrdup (module->name);
      for (i=0; dll_symname[i]; i++)
	if (!isalnum (dll_symname[i]))
	  dll_symname[i] = '_';

      do_this_dll = 0;

      for (i=0; i<pe_def_file->num_imports; i++)
	if (pe_def_file->imports[i].module == module)
	  {
	    def_file_export exp;
	    struct bfd_link_hash_entry *blhe;

	    /* see if we need this import */
	    char *name = (char *) xmalloc (strlen (pe_def_file->imports[i].internal_name) + 2);
	    sprintf (name, "%s%s", U(""), pe_def_file->imports[i].internal_name);
	    blhe = bfd_link_hash_lookup (link_info->hash, name,
					 false, false, false);
	    free (name);
	    if (blhe && blhe->type == bfd_link_hash_undefined)
	      {
		bfd *one;
		/* we do */
		if (!do_this_dll)
		  {
		    bfd *ar_head = make_head (output_bfd);
		    add_bfd_to_link (ar_head, ar_head->filename, link_info);
		    do_this_dll = 1;
		  }
		exp.internal_name = pe_def_file->imports[i].internal_name;
		exp.name = pe_def_file->imports[i].name;
		exp.ordinal = pe_def_file->imports[i].ordinal;
		exp.hint = exp.ordinal >= 0 ? exp.ordinal : 0;
		exp.flag_private = 0;
		exp.flag_constant = 0;
		exp.flag_data = 0;
		exp.flag_noname = exp.name ? 0 : 1;
		one = make_one (&exp, output_bfd);
		add_bfd_to_link (one, one->filename, link_info);
	      }
	  }
      if (do_this_dll)
	{
	  bfd *ar_tail = make_tail (output_bfd);
	  add_bfd_to_link (ar_tail, ar_tail->filename, link_info);
	}

      free (dll_symname);
    }
}

/************************************************************************

 We were handed a *.DLL file.  Parse it and turn it into a set of
 IMPORTS directives in the def file.  Return true if the file was
 handled, false if not.

 ************************************************************************/

static unsigned int
pe_get16 (abfd, where)
     bfd *abfd;
     int where;
{
  unsigned char b[2];
  bfd_seek (abfd, where, SEEK_SET);
  bfd_read (b, 1, 2, abfd);
  return b[0] + (b[1]<<8);
}

static unsigned int
pe_get32 (abfd, where)
     bfd *abfd;
     int where;
{
  unsigned char b[4];
  bfd_seek (abfd, where, SEEK_SET);
  bfd_read (b, 1, 4, abfd);
  return b[0] + (b[1]<<8) + (b[2]<<16) + (b[3]<<24);
}

#if 0 /* This is not currently used.  */

static unsigned int
pe_as16 (ptr)
     void *ptr;
{
  unsigned char *b = ptr;
  return b[0] + (b[1]<<8);
}

#endif

static unsigned int
pe_as32 (ptr)
     void *ptr;
{
  unsigned char *b = ptr;
  return b[0] + (b[1]<<8) + (b[2]<<16) + (b[3]<<24);
}

boolean
pe_implied_import_dll (filename)
     const char *filename;
{
  bfd *dll;
  unsigned long pe_header_offset, opthdr_ofs, num_entries, i;
  unsigned long export_rva, export_size, nsections, secptr, expptr;
  unsigned char *expdata, *erva;
  unsigned long name_rvas, ordinals, nexp, ordbase;
  const char *dll_name;

  /* No, I can't use bfd here.  kernel32.dll puts its export table in
     the middle of the .rdata section. */

  dll = bfd_openr (filename, pe_details->target_name);
  if (!dll)
    {
      einfo ("%Xopen %s: %s\n", filename, bfd_errmsg (bfd_get_error ()));
      return false;
    }
  /* PEI dlls seem to be bfd_objects */
  if (!bfd_check_format (dll, bfd_object))
    {
      einfo ("%X%s: this doesn't appear to be a DLL\n", filename);
      return false;
    }

  dll_name = filename;
  for (i=0; filename[i]; i++)
    if (filename[i] == '/' || filename[i] == '\\' || filename[i] == ':')
      dll_name = filename + i + 1;

  pe_header_offset = pe_get32 (dll, 0x3c);
  opthdr_ofs = pe_header_offset + 4 + 20;
  num_entries = pe_get32 (dll, opthdr_ofs + 92);
  if (num_entries < 1) /* no exports */
    return false;
  export_rva = pe_get32 (dll, opthdr_ofs + 96);
  export_size = pe_get32 (dll, opthdr_ofs + 100);
  nsections = pe_get16 (dll, pe_header_offset + 4 + 2);
  secptr = (pe_header_offset + 4 + 20 +
	    pe_get16 (dll, pe_header_offset + 4 + 16));
  expptr = 0;
  for (i=0; i<nsections; i++)
    {
      char sname[8];
      unsigned long secptr1 = secptr + 40 * i;
      unsigned long vaddr = pe_get32 (dll, secptr1 + 12);
      unsigned long vsize = pe_get32 (dll, secptr1 + 16);
      unsigned long fptr = pe_get32 (dll, secptr1 + 20);
      bfd_seek(dll, secptr1, SEEK_SET);
      bfd_read(sname, 1, 8, dll);
      if (vaddr <= export_rva && vaddr+vsize > export_rva)
	{
	  expptr = fptr + (export_rva - vaddr);
	  if (export_rva + export_size > vaddr + vsize)
	    export_size = vsize - (export_rva - vaddr);
	  break;
	}
    }

  expdata = (unsigned char *) xmalloc (export_size);
  bfd_seek (dll, expptr, SEEK_SET);
  bfd_read (expdata, 1, export_size, dll);
  erva = expdata - export_rva;

  if (pe_def_file == 0)
    pe_def_file = def_file_empty();

  nexp = pe_as32 (expdata+24);
  name_rvas = pe_as32 (expdata+32);
  ordinals = pe_as32 (expdata+36);
  ordbase = pe_as32 (expdata+16);
  for (i=0; i<nexp; i++)
    {
      unsigned long name_rva = pe_as32 (erva+name_rvas+i*4);
      def_file_import *imp;
      imp = def_file_add_import (pe_def_file, erva+name_rva, dll_name,
				 i, 0);
    }

  return true;
}

/************************************************************************

 These are the main functions, called from the emulation.  The first
 is called after the bfds are read, so we can guess at how much space
 we need.  The second is called after everything is placed, so we
 can put the right values in place.

 ************************************************************************/

void
pe_dll_build_sections (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  pe_dll_id_target (bfd_get_target (abfd));
  process_def_file (abfd, info);

  generate_edata (abfd, info);
  build_filler_bfd (1);
}

void
pe_exe_build_sections (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
{
  pe_dll_id_target (bfd_get_target (abfd));
  build_filler_bfd (0);
}

void
pe_dll_fill_sections (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  pe_dll_id_target (bfd_get_target (abfd));
  image_base = pe_data (abfd)->pe_opthdr.ImageBase;

  generate_reloc (abfd, info);
  if (reloc_sz > 0)
    {
      bfd_set_section_size (filler_bfd, reloc_s, reloc_sz);

      /* Resize the sections.  */
      lang_size_sections (stat_ptr->head, abs_output_section,
			  &stat_ptr->head, 0, (bfd_vma) 0, false);

      /* Redo special stuff.  */
      ldemul_after_allocation ();

      /* Do the assignments again.  */
      lang_do_assignments (stat_ptr->head,
			   abs_output_section,
			   (fill_type) 0, (bfd_vma) 0);
    }

  fill_edata (abfd, info);

  pe_data (abfd)->dll = 1;

  edata_s->contents = edata_d;
  reloc_s->contents = reloc_d;
}

void
pe_exe_fill_sections (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  pe_dll_id_target (bfd_get_target (abfd));
  image_base = pe_data (abfd)->pe_opthdr.ImageBase;

  generate_reloc (abfd, info);
  if (reloc_sz > 0)
    {
      bfd_set_section_size (filler_bfd, reloc_s, reloc_sz);

      /* Resize the sections.  */
      lang_size_sections (stat_ptr->head, abs_output_section,
			  &stat_ptr->head, 0, (bfd_vma) 0, false);

      /* Redo special stuff.  */
      ldemul_after_allocation ();

      /* Do the assignments again.  */
      lang_do_assignments (stat_ptr->head,
			   abs_output_section,
			   (fill_type) 0, (bfd_vma) 0);
    }
  reloc_s->contents = reloc_d;
}
