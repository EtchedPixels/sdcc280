/* tc-mcore.c -- Assemble code for M*Core
   Copyright (C) 1999 Free Software Foundation.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include <stdio.h>
#include "as.h"
#include "bfd.h"
#include "subsegs.h"
#define DEFINE_TABLE
#include "../opcodes/mcore-opc.h"
#include <ctype.h>
#include <string.h>

#ifdef OBJ_ELF
#include "elf/mcore.h"
#endif

#ifndef streq
#define streq(a,b) (strcmp (a, b) == 0)
#endif

/* Forward declarations for dumb compilers.  */
static void   mcore_s_literals PARAMS ((int));
static void   mcore_cons PARAMS ((int));
static void   mcore_float_cons PARAMS ((int));
static void   mcore_stringer PARAMS ((int));
static void   mcore_fill   PARAMS ((int));
static int    log2 PARAMS ((unsigned int));
static char * parse_reg PARAMS ((char *, unsigned *));
static char * parse_creg PARAMS ((char *, unsigned *));
static char * parse_exp PARAMS ((char *, expressionS *));
static void   make_name PARAMS ((char *, char *, int));
static int    enter_literal PARAMS ((expressionS *, int));
static char * parse_rt PARAMS ((char *, char **, int, expressionS *));
static char * parse_imm PARAMS ((char *, unsigned *, unsigned, unsigned));
static char * parse_mem PARAMS ((char *, unsigned *, unsigned *, unsigned));
static void   dump_literals PARAMS ((int));
static void   check_literals PARAMS ((int, int));
static void   mcore_s_text    PARAMS ((int));
static void   mcore_s_data    PARAMS ((int));
static void   mcore_s_section PARAMS ((int));
static void   mcore_s_bss     PARAMS ((int));
#ifdef OBJ_ELF
static void   mcore_s_comm    PARAMS ((int));
#endif


/* Several places in this file insert raw instructions into the
   object. They should use MCORE_INST_XXX macros to get the opcodes
   and then use these two macros to crack the MCORE_INST value into
   the appropriate byte values.  */
#define	INST_BYTE0(x)  (((x) >> 8) & 0xFF)
#define	INST_BYTE1(x)  ((x) & 0xFF)

const char comment_chars[] = "#/";
const char line_separator_chars[] = ";";
const char line_comment_chars[] = "#/";

const int md_reloc_size = 8;

static int do_jsri2bsr = 0;	/* change here from 1 by Cruess 19 August 97 */
static int sifilter_mode = 0;

const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant */
/* As in 0f12.456 */
/* or    0d1.2345e12 */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

#define C(what,length) (((what) << 2) + (length))
#define GET_WHAT(x)    ((x >> 2))

/* These are the two types of relaxable instruction */
#define COND_JUMP  1
#define UNCD_JUMP  2

#define UNDEF_DISP      0
#define COND12          1
#define COND32          2
#define UNCD12          1
#define UNCD32          2
#define UNDEF_WORD_DISP 4
#define END             5

#define C12_LEN	        2
#define C32_LEN	       10	/* allow for align */
#define U12_LEN	        2
#define U32_LEN	        8	/* allow for align */


/* Initialize the relax table */
const relax_typeS md_relax_table[] =
{
{    1,     1,	     0, 0 },			/* 0: unused */  
{    1,     1,	     0, 0 },			/* 1: unused */  
{    1,     1,	     0, 0 },			/* 2: unused */  
{    1,     1,	     0, 0 },			/* 3: unused */  
{    1,     1,	     0, 0 },			/* 4: unused */  
{ 2048, -2046, C12_LEN, C(COND_JUMP, COND32) },	/* 5: C(COND_JUMP, COND12) */
{    0,     0, C32_LEN, 0 },			/* 6: C(COND_JUMP, COND32) */
{    1,     1,	     0, 0 },			/* 7: unused */  
{    1,     1,	     0, 0 },			/* 8: unused */  
{ 2048, -2046, U12_LEN, C(UNCD_JUMP, UNCD32) },	/* 9: C(UNCD_JUMP, UNCD12) */
{    0,     0, U32_LEN, 0 },			/*10: C(UNCD_JUMP, UNCD32) */
{    1,     1,	     0, 0 },			/*11: unused */  
{    0,     0,	     0, 0 }			/*12: unused */  
};

/* LITERAL POOL DATA STRUCTURES */
struct literal
{
  unsigned short  refcnt;
  unsigned char	  ispcrel;
  unsigned char	  unused;
  expressionS	  e;
};

#define MAX_POOL_SIZE	(1024/4)
static struct literal litpool [MAX_POOL_SIZE];
static unsigned poolsize;
static unsigned poolnumber;
static unsigned long poolspan;

/* SPANPANIC: the point at which we get too scared and force a dump
   of the literal pool, and perhaps put a branch in place.
   Calculated as:
  		 1024	span of lrw/jmpi/jsri insn (actually span+1)
  		-2	possible alignment at the insn.
  		-2	possible alignment to get the table aligned.
  		-2	an inserted branch around the table.
  	     == 1018
   at 1018, we might be in trouble.
   -- so we have to be smaller than 1018 and since we deal with 2-byte
   instructions, the next good choice is 1016.
   -- Note we have a test case that fails when we've got 1018 here.  */
#define SPANPANIC	(1016)		/* 1024 - 1 entry - 2 byte rounding */
#define SPANCLOSE	(900)
#define SPANEXIT	(600)
static symbolS * poolsym;		/* label for current pool */
static char poolname[8];
static struct hash_control * opcode_hash_control;	/* Opcode mnemonics */

/* This table describes all the machine specific pseudo-ops the assembler
   has to support.  The fields are:
     Pseudo-op name without dot
     Function to call to execute this pseudo-op
     Integer arg to pass to the function   */
const pseudo_typeS md_pseudo_table[] =
{
  { "export",   s_globl,          0 },
  { "import",   s_ignore,         0 },
  { "literals", mcore_s_literals, 0 },
  { "page",     listing_eject,    0 },

  /* The following are to intercept the placement of data into the text
     section (eg addresses for a switch table), so that the space they
     occupy can be taken into account when deciding whether or not to
     dump the current literal pool.
     XXX - currently we do not cope with the .space and .dcb.d directives.  */
  { "ascii",    mcore_stringer,       0 },
  { "asciz",    mcore_stringer,       1 },
  { "byte",     mcore_cons,           1 },
  { "dc",       mcore_cons,           2 },
  { "dc.b",     mcore_cons,           1 },
  { "dc.d",     mcore_float_cons,    'd'},
  { "dc.l",     mcore_cons,           4 },
  { "dc.s",     mcore_float_cons,    'f'},
  { "dc.w",     mcore_cons,           2 },
  { "dc.x",     mcore_float_cons,    'x'},
  { "double",   mcore_float_cons,    'd'},
  { "float",    mcore_float_cons,    'f'},
  { "hword",    mcore_cons,           2 },
  { "int",      mcore_cons,           4 },
  { "long",     mcore_cons,           4 },
  { "octa",     mcore_cons,          16 },
  { "quad",     mcore_cons,           8 },
  { "short",    mcore_cons,           2 },
  { "single",   mcore_float_cons,    'f'},
  { "string",   mcore_stringer,       1 },
  { "word",     mcore_cons,           2 },
  { "fill",     mcore_fill,           0 },

  /* Allow for the effect of section changes.  */
  { "text",      mcore_s_text,    0 },
  { "data",      mcore_s_data,    0 },
  { "bss",       mcore_s_bss,     1 },
#ifdef OBJ_EF
  { "comm",      mcore_s_comm,    0 },
#endif
  { "section",   mcore_s_section, 0 },
  { "section.s", mcore_s_section, 0 },
  { "sect",      mcore_s_section, 0 },
  { "sect.s",    mcore_s_section, 0 },
  
  { 0,          0,                0 }
};

static void
mcore_s_literals (ignore)
     int ignore;
{
  dump_literals (0);
  demand_empty_rest_of_line ();
}


static void
mcore_cons (nbytes)
     int nbytes;
{
  if (now_seg == text_section)
    {
      char * ptr = input_line_pointer;
      int    commas = 1;
      
      /* Count the number of commas on the line.  */
      while (! is_end_of_line [* ptr])
	commas += * ptr ++ == ',';
      
      poolspan += nbytes * commas;
    }
  
  cons (nbytes);

  /* In theory we ought to call check_literals (2,0) here in case
     we need to dump the literal table.  We cannot do this however,
     as the directives that we are intercepting may be being used
     to build a switch table, and we must not interfere with its
     contents.  Instead we cross our fingers and pray... */
}

static void
mcore_float_cons (float_type)
     int float_type;
{
  if (now_seg == text_section)
    {
      char * ptr = input_line_pointer;
      int    commas = 1;

#ifdef REPEAT_CONS_EXPRESSIONS
#error REPEAT_CONS_EXPRESSIONS not handled
#endif
      
      /* Count the number of commas on the line.  */
      while (! is_end_of_line [* ptr])
	commas += * ptr ++ == ',';

      /* We would like to compute "hex_float (float_type) * commas"
	 but hex_float is not exported from read.c  */
      float_type == 'f' ? 4 : (float_type == 'd' ? 8 : 12);
      poolspan += float_type * commas;
    }

  float_cons (float_type);
  
  /* See the comment in mcore_cons () about calling check_literals.
     It is unlikely that a switch table will be constructed using
     floating point values, but it is still likely that an indexed
     table of floating point constants is being created by these
     directives, so again we must not interfere with their placement.  */
}

static void
mcore_stringer (append_zero)
     int append_zero;
{
  if (now_seg == text_section)
    {
      char * ptr = input_line_pointer;
      
      /* In theory we should compute how many bytes are going to
	 be occupied by the string(s) and add this to the poolspan.
	 To keep things simple however, we just add the number of
	 bytes left on the current line.  This will be an over-
	 estimate, which is OK, and automatically allows for the
	 appending a zero byte, since the real string(s) is/are
	 required to be enclosed in double quotes.  */
      while (! is_end_of_line [* ptr])
	ptr ++;

      poolspan += ptr - input_line_pointer;
    }
  
  stringer (append_zero);

  /* We call check_literals here in case a large number of strings are
     being placed into the text section with a sequence of stringer
     directives.  In theory we could be upsetting something if these
     strings are actually in an indexed table instead of referenced by
     individual labels.  Let us hope that that never happens.  */
  check_literals (2, 0);
}

static void
mcore_fill (unused)
     int unused;
{
  if (now_seg == text_section)
    {
      char * str = input_line_pointer;
      int    size = 1;
      int    repeat;

      repeat = atoi (str);
      
      /* Look to see if a size has been specified.  */
      while (*str != '\n' && *str != 0 && *str != ',')
	++ str;
      
      if (* str == ',')
	{
	  size = atoi (str + 1);

	  if (size > 8)
	    size = 8;
	  else if (size < 0)
	    size = 0;
	}

      poolspan += size * repeat;
    }
  
  s_fill (unused);

  check_literals (2, 0);
}

/* Handle the section changing pseudo-ops.  These call through to the
   normal implementations, but they dump the literal pool first.  */

static void
mcore_s_text (ignore)
     int ignore;
{
  dump_literals (0);
  
#ifdef OBJ_ELF
  obj_elf_text (ignore);
#else
  s_text (ignore);
#endif
}

static void
mcore_s_data (ignore)
     int ignore;
{
  dump_literals (0);
  
#ifdef OBJ_ELF
  obj_elf_data (ignore);
#else
  s_data (ignore);
#endif
}

static void
mcore_s_section (ignore)
     int ignore;
{
  /* Scan forwards to find the name of the section.  If the section
     being switched to is ".line" then this is a DWARF1 debug section
     which is arbitarily placed inside generated code.  In this case
     do not dump the literal pool because it is a) inefficient and
     b) would require the generation of extra code to jump around the
     pool.  */
  char * ilp = input_line_pointer;

  while (*ilp != 0 && isspace(*ilp))
    ++ ilp;

  if (strncmp (ilp, ".line", 5) == 0
      && (isspace (ilp[5]) || *ilp == '\n' || *ilp == '\r'))
    ;
  else
    dump_literals (0);

#ifdef OBJ_ELF
  obj_elf_section (ignore);
#endif
#ifdef OBJ_COFF
  obj_coff_section (ignore);
#endif
}

static void
mcore_s_bss (needs_align)
     int needs_align;
{
  dump_literals (0);
  
  s_lcomm_bytes (needs_align);
}

#ifdef OBJ_ELF
static void
mcore_s_comm (needs_align)
     int needs_align;
{
  dump_literals (0);
  
  obj_elf_common (needs_align);
}
#endif

/* This function is called once, at assembler startup time.  This should
   set up all the tables, etc that the MD part of the assembler needs.  */
void
md_begin ()
{
  mcore_opcode_info * opcode;
  char * prev_name = "";

  opcode_hash_control = hash_new ();

  /* Insert unique names into hash table */
  for (opcode = mcore_table; opcode->name; opcode ++)
    {
      if (streq (prev_name, opcode->name))
	{
	  /* Make all the opcodes with the same name point to the same
	     string.  */
	  opcode->name = prev_name;
	}
      else
	{
	  prev_name = opcode->name;
	  hash_insert (opcode_hash_control, opcode->name, (char *) opcode);
	}
    }
}

static int reg_m;
static int reg_n;
static expressionS immediate;	/* absolute expression */

/* Get a log2(val).  */
static int
log2 (val)
    unsigned int val;
{
    int log = -1;
    while (val != 0)
      {
        log ++;
        val >>= 1;
      }
    
    return log;
}

/* Try to parse a reg name.  */
static char *
parse_reg (s, reg)
     char * s;
     unsigned * reg;
{
  /* Strip leading whitespace.  */
  while (isspace (* s))
    ++ s;
  
  if (tolower (s[0]) == 'r')
    {
      if (s[1] == '1' && s[2] >= '0' && s[2] <= '5')
	{
	  *reg = 10 + s[2] - '0';
	  return s + 3;
	}
      
      if (s[1] >= '0' && s[1] <= '9')
	{
	  *reg = s[1] - '0';
	  return s + 2;
	}
    }
  else if (   tolower (s[0]) == 's'
	   && tolower (s[1]) == 'p'
	   && ! isalnum (s[2]))
    {
      * reg = 0;
      return s + 2;
    }
  
  as_bad (_("register expected, but saw '%.6s'"), s);
  return s;
}

static struct Cregs
{
  char * name;
  unsigned int crnum;
}
cregs[] =
{
  { "psr",	 0},
  { "vbr",	 1},
  { "epsr",	 2},
  { "fpsr",	 3},
  { "epc",	 4},
  { "fpc",	 5},
  { "ss0",	 6},
  { "ss1",	 7},
  { "ss2",	 8},
  { "ss3",	 9},
  { "ss4",	10},
  { "gcr",	11},
  { "gsr",	12},
  { "",		 0}
};

static char *
parse_creg (s, reg)
     char * s;
     unsigned * reg;
{
  int i;

  /* Strip leading whitespace.  */
  while (isspace (* s))
    ++s;
  
  if ((tolower (s[0]) == 'c' && tolower (s[1]) == 'r'))
    {
      if (s[2] == '3' && s[3] >= '0' && s[3] <= '1')
	{
	  *reg = 30 + s[3] - '0';
	  return s + 4;
	}
      
      if (s[2] == '2' && s[3] >= '0' && s[3] <= '9')
	{
	  *reg = 20 + s[3] - '0';
	  return s + 4;
	}
      
      if (s[2] == '1' && s[3] >= '0' && s[3] <= '9')
	{
	  *reg = 10 + s[3] - '0';
	  return s + 4;
	}
      
      if (s[2] >= '0' && s[2] <= '9')
	{
	  *reg = s[2] - '0';
	  return s + 3;
	}
    }
  
  /* Look at alternate creg names before giving error.  */
  for (i = 0; cregs[i].name[0] != '\0'; i++)
    {
      char buf [10];
      int  length;
      int  j;
      
      length = strlen (cregs[i].name);
      
      for (j = 0; j < length; j++)
	buf[j] = tolower (s[j]);
      
      if (strncmp (cregs[i].name, buf, length) == 0)
	{
	  *reg = cregs[i].crnum;
	  return s + length;
	}
    }
  
  as_bad (_("control register expected, but saw '%.6s'"), s);
  
  return s;
}

static char *
parse_exp (s, e)
     char * s;
     expressionS * e;
{
  char * save;
  char * new;

  /* Skip whitespace.  */
  while (isspace (* s))
    ++ s;
  
  save = input_line_pointer;
  input_line_pointer = s;

  expression (e);
  
  if (e->X_op == O_absent)
    as_bad (_("missing operand"));
  
  new = input_line_pointer;
  input_line_pointer = save;
  
  return new;
}

static void
make_name (s, p, n)
     char * s;
     char * p;
     int n;
{
  static const char hex[] = "0123456789ABCDEF";

  s[0] = p[0];
  s[1] = p[1];
  s[2] = p[2];
  s[3] = hex[(n >> 12) & 0xF];
  s[4] = hex[(n >>  8) & 0xF];
  s[5] = hex[(n >>  4) & 0xF];
  s[6] = hex[(n)       & 0xF];
  s[7] = 0;
}

#define POOL_END_LABEL   ".LE"
#define POOL_START_LABEL ".LS"

static void
dump_literals (isforce)
     int isforce;
{
  int i;
  struct literal * p;
  symbolS * brarsym;
  
  if (poolsize == 0)
    return;

  /* Must we branch around the literal table? */
  if (isforce)
    {
      char * output;
      char brarname[8];
      
      make_name (brarname, POOL_END_LABEL, poolnumber);
      
      brarsym = symbol_make (brarname);
      
      symbol_table_insert (brarsym);
      
      output = frag_var (rs_machine_dependent,
			 md_relax_table[C (UNCD_JUMP, UNCD32)].rlx_length,
			 md_relax_table[C (UNCD_JUMP, UNCD12)].rlx_length,
			 C (UNCD_JUMP, 0), brarsym, 0, 0);
      output[0] = INST_BYTE0 (MCORE_INST_BR);	/* br .+xxx */
      output[1] = INST_BYTE1 (MCORE_INST_BR);
    }
      
  /* Make sure that the section is sufficiently aligned and that
     the literal table is aligned within it.  */
  record_alignment (now_seg, 2);
  frag_align (2, 0, 0);
  
  colon (S_GET_NAME (poolsym));
  
  for (i = 0, p = litpool; i < poolsize; i++, p++)
    emit_expr (& p->e, 4);
  
  if (isforce)
    colon (S_GET_NAME (brarsym));
  
   poolsize = 0;
}

static void
check_literals (kind, offset)
     int kind;
     int offset;
{
  poolspan += offset;
    
  /* SPANCLOSE and SPANEXIT are smaller numbers than SPANPANIC.
     SPANPANIC means that we must dump now.
     kind == 0 is any old instruction.
     kind  > 0 means we just had a control transfer instruction.
     kind == 1 means within a function
     kind == 2 means we just left a function
    
     The dump_literals (1) call inserts a branch around the table, so
     we first look to see if its a situation where we won't have to
     insert a branch (e.g., the previous instruction was an unconditional
     branch).
    
     SPANPANIC is the point where we must dump a single-entry pool.
     it accounts for alignments and an inserted branch.
     the 'poolsize*2' accounts for the scenario where we do:
       lrw r1,lit1; lrw r2,lit2; lrw r3,lit3
     Note that the 'lit2' reference is 2 bytes further along
     but the literal it references will be 4 bytes further along,
     so we must consider the poolsize into this equation.
     This is slightly over-cautious, but guarantees that we won't
     panic because a relocation is too distant.  */
  
  if (poolspan > SPANCLOSE && kind > 0)
    dump_literals (0);
  else if (poolspan > SPANEXIT && kind > 1)
    dump_literals (0);
  else if (poolspan >= (SPANPANIC - poolsize * 2))
    dump_literals (1);
}

static int
enter_literal (e, ispcrel)
     expressionS * e;
     int ispcrel;
{
  int i;
  struct literal * p;

  if (poolsize >= MAX_POOL_SIZE - 2)
    {
      /* The literal pool is as full as we can handle. We have
         to be 2 entries shy of the 1024/4=256 entries because we
         have to allow for the branch (2 bytes) and the alignment
         (2 bytes before the first insn referencing the pool and
         2 bytes before the pool itself) == 6 bytes, rounds up
         to 2 entries.  */ 
      dump_literals (1);
    }

  if (poolsize == 0)
    {
      /* Create new literal pool.  */
      if (++ poolnumber > 0xFFFF)
	as_fatal (_("more than 65K literal pools"));
      
      make_name (poolname, POOL_START_LABEL, poolnumber);
      poolsym = symbol_make (poolname);
      symbol_table_insert (poolsym);
      poolspan = 0;
    }
  
  /* Search pool for value so we don't have duplicates.  */
  for (p = litpool, i = 0; i < poolsize; i++, p++)
    {
      if (e->X_op == p->e.X_op
	  && e->X_add_symbol == p->e.X_add_symbol
	  && e->X_add_number == p->e.X_add_number
	  && ispcrel == p->ispcrel)
	{
	  p->refcnt ++;
	  return i;
	}
    }

  p->refcnt  = 1;
  p->ispcrel = ispcrel;
  p->e       = * e;
  
  poolsize ++;

  return i;   
}

/* Parse a literal specification. -- either new or old syntax.
   old syntax: the user supplies the label and places the literal.
   new syntax: we put it into the literal pool.  */
static char *
parse_rt (s, outputp, ispcrel, ep)
     char * s;
     char ** outputp;
     int ispcrel;
     expressionS * ep;
{
  expressionS e;
  int n;
  
  if (ep)
    /* Indicate nothing there.  */
    ep->X_op = O_absent;
  
  if (*s == '[')
    {
      s = parse_exp (s + 1, & e);
      
      if (*s == ']')
	s++;
      else
	as_bad (_("missing ']'"));
    }
  else
    {
      s = parse_exp (s, & e);
      
      n = enter_literal (& e, ispcrel);
      
      if (ep)
	*ep = e;

      /* Create a reference to pool entry.  */
      e.X_op         = O_symbol;
      e.X_add_symbol = poolsym;
      e.X_add_number = n << 2;
    }
  
  * outputp = frag_more (2);

  fix_new_exp (frag_now, (*outputp) - frag_now->fr_literal, 2, & e, 1,
	       BFD_RELOC_MCORE_PCREL_IMM8BY4);

  return s;
}

static char *
parse_imm (s, val, min, max)
     char * s;
     unsigned * val;
     unsigned min;
     unsigned max;
{
  char * new;
  expressionS e;
  
  new = parse_exp (s, & e);
  
  if (e.X_op == O_absent)
    ; /* An error message has already been emitted.  */
  else if (e.X_op != O_constant)
    as_bad (_("operand must be a constant"));
  else if (e.X_add_number < min || e.X_add_number > max)
    as_bad (_("operand must be absolute in range %d..%d, not %d"),
	    min, max, e.X_add_number);

  * val = e.X_add_number;
  
  return new;
}

static char *
parse_mem (s, reg, off, siz)
     char * s;
     unsigned * reg;
     unsigned * off;
     unsigned siz;
{
  char * new;

  * off = 0;
  
  while (isspace (* s))
    ++ s;
      
  if (* s == '(')
    {
      s = parse_reg (s + 1, reg);

      while (isspace (* s))
	++ s;
      
      if (* s == ',')
	{
	  s = parse_imm (s + 1, off, 0, 63);
	  
	  if (siz > 1)
	    {
	      if (siz > 2)
		{
		  if (* off & 0x3)
		    as_bad (_("operand must be a multiple of 4"));
		  
		  * off >>= 2;
		}
	      else
		{
		  if (* off & 0x1)
		    as_bad (_("operand must be a multiple of 2"));
		  
		  * off >>= 1;
		}
	    }
	}
      
      while (isspace (* s))
	++ s;
      
      if (* s == ')')
	s ++;
    }
  else
    as_bad (_("base register expected"));
  
  return s;
}

/* This is the guts of the machine-dependent assembler.  STR points to a
   machine dependent instruction.  This function is supposed to emit
   the frags/bytes it assembles to.  */

void
md_assemble (str)
     char * str;
{
  char * op_start;
  char * op_end;
  mcore_opcode_info * opcode;
  char * output;
  int nlen = 0;
  unsigned short inst;
  unsigned reg;
  unsigned off;
  unsigned isize;
  expressionS e;
  char name[20];

  /* Drop leading whitespace.  */
  while (isspace (* str))
    str ++;

  /* Find the op code end.  */
  for (op_start = op_end = str;
       * op_end && nlen < 20 && !is_end_of_line [*op_end] && *op_end != ' ';
       op_end++)
    {
      name[nlen] = op_start[nlen];
      nlen++;
    }
  
  name [nlen] = 0;
  
  if (nlen == 0)
    {
      as_bad (_("can't find opcode "));
      return;
    }

  opcode = (mcore_opcode_info *) hash_find (opcode_hash_control, name);
  if (opcode == NULL)
    {
      as_bad (_("unknown opcode \"%s\""), name);
      return;
    }
  
  inst = opcode->inst;
  isize = 2;
  
  switch (opcode->opclass)
    {
    case O0:
      output = frag_more (2);
      break;
      
    case OT:
      op_end = parse_imm (op_end + 1, & reg, 0, 3);
      inst |= reg;
      output = frag_more (2);
      break;
      
    case O1:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      output = frag_more (2);
      break;
      
    case JMP:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      output = frag_more (2);
      /* In a sifilter mode, we emit this insn 2 times,
         fixes problem of an interrupt during a jmp.. */
      if (sifilter_mode)
	{
	  output[0] = INST_BYTE0 (inst);
	  output[1] = INST_BYTE1 (inst);
	  output = frag_more (2);
	}
      break;
      
    case JSR:
      op_end = parse_reg (op_end + 1, & reg);
      
      if (reg == 15)
	as_bad (_("invalid register: r15 illegal"));
      
      inst |= reg;
      output = frag_more (2);
      
      if (sifilter_mode)
	{
	  /* Replace with:  bsr .+2 ; addi r15,6; jmp rx ; jmp rx */
	  inst = MCORE_INST_BSR;	/* with 0 displacement */
	  output[0] = INST_BYTE0 (inst);
	  output[1] = INST_BYTE1 (inst);

	  output = frag_more (2);
	  inst = MCORE_INST_ADDI;
	  inst |= 15;			/* addi r15,6 */
	  inst |= (6 - 1) << 4;		/* over the jmp's */
	  output[0] = INST_BYTE0 (inst);
	  output[1] = INST_BYTE1 (inst);

	  output = frag_more (2);
	  inst = MCORE_INST_JMP | reg;
	  output[0] = INST_BYTE0 (inst);
	  output[1] = INST_BYTE1 (inst);

	  output = frag_more (2);		/* 2nd emitted in fallthru */
	}
      break;
      
    case OC:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (*op_end == ',')
	{
	  op_end = parse_creg (op_end + 1, & reg);
	  inst |= reg << 4;
	}
      
      output = frag_more (2);
      break;

    case O2:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_reg (op_end + 1, & reg);
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case X1:	/* Handle both syntax-> xtrb- r1,rx OR xtrb- rx */
      op_end = parse_reg (op_end + 1, & reg);
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')	/* xtrb- r1,rx */
	{
	  if (reg != 1)
	    as_bad (_("destination register must be r1"));
	  
	  op_end = parse_reg (op_end + 1, & reg);
	}
      
      inst |= reg;
      output = frag_more (2);
      break;
      
    case O1R1:  /* div- rx,r1 */
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_reg (op_end + 1, & reg);
	  if (reg != 1)
	    as_bad (_("source register must be r1"));
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OI:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 32);
	  inst |= (reg - 1) << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OB:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 0, 31);
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OB2:		/* like OB, but arg is 2^n instead of n */
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 1 << 31);
	  /* Further restrict the immediate to a power of two. */
	  if ((reg & (reg - 1)) == 0)
	    reg = log2 (reg);
	  else
	    {
	      reg = 0;
	      as_bad (_("immediate is not a power of two"));
	    }
	  inst |= (reg) << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OBRa:	/* Specific for bgeni: imm of 0->6 translate to movi. */
    case OBRb:
    case OBRc:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 0, 31);
	  /* immediate values of 0 -> 6 translate to movi */
	  if (reg <= 6)
	    {
	      inst = (inst & 0xF) | MCORE_INST_BGENI_ALT;
	      reg = 0x1 << reg;
	      as_warn (_("translating bgeni to movi"));
	    }
	  inst &= ~ 0x01f0;
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OBR2:	/* like OBR, but arg is 2^n instead of n */
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 1 << 31);
	  
	  /* Further restrict the immediate to a power of two.  */
	  if ((reg & (reg - 1)) == 0)
	    reg = log2 (reg);
	  else
	    {
	      reg = 0;
	      as_bad (_("immediate is not a power of two"));
	    }
	  
	  /* Immediate values of 0 -> 6 translate to movi. */
	  if (reg <= 6)
	    {
	      inst = (inst & 0xF) | MCORE_INST_BGENI_ALT;
	      reg = 0x1 << reg;
	      as_warn (_("translating mgeni to movi"));
	    }
	  
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case OMa:	/* Specific for bmaski: imm 1->7 translate to movi.  */
    case OMb:
    case OMc:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 32);
	  
	  /* Immediate values of 1 -> 7 translate to movi. */
	  if (reg <= 7)
	    {
	      inst = (inst & 0xF) | MCORE_INST_BMASKI_ALT;
	      reg = (0x1 << reg) - 1;
	      inst |= reg << 4;
	      
	      as_warn (_("translating bmaski to movi"));
	    }
	  else
	    {
	      inst &= ~ 0x01F0;
	      inst |= (reg & 0x1F) << 4;
	    }
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case SI:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 31);
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;

    case I7:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 0, 0x7F);
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case LS:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg << 8;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  int size;
	  
	  if ((inst & 0x6000) == 0)
	    size = 4;
	  else if ((inst & 0x6000) == 0x4000)
	    size = 2;
	  else if ((inst & 0x6000) == 0x2000)
	    size = 1;
	  
	  op_end = parse_mem (op_end + 1, & reg, & off, size);
	  
	  if (off > 16)
	    as_bad (_("displacement too large (%d)"), off);
	  else
	    inst |= (reg) | (off << 4);
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case LR:
      op_end = parse_reg (op_end + 1, & reg);
      
      if (reg == 0 || reg == 15)
	as_bad (_("Invalid register: r0 and r15 illegal"));
      
      inst |= (reg << 8);
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	/* parse_rt calls frag_more() for us.  */
	input_line_pointer = parse_rt (op_end + 1, & output, 0, 0);
      else
	{
	  as_bad (_("second operand missing"));
	  output = frag_more (2);		/* save its space */
	}
      break;
      
    case LJ:
      input_line_pointer = parse_rt (op_end + 1, & output, 1, 0);
      /* parse_rt() calls frag_more() for us.  */
      break;
      
    case RM:
      op_end = parse_reg (op_end + 1, & reg);
      
      if (reg == 0 || reg == 15)
	as_bad (_("bad starting register: r0 and r15 invalid"));
      
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == '-')
	{
	  op_end = parse_reg (op_end + 1, & reg);
	  
	  if (reg != 15)
	    as_bad (_("ending register must be r15"));
	  
	  /* Skip whitespace.  */
	  while (isspace (* op_end))
	    ++ op_end;
	}
      
      if (* op_end == ',')
	{
	  op_end ++;
	  
	  /* Skip whitespace.  */
	  while (isspace (* op_end))
	    ++ op_end;
  
	  if (* op_end == '(')
	    {
	      op_end = parse_reg (op_end + 1, & reg);
	      
	      if (reg != 0)
		as_bad (_("bad base register: must be r0"));
	      
	      if (* op_end == ')')
		op_end ++;
	    }
	  else
	    as_bad (_("base register expected"));
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case RQ:
      op_end = parse_reg (op_end + 1, & reg);
      
      if (reg != 4)
	as_fatal (_("first register must be r4"));
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == '-')
	{
	  op_end = parse_reg (op_end + 1, & reg);
	  
	  if (reg != 7)
	    as_fatal (_("last register must be r7"));
	  
	  /* Skip whitespace.  */
	  while (isspace (* op_end))
	    ++ op_end;
  
	  if (* op_end == ',')
	    {
	      op_end ++;
	      
	      /* Skip whitespace.  */
	      while (isspace (* op_end))
		++ op_end;
  
	      if (* op_end == '(')
		{
		  op_end = parse_reg (op_end + 1, & reg);
		  
		  if (reg >= 4 && reg <= 7)
		    as_fatal ("base register cannot be r4, r5, r6, or r7");
		  
		  inst |= reg;
		  
		  /* Skip whitespace.  */
		  while (isspace (* op_end))
		    ++ op_end;
		  
		  if (* op_end == ')')
		    op_end ++;
		}
	      else
		as_bad (_("base register expected"));
	    }
	  else
	    as_bad (_("second operand missing"));
	}
      else
	as_bad (_("reg-reg expected"));
      
      output = frag_more (2);
      break;
      
    case BR:
      input_line_pointer = parse_exp (op_end + 1, & e);
      
      output = frag_more (2);
      
      fix_new_exp (frag_now, output-frag_now->fr_literal, 
		   2, & e, 1, BFD_RELOC_MCORE_PCREL_IMM11BY2);
      break;
      
    case BL:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg << 4;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_exp (op_end + 1, & e);
	  output = frag_more (2);
	  
	  fix_new_exp (frag_now, output-frag_now->fr_literal, 
		       2, & e, 1, BFD_RELOC_MCORE_PCREL_IMM4BY2);
	}
      else
	{
	  as_bad (_("second operand missing"));
	  output = frag_more (2);
	}
      break;
      
    case JC:
      input_line_pointer = parse_exp (op_end + 1, & e);
      
      output = frag_var (rs_machine_dependent,
			 md_relax_table[C (COND_JUMP, COND32)].rlx_length,
			 md_relax_table[C (COND_JUMP, COND12)].rlx_length,
			 C (COND_JUMP, 0), e.X_add_symbol, e.X_add_number, 0);
      isize = C32_LEN;
      break;
      
    case JU:
      input_line_pointer = parse_exp (op_end + 1, & e);
      output = frag_var (rs_machine_dependent,
			 md_relax_table[C (UNCD_JUMP, UNCD32)].rlx_length,
			 md_relax_table[C (UNCD_JUMP, UNCD12)].rlx_length,
			 C (UNCD_JUMP, 0), e.X_add_symbol, e.X_add_number, 0);
      isize = U32_LEN;
      break;
      
    case JL:
      inst = MCORE_INST_JSRI;		/* jsri */
      input_line_pointer = parse_rt (op_end + 1, & output, 1, & e);
      /* parse_rt() calls frag_more for us.  */
      
      /* Only do this if we know how to do it ... */
      if (e.X_op != O_absent && do_jsri2bsr)
	{
	  /* Look at adding the R_PCREL_JSRIMM11BY2.  */
	  fix_new_exp (frag_now, output-frag_now->fr_literal, 
		       2, & e, 1, BFD_RELOC_MCORE_PCREL_JSR_IMM11BY2);
	}
      break;

    case RSI:				/* SI, but imm becomes 32-imm */
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 31);
	  
	  reg = 32 - reg;
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case DO21:			/* O2, dup rd, lit must be 1 */
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      inst |= reg << 4;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 31);
	  
	  if (reg != 1)
	    as_bad (_("second operand must be 1"));
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;
      
    case SIa:
      op_end = parse_reg (op_end + 1, & reg);
      inst |= reg;
      
      /* Skip whitespace.  */
      while (isspace (* op_end))
	++ op_end;
  
      if (* op_end == ',')
	{
	  op_end = parse_imm (op_end + 1, & reg, 1, 31);

	  if (reg == 0)
	    as_bad (_("zero used as immediate value"));
	  
	  inst |= reg << 4;
	}
      else
	as_bad (_("second operand missing"));
      
      output = frag_more (2);
      break;

    default:
      as_bad (_("unimplemented opcode \"%s\""), name);
    }
  
  output[0] = INST_BYTE0 (inst);
  output[1] = INST_BYTE1 (inst);
  
  check_literals (opcode->transfer, isize);
}

symbolS *
md_undefined_symbol (name)
       char * name;
{
  return 0;
}

void
md_mcore_end ()
{
  dump_literals (0);
  subseg_set (text_section, 0);
}

/* Various routines to kill one day.  */
/* Equal to MAX_PRECISION in atof-ieee.c */
#define MAX_LITTLENUMS 6

/* Turn a string in input_line_pointer into a floating point constant of type
   type, and store the appropriate bytes in *litP.  The number of LITTLENUMS
   emitted is stored in *sizeP.  An error message is returned, or NULL on OK.*/
char *
md_atof (type, litP, sizeP)
     int type;
     char * litP;
     int * sizeP;
{
  int prec;
  LITTLENUM_TYPE words[MAX_LITTLENUMS];
  int    i;
  char * t;
  char * atof_ieee ();

  switch (type)
    {
    case 'f':
    case 'F':
    case 's':
    case 'S':
      prec = 2;
      break;

    case 'd':
    case 'D':
    case 'r':
    case 'R':
      prec = 4;
      break;

    case 'x':
    case 'X':
      prec = 6;
      break;

    case 'p':
    case 'P':
      prec = 6;
      break;

    default:
      *sizeP = 0;
      return _("Bad call to MD_NTOF()");
    }
  
  t = atof_ieee (input_line_pointer, type, words);
  
  if (t)
    input_line_pointer = t;

  *sizeP = prec * sizeof (LITTLENUM_TYPE);
  
    for (i = 0; i < prec; i++)
      {
	md_number_to_chars (litP, (valueT) words[i],
			    sizeof (LITTLENUM_TYPE));
	litP += sizeof (LITTLENUM_TYPE);
      }
  
  return 0;
}

CONST char * md_shortopts = "";

#define OPTION_JSRI2BSR_ON	(OPTION_MD_BASE + 0)
#define OPTION_JSRI2BSR_OFF	(OPTION_MD_BASE + 1)
#define OPTION_SIFILTER_ON	(OPTION_MD_BASE + 2)
#define OPTION_SIFILTER_OFF	(OPTION_MD_BASE + 3)

struct option md_longopts[] =
{
  { "no-jsri2bsr", no_argument, NULL, OPTION_JSRI2BSR_OFF},
  { "jsri2bsr",    no_argument, NULL, OPTION_JSRI2BSR_ON},
  { "sifilter",    no_argument, NULL, OPTION_SIFILTER_ON},
  { "no-sifilter", no_argument, NULL, OPTION_SIFILTER_OFF},
  { NULL,          no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

int
md_parse_option (c, arg)
     int c;
     char * arg;
{
  int i;
  char * p;

  switch (c)
    {
      
    case OPTION_JSRI2BSR_ON:  do_jsri2bsr = 1;   break;
    case OPTION_JSRI2BSR_OFF: do_jsri2bsr = 0;   break;
    case OPTION_SIFILTER_ON:  sifilter_mode = 1; break;
    case OPTION_SIFILTER_OFF: sifilter_mode = 0; break;
    default:                  return 0;
    }

  return 1;
}

void
md_show_usage (stream)
     FILE * stream;
{
  fprintf (stream, _("\
MCORE specific options:\n\
  -{no-}jsri2bsr	  {dis}able jsri to bsr transformation (def: dis)\n\
  -{no-}sifilter	  {dis}able silicon filter behavior (def: dis)"));
}

int md_short_jump_size;

void
md_create_short_jump (ptr, from_Nddr, to_Nddr, frag, to_symbol)
     char * ptr;
     addressT from_Nddr;
     addressT to_Nddr;
     fragS * frag;
     symbolS * to_symbol;
{
  as_fatal (_("failed sanity check: short_jump"));
}

void
md_create_long_jump (ptr, from_Nddr, to_Nddr, frag, to_symbol)
     char * ptr;
     addressT from_Nddr;
     addressT to_Nddr;
     fragS * frag;
     symbolS * to_symbol;
{
  as_fatal (_("failed sanity check: long_jump"));
}

/* Called after relaxing, change the frags so they know how big they are.  */
void
md_convert_frag (abfd, sec, fragP)
     bfd * abfd;
     segT sec;
     register fragS * fragP;
{
  unsigned char * buffer;
  int targ_addr = S_GET_VALUE (fragP->fr_symbol) + fragP->fr_offset;
  
  buffer = (unsigned char *) (fragP->fr_fix + fragP->fr_literal);
  targ_addr += symbol_get_frag (fragP->fr_symbol)->fr_address;

  switch (fragP->fr_subtype)
    {
    case C (COND_JUMP, COND12):
    case C (UNCD_JUMP, UNCD12):
      {
	/* Get the address of the end of the instruction.  */
	int next_inst = fragP->fr_fix + fragP->fr_address + 2;
	unsigned char t0;
	int disp = targ_addr - next_inst;
	
	if (disp & 1)
	  as_bad (_("odd displacement at %x"), next_inst - 2);
	
	disp >>= 1;
	  {
	    t0 = buffer[0] & 0xF8;
	
	    md_number_to_chars (buffer, disp, 2);
	
	    buffer[0] = (buffer[0] & 0x07) | t0;
	  }
	
	fragP->fr_fix += 2;
	fragP->fr_var = 0;
      }
      break;

    case C (COND_JUMP, COND32):
    case C (COND_JUMP, UNDEF_WORD_DISP):
      {
	/* A conditional branch wont fit into 12 bits so:
	 *	b!cond	1f
	 *	jmpi	0f
	 *	.align 2
	 * 0:	.long disp
	 * 1:
	 *
	 * if the b!cond is 4 byte aligned, the literal which would
	 * go at x+4 will also be aligned.
	 */
	int first_inst = fragP->fr_fix + fragP->fr_address;
	int needpad = (first_inst & 3);

	  buffer[0] ^= 0x08;	/* Toggle T/F bit */

	buffer[2] = INST_BYTE0 (MCORE_INST_JMPI);	/* Build jmpi */
	buffer[3] = INST_BYTE1 (MCORE_INST_JMPI);
 
	if (needpad)
	  {
	      {
		buffer[1] = 4;	/* branch over jmpi, pad, and ptr */
		buffer[3] = 1;	/* jmpi offset of 1 gets the pointer */
	      }
	    
	    buffer[4] = 0;	/* alignment/pad */
	    buffer[5] = 0;
	    buffer[6] = 0;	/* space for 32 bit address */
	    buffer[7] = 0;
	    buffer[8] = 0;
	    buffer[9] = 0;
	    
	    /* Make reloc for the long disp */
	    fix_new (fragP, fragP->fr_fix + 6, 4,
		     fragP->fr_symbol, fragP->fr_offset, 0, BFD_RELOC_32);
	    
	    fragP->fr_fix += C32_LEN;
	  }
	else
	  {
	    /* See comment below about this given gas' limitations for
	       shrinking the fragment. '3' is the amount of code that
	       we inserted here, but '4' is right for the space we reserved
	       for this fragment. */
	      {
		buffer[1] = 3;	/* branch over jmpi, and ptr */
		buffer[3] = 0;	/* jmpi offset of 0 gets the pointer */
	      }
	    
	    buffer[4] = 0;	/* space for 32 bit address */
	    buffer[5] = 0;
	    buffer[6] = 0;
	    buffer[7] = 0;
	    
	    /* Make reloc for the long disp.  */
	    fix_new (fragP, fragP->fr_fix + 4, 4,
		     fragP->fr_symbol, fragP->fr_offset, 0, BFD_RELOC_32);
	    fragP->fr_fix += C32_LEN;

	    /* Frag is actually shorter (see the other side of this ifdef)
	       but gas isn't prepared for that.  We have to re-adjust
	       the branch displacement so that it goes beyond the 
	       full length of the fragment, not just what we actually
	       filled in.  */
	      buffer[1] = 4;	/* jmpi, ptr, and the 'tail pad' */
	  }
	
	fragP->fr_var = 0;
      }
      break;

    case C (UNCD_JUMP, UNCD32):
    case C (UNCD_JUMP, UNDEF_WORD_DISP):
      {
	/* An unconditional branch will not fit in 12 bits, make code which
	   looks like:
	  	jmpi	0f
	  	.align 2
	     0:	.long disp
	   we need a pad if "first_inst" is 4 byte aligned.
	   [because the natural literal place is x + 2]  */
	int first_inst = fragP->fr_fix + fragP->fr_address;
	int needpad = !(first_inst & 3);

	buffer[0] = INST_BYTE0 (MCORE_INST_JMPI);	/* Build jmpi */
	buffer[1] = INST_BYTE1 (MCORE_INST_JMPI);

	if (needpad)
	  {
	      buffer[1] = 1;	/* jmpi offset of 1 since padded */
	    buffer[2] = 0;	/* alignment */
	    buffer[3] = 0;
	    buffer[4] = 0;	/* space for 32 bit address */
	    buffer[5] = 0;
	    buffer[6] = 0;
	    buffer[7] = 0;
	    
	    /* Make reloc for the long disp.  */
	    fix_new (fragP, fragP->fr_fix + 4, 4,
		     fragP->fr_symbol, fragP->fr_offset, 0, BFD_RELOC_32);
	    
	    fragP->fr_fix += U32_LEN;
	  }
	else
	  {
	      buffer[1] = 0;	/* jmpi offset of 0 if no pad */
	    buffer[2] = 0;	/* space for 32 bit address */
	    buffer[3] = 0;
	    buffer[4] = 0;
	    buffer[5] = 0;
	    
	    /* Make reloc for the long disp.  */
	    fix_new (fragP, fragP->fr_fix + 2, 4,
		     fragP->fr_symbol, fragP->fr_offset, 0, BFD_RELOC_32);
	    fragP->fr_fix += U32_LEN;
	  }

	fragP->fr_var = 0;
      }
      break;

    default:
      abort ();
    }
}

/* Applies the desired value to the specified location.
   Also sets up addends for 'rela' type relocations.  */
int
md_apply_fix3 (fixP, valp, segment)
     fixS *   fixP;
     valueT * valp;
     segT     segment;
{
  char *       buf  = fixP->fx_where + fixP->fx_frag->fr_literal;
  char *       file = fixP->fx_file ? fixP->fx_file : _("unknown");
  const char * symname;
  /* Note: use offsetT because it is signed, valueT is unsigned.  */
  offsetT      val  = (offsetT) * valp; 
  
  symname = fixP->fx_addsy ? S_GET_NAME (fixP->fx_addsy) : _("<unknown>");
  /* Save this for the addend in the relocation record.  */
  fixP->fx_addnumber = val;

  /* If the fix is relative to a symbol which is not defined, or not
     in the same segment as the fix, we cannot resolve it here.  */
  if (fixP->fx_addsy != NULL
      && (   ! S_IS_DEFINED (fixP->fx_addsy)
	  || (S_GET_SEGMENT (fixP->fx_addsy) != segment)))
    {
      fixP->fx_done = 0;
#ifdef OBJ_ELF
      /* For ELF we can just return and let the reloc that will be generated
	 take care of everything.  For COFF we still have to insert 'val'
	 into the insn since the addend field will be ignored.  */
      return 0;
#endif
    }
  else
    fixP->fx_done = 1;
  
  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_MCORE_PCREL_IMM11BY2:     /* second byte of 2 byte opcode */
      if ((val & 1) != 0)
	as_bad_where (file, fixP->fx_line,
		      _("odd distance branch (0x%x bytes)"), val);
      val /= 2;
      if (((val & ~0x3ff) != 0) && ((val | 0x3ff) != -1))
	as_bad_where (file, fixP->fx_line,
		      _("pcrel for branch to %s too far (0x%x)"),
		      symname, val);
        buf[0] |= ((val >> 8) & 0x7);
        buf[1] |= (val & 0xff);
      break;

    case BFD_RELOC_MCORE_PCREL_IMM8BY4:	/* lower 8 bits of 2 byte opcode */
      val += 3;
      val /= 4;
      if (val & ~0xff)
	as_bad_where (file, fixP->fx_line,
		      _("pcrel for lrw/jmpi/jsri to %s too far (0x%x)"),
		      symname, val);
      else
	buf[1] |= (val & 0xff);
      break;

    case BFD_RELOC_MCORE_PCREL_IMM4BY2:	/* loopt instruction */
      if ((val < -32) || (val > -2))
	as_bad_where (file, fixP->fx_line,
		      _("pcrel for loopt too far (0x%x)"), val);
      val /= 2;
        buf[1] |= (val & 0xf);
      break;

    case BFD_RELOC_MCORE_PCREL_JSR_IMM11BY2:
      /* Conditional linker map jsri to bsr.  */
      /* If its a local target and close enough, fix it. 
         NB: >= -2k for backwards bsr; < 2k for forwards...  */
      if (fixP->fx_addsy == 0 && val >= -2048  && val < 2048)
	{
	  long nval = (val / 2) & 0x7ff;
	  nval |= MCORE_INST_BSR;
	  
	  /* REPLACE the instruction, don't just modify it.  */
	  buf[0] = INST_BYTE0 (nval);
	  buf[1] = INST_BYTE1 (nval);
	}
      else
	fixP->fx_done = 0;
      break;

    case BFD_RELOC_MCORE_PCREL_32:
    case BFD_RELOC_VTABLE_INHERIT:
    case BFD_RELOC_VTABLE_ENTRY:
      fixP->fx_done = 0;
      break;
      
    default:
      if (fixP->fx_addsy != NULL)
	{
	  /* If the fix is an absolute reloc based on a symbol's
	     address, then it cannot be resolved until the final link.  */
	  fixP->fx_done = 0;
	}
#ifdef OBJ_ELF      
      else
#endif
	{
	  if (fixP->fx_size == 4)
	    ;
	  else if (fixP->fx_size == 2 && val >= -32768 && val <= 32767)
	    ;
	  else if (fixP->fx_size == 1 && val >= -256 && val <= 255)
	    ;
	  else
	    abort ();
	  md_number_to_chars (buf, val, fixP->fx_size);
	}
      break;
    }

  return 0; /* Return value is ignored.  */
}

void
md_operand (expressionP)
     expressionS * expressionP;
{
  /* Ignore leading hash symbol, if poresent.  */
  if (* input_line_pointer == '#')
    {
      input_line_pointer ++;
      expression (expressionP);
    }
}

int md_long_jump_size;

/* Called just before address relaxation, return the length
   by which a fragment must grow to reach it's destination.  */
int
md_estimate_size_before_relax (fragP, segment_type)
     register fragS * fragP;
     register segT segment_type;
{
  switch (fragP->fr_subtype)
    {
    case C (UNCD_JUMP, UNDEF_DISP):
      /* Used to be a branch to somewhere which was unknown.  */
      if (!fragP->fr_symbol)
	{
	  fragP->fr_subtype = C (UNCD_JUMP, UNCD12);
	  fragP->fr_var = md_relax_table[C (UNCD_JUMP, UNCD12)].rlx_length;
	}
      else if (S_GET_SEGMENT (fragP->fr_symbol) == segment_type)
	{
	  fragP->fr_subtype = C (UNCD_JUMP, UNCD12);
	  fragP->fr_var = md_relax_table[C (UNCD_JUMP, UNCD12)].rlx_length;
	}
      else
	{
	  fragP->fr_subtype = C (UNCD_JUMP, UNDEF_WORD_DISP);
	  fragP->fr_var = md_relax_table[C (UNCD_JUMP, UNCD32)].rlx_length;
	  return md_relax_table[C (UNCD_JUMP, UNCD32)].rlx_length;
	}
      break;

    default:
      abort ();
      
    case C (COND_JUMP, UNDEF_DISP):
      /* Used to be a branch to somewhere which was unknown. */
      if (fragP->fr_symbol
	  && S_GET_SEGMENT (fragP->fr_symbol) == segment_type)
	{
	  /* Got a symbol and it's defined in this segment, become byte
	     sized - maybe it will fix up */
	  fragP->fr_subtype = C (COND_JUMP, COND12);
	  fragP->fr_var = md_relax_table[C (COND_JUMP, COND12)].rlx_length;
	}
      else if (fragP->fr_symbol)
	{
	  /* Its got a segment, but its not ours, so it will always be long.  */
	  fragP->fr_subtype = C (COND_JUMP, UNDEF_WORD_DISP);
	  fragP->fr_var = md_relax_table[C (COND_JUMP, COND32)].rlx_length;
	  return md_relax_table[C (COND_JUMP, COND32)].rlx_length;
	}
      else
	{
	  /* We know the abs value.  */
	  fragP->fr_subtype = C (COND_JUMP, COND12);
	  fragP->fr_var = md_relax_table[C (COND_JUMP, COND12)].rlx_length;
	}

      break;
    }
  
  return fragP->fr_var;
}

/* Put number into target byte order.  */
void
md_number_to_chars (ptr, use, nbytes)
     char * ptr;
     valueT use;
     int nbytes;
{
    switch (nbytes)
      {
      case 4: *ptr++ = (use >> 24) & 0xff; /* fall through */
      case 3: *ptr++ = (use >> 16) & 0xff; /* fall through */
      case 2: *ptr++ = (use >>  8) & 0xff; /* fall through */
      case 1: *ptr++ = (use >>  0) & 0xff;    break;
      default: abort ();
      }
}

/* Round up a section size to the appropriate boundary.  */
valueT
md_section_align (segment, size)
     segT segment;
     valueT size;
{
  return size;			/* Byte alignment is fine */
}


/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */
long
md_pcrel_from_section (fixp, sec)
     fixS * fixp;
     segT sec;
{
#ifdef OBJ_ELF
  /* If the symbol is undefined or defined in another section
     we leave the add number alone for the linker to fix it later.
     Only account for the PC pre-bump (which is 2 bytes on the MCore). */
  if (fixp->fx_addsy != (symbolS *) NULL
      && (! S_IS_DEFINED (fixp->fx_addsy)
	  || (S_GET_SEGMENT (fixp->fx_addsy) != sec)))
      
  {
    assert (fixp->fx_size == 2);	/* must be an insn */
    return fixp->fx_size;
  }
#endif

  /* The case where we are going to resolve things... */
  return  fixp->fx_size + fixp->fx_where + fixp->fx_frag->fr_address;
}

#define F(SZ,PCREL)		(((SZ) << 1) + (PCREL))
#define MAP(SZ,PCREL,TYPE)	case F (SZ, PCREL): code = (TYPE); break

arelent *
tc_gen_reloc (section, fixp)
     asection * section;
     fixS * fixp;
{
  arelent * rel;
  bfd_reloc_code_real_type code;
  int handled = 0;

  switch (fixp->fx_r_type)
    {
      /* These confuse the size/pcrel macro approach. */
    case BFD_RELOC_VTABLE_INHERIT:
    case BFD_RELOC_VTABLE_ENTRY:
    case BFD_RELOC_MCORE_PCREL_IMM4BY2:
    case BFD_RELOC_MCORE_PCREL_IMM8BY4:
    case BFD_RELOC_MCORE_PCREL_IMM11BY2:
    case BFD_RELOC_MCORE_PCREL_JSR_IMM11BY2:
      code = fixp->fx_r_type;
      break;
    
    default:
      switch (F (fixp->fx_size, fixp->fx_pcrel))
        {
          MAP (1, 0, BFD_RELOC_8);
          MAP (2, 0, BFD_RELOC_16);
          MAP (4, 0, BFD_RELOC_32);
          MAP (1, 1, BFD_RELOC_8_PCREL);
          MAP (2, 1, BFD_RELOC_16_PCREL);
          MAP (4, 1, BFD_RELOC_32_PCREL);
        default:
	  code = fixp->fx_r_type;
          as_bad (_("Can not do %d byte %srelocation"),
		  fixp->fx_size,
	          fixp->fx_pcrel ? _("pc-relative") : "");
        }
      break;
  }

  rel = (arelent *) xmalloc (sizeof (arelent));
  rel->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
  *rel->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  rel->address = fixp->fx_frag->fr_address + fixp->fx_where;
  /* Always pass the addend along!  */
  rel->addend = fixp->fx_addnumber;

  rel->howto = bfd_reloc_type_lookup (stdoutput, code);
  
  if (rel->howto == NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("Cannot represent relocation type %s"),
                    bfd_get_reloc_code_name (code));
      
      /* Set howto to a garbage value so that we can keep going.  */
      rel->howto = bfd_reloc_type_lookup (stdoutput, BFD_RELOC_32);
      assert (rel->howto != NULL);
    }

  return rel;
}

#ifdef OBJ_ELF
/* See whether we need to force a relocation into the output file.
   This is used to force out switch and PC relative relocations when
   relaxing.  */
int
mcore_force_relocation (fix)
     fixS * fix;
{
  if (   fix->fx_r_type == BFD_RELOC_VTABLE_INHERIT
      || fix->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    return 1;

  return 0;
}

/* Return true if the fix can be handled by GAS, false if it must
   be passed through to the linker.  */
boolean
mcore_fix_adjustable (fixP)
   fixS * fixP;
{
  if (fixP->fx_addsy == NULL)
    return 1;
  
  /* We need the symbol name for the VTABLE entries.  */
  if (   fixP->fx_r_type == BFD_RELOC_VTABLE_INHERIT
      || fixP->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    return 0;

  return 1;
}
#endif /* OBJ_ELF */
