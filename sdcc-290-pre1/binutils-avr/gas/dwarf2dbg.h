/* dwarf2dbg.h - DWARF2 debug support
   Copyright (C) 1999 Free Software Foundation, Inc.

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

#ifndef AS_DWARF2DBG_H
#define AS_DWARF2DBG_H

#include "as.h"

#define DWARF2_FLAG_BEGIN_STMT	(1 << 0)	/* beginning of statement */
#define DWARF2_FLAG_BEGIN_BLOCK	(1 << 1)	/* beginning of basic block */

struct dwarf2_line_info
  {
    char *filename;
    unsigned int filenum;
    unsigned int line;
    unsigned int column;
    unsigned int flags;
  };

/* Implements the .file FILENO "FILENAME" directive.  FILENO can be 0
   to indicate that no file number has been assigned.  All real file
   number must be >0.  */
extern void dwarf2_directive_file PARAMS ((int dummy));

/* Implements the .loc FILENO LINENO [COLUMN] directive.  FILENO is
   the file number, LINENO the line number and the (optional) COLUMN
   the column of the source code that the following instruction
   corresponds to.  FILENO can be 0 to indicate that the filename
   specified by the textually most recent .file directive should be
   used.  */
extern void dwarf2_directive_loc PARAMS ((int dummy));

/* Returns the current source information.  If .file directives have
   been encountered, the info for the corresponding source file is
   returned.  Otherwise, the info for the assembly source file is
   returned.  */
extern void dwarf2_where PARAMS ((struct dwarf2_line_info *l));

/* This function generates .debug_line info based on the address and
   source information passed in the arguments.  ADDR should be the
   frag-relative offset of the instruction the information is for and
   L is the source information that should be associated with that
   address. */
extern void dwarf2_gen_line_info PARAMS ((addressT addr,
					  struct dwarf2_line_info *l));

/* Must be called after all other input is processed to finish up the
   .debug_line section.  */
extern void dwarf2_finish PARAMS ((void));

#endif /* AS_DWARF2DBG_H */
