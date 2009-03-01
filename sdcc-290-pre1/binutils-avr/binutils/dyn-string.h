/* An abstract string datatype.
   Copyright (C) 1998 Free Software Foundation, Inc.
   Contributed by Mark Mitchell (mark@markmitchell.com).

   This file is part of GNU CC.
   
   GNU CC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* This file lives in at least two places: binutils and gcc.
   Don't change one without the other.  */

typedef struct dyn_string
{
  int allocated; /* The amount of space allocated for the string.  */
  int length; /* The actual length of the string.  */
  char *s; /* The string itself, NUL-terminated.  */
}* dyn_string_t;

extern dyn_string_t dyn_string_new      PARAMS((int));
extern void dyn_string_delete           PARAMS((dyn_string_t));
extern dyn_string_t dyn_string_append   PARAMS((dyn_string_t, char*));
extern dyn_string_t dyn_string_resize   PARAMS((dyn_string_t, int));
