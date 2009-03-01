/* ELF object file format.
   Copyright (C) 1992, 93, 94, 95, 96, 97, 98, 1999
   Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */


/* HP PA-RISC support was contributed by the Center for Software Science
   at the University of Utah.  */

#ifndef _OBJ_ELF_H
#define _OBJ_ELF_H

#define OBJ_ELF 1

#ifndef OUTPUT_FLAVOR
#define OUTPUT_FLAVOR bfd_target_elf_flavour
#endif

#include <bfd.h>

#define BYTES_IN_WORD 4		/* for now */
#include "bfd/elf-bfd.h"

#include "targ-cpu.h"

#ifdef TC_ALPHA
#define ECOFF_DEBUGGING alpha_flag_mdebug
extern int alpha_flag_mdebug;
#endif

/* For now, always set ECOFF_DEBUGGING for a MIPS target.  */
#ifdef TC_MIPS
#ifdef MIPS_STABS_ELF
#define ECOFF_DEBUGGING 0
#else
#define ECOFF_DEBUGGING 1
#endif /* MIPS_STABS_ELF */
#endif /* TC_MIPS */

/* Additional information we keep for each symbol.  */
struct elf_obj_sy
{
  /* Whether the symbol has been marked as local.  */
  int local;

  /* Use this to keep track of .size expressions that involve
     differences that we can't compute yet.  */
  expressionS *size;

  /* The name specified by the .symver directive.  */
  char *versioned_name;

#ifdef ECOFF_DEBUGGING
  /* If we are generating ECOFF debugging information, we need some
     additional fields for each symbol.  */
  struct efdr *ecoff_file;
  struct localsym *ecoff_symbol;
  valueT ecoff_extern_size;
#endif
};

#define OBJ_SYMFIELD_TYPE struct elf_obj_sy

#ifndef FALSE
#define FALSE 0
#define TRUE  !FALSE
#endif

#define obj_begin() elf_begin ()
extern void elf_begin PARAMS ((void));

/* should be conditional on address size! */
#define elf_symbol(asymbol) ((elf_symbol_type *)(&(asymbol)->the_bfd))

#ifndef S_GET_SIZE
#define S_GET_SIZE(S) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_size)
#endif
#ifndef S_SET_SIZE
#define S_SET_SIZE(S,V) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_size = (V))
#endif

#ifndef S_GET_ALIGN
#define S_GET_ALIGN(S) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_value)
#endif
#ifndef S_SET_ALIGN
#define S_SET_ALIGN(S,V) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_value = (V))
#endif

#define S_GET_OTHER(S) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_other)
#define S_SET_OTHER(S,V) \
  (elf_symbol (symbol_get_bfdsym (S))->internal_elf_sym.st_other = (V))

extern asection *gdb_section;

#ifndef obj_frob_file
#define obj_frob_file  elf_frob_file
#endif
extern void elf_frob_file PARAMS ((void));

#ifndef obj_frob_file_after_relocs
#define obj_frob_file_after_relocs  elf_frob_file_after_relocs
#endif
extern void elf_frob_file_after_relocs PARAMS ((void));

#define obj_app_file elf_file_symbol
extern void elf_file_symbol PARAMS ((char *));

extern void obj_elf_ident PARAMS ((int));

extern void obj_elf_section_change_hook PARAMS ((void));

extern void obj_elf_section PARAMS ((int));
extern void obj_elf_previous PARAMS ((int));
extern void obj_elf_version PARAMS ((int));
extern void obj_elf_common PARAMS ((int));
extern void obj_elf_data PARAMS ((int));
extern void obj_elf_text PARAMS ((int));

/* BFD wants to write the udata field, which is a no-no for the
   globally defined sections.  */
#ifndef obj_sec_sym_ok_for_reloc
#define obj_sec_sym_ok_for_reloc(SEC)	((SEC)->owner != 0)
#endif

/* When setting one symbol equal to another, by default we probably
   want them to have the same "size", whatever it means in the current
   context.  */
#ifndef OBJ_COPY_SYMBOL_ATTRIBUTES
#define OBJ_COPY_SYMBOL_ATTRIBUTES(DEST,SRC)			\
do								\
  {								\
    struct elf_obj_sy *srcelf = symbol_get_obj (SRC);		\
    struct elf_obj_sy *destelf = symbol_get_obj (DEST);		\
    if (srcelf->size)						\
      {								\
	if (destelf->size == NULL)				\
	  destelf->size =					\
	    (expressionS *) xmalloc (sizeof (expressionS));	\
	*destelf->size = *srcelf->size;				\
      }								\
    else							\
      {								\
	if (destelf->size != NULL)				\
	  free (destelf->size);					\
	destelf->size = NULL;					\
      }								\
    S_SET_SIZE ((DEST), S_GET_SIZE (SRC));			\
    S_SET_OTHER ((DEST), S_GET_OTHER (SRC));			\
  }								\
while (0)
#endif

/* Stabs go in a separate section.  */
#define SEPARATE_STAB_SECTIONS 1

/* We need 12 bytes at the start of the section to hold some initial
   information.  */
extern void obj_elf_init_stab_section PARAMS ((segT));
#define INIT_STAB_SECTION(seg) obj_elf_init_stab_section (seg)

#ifdef ECOFF_DEBUGGING
/* We smuggle stabs in ECOFF rather than using a separate section.
   The Irix linker can not handle a separate stabs section.  */

#undef  SEPARATE_STAB_SECTIONS
#define SEPARATE_STAB_SECTIONS (!ECOFF_DEBUGGING)

#undef  INIT_STAB_SECTION
#define INIT_STAB_SECTION(seg) \
  ((void)(ECOFF_DEBUGGING ? 0 : (obj_elf_init_stab_section (seg), 0)))

#undef OBJ_PROCESS_STAB
#define OBJ_PROCESS_STAB(seg, what, string, type, other, desc)		\
  if (ECOFF_DEBUGGING)							\
    ecoff_stab ((seg), (what), (string), (type), (other), (desc))
#endif /* ECOFF_DEBUGGING */

extern void elf_frob_symbol PARAMS ((symbolS *, int *));
#ifndef obj_frob_symbol
#define obj_frob_symbol(symp, punt) elf_frob_symbol (symp, &punt)
#endif

extern void elf_pop_insert PARAMS ((void));
#ifndef obj_pop_insert
#define obj_pop_insert()	elf_pop_insert()
#endif

#ifndef OBJ_MAYBE_ELF
#define obj_ecoff_set_ext elf_ecoff_set_ext
#ifdef ANSI_PROTOTYPES
struct ecoff_extr;
#endif
extern void elf_ecoff_set_ext PARAMS ((symbolS *, struct ecoff_extr *));
#endif

#endif /* _OBJ_ELF_H */
