/* z280mch.c */

/*
 *  Copyright (C) 1989-2014  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */

/*  Copyright (c) 2017  John R. Coffman
 *
 *  New approach to assembly for Z80/Z180/Z280 based on the work of
 *  Alan Baldwin.  The updates are made under the GPL 3 license
 *  agreement.
 */

#include "asxxxx.h"
#include "z280def.h"

/*#include "z280opcy.h"		(included below)  */

char	*cpu	= "2/14/17 Zilog Z80 / Z180 / HD64180 / Z280";
char	*dsft	= "asm";

a_uint	mchtyp;	/* ARB */
a_uint	debug;	/* ARB */

#define	UN	((char) (OPCY_NONE | 0x00))
#define	P2	((char) (OPCY_NONE | 0x01))
#define	P3	((char) (OPCY_NONE | 0x02))
#define	P4	((char) (OPCY_NONE | 0x03))
#define	P5	((char) (OPCY_NONE | 0x04))
#define	P6	((char) (OPCY_NONE | 0x05))
#define	P7	((char) (OPCY_NONE | 0x06))
#define	P8	((char) (OPCY_NONE | 0x07))
#define	P9	((char) (OPCY_NONE | 0x08))
#define	P10	((char) (OPCY_NONE | 0x09))
#define	P11	((char) (OPCY_NONE | 0x0A))


/*
 * Z80 Opcode Cycle Pages
 */

static char  z80pg1[256] = {
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   4,10, 7, 6, 4, 4, 7, 4, 4,11, 7, 6, 4, 4, 7, 4,
/*10*/  13,10, 7, 6, 4, 4, 7, 4,12,11, 7, 6, 4, 4, 7, 4,
/*20*/  12,10,16, 6, 4, 4, 7, 4,12,11,16, 6, 4, 4, 7, 4,
/*30*/  12,10,13, 6,11,11,10, 4,12,11,13, 6, 4, 4, 7, 4,
/*40*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*50*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*60*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*70*/   7, 7, 7, 7, 7, 7, 4, 7, 4, 4, 4, 4, 4, 4, 7, 4,
/*80*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*90*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*A0*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*B0*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/*C0*/  11,10,10,10,17,11, 7,11,11,10,10,P2,17,17, 7,11,
/*D0*/  11,10,10,11,17,11, 7,11,11, 4,10,11,17,P3, 7,11,
/*E0*/  11,10,10,19,17,11, 7,11,11, 4,10, 4,17,P4, 7,11,
/*F0*/  11,10,10, 4,17,11, 7,11,11, 6,10, 4,17,P5, 7,11
};

static char  z80pg2[256] = {  /* P2 == CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*10*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*20*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*30*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*40*/   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
/*50*/   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
/*60*/   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
/*70*/   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
/*80*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*90*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*A0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*B0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*C0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*D0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*E0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
/*F0*/   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8
};

static char  z80pg3[256] = {  /* P3 == DD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*20*/  UN,14,20,10, 7, 7,10,UN,UN,15,20,10, 7, 7,10,UN,
/*30*/  UN,UN,UN,UN,23,23,19,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*50*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*60*/   7,UN, 7,UN, 7, 7,19, 7,UN, 7,UN, 7, 7, 7,19, 7,
/*70*/  19,19,19,19,19,19,UN,19,UN,UN,UN,UN, 7, 7,19,UN,
/*80*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*90*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*A0*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*B0*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,P6,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,14,UN,23,UN,15,UN,UN,UN, 8,UN,UN,UN,P8,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN
};

static char  z80pg4[256] = {  /* P4 == ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*20*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*40*/  12,12,15,20, 8,14, 8, 9,12,12,15,20,UN,14,UN, 9,
/*50*/  12,12,15,20,UN,UN, 8, 9,12,12,15,20,UN,UN, 8, 9,
/*60*/  12,12,15,20,UN,UN,UN,18,12,12,15,20,UN,UN,UN,18,
/*70*/  UN,UN,15,20,UN,UN,UN,UN,12,12,15,20,UN,UN,UN,UN,
/*80*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*90*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*A0*/  16,16,16,16,UN,UN,UN,UN,16,16,16,16,UN,UN,UN,UN,
/*B0*/  21,21,21,21,UN,UN,UN,UN,21,21,21,21,UN,UN,UN,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN
};

static char  z80pg5[256] = {  /* P5 == FD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*20*/  UN,14,20,10, 7, 7,10,UN,UN,15,20,10, 7, 7,10,UN,
/*30*/  UN,UN,UN,UN,23,23,19,UN,UN,15,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*50*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*60*/   7,UN, 7,UN, 7, 7,19, 7,UN, 7,UN, 7, 7, 7,19, 7,
/*70*/  19,19,19,19,19,19,UN,19,UN,UN,UN,UN, 7, 7,19,UN,
/*80*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*90*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*A0*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*B0*/  UN,UN,UN,UN, 7, 7,19,UN,UN,UN,UN,UN, 7, 7,19,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,P7,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,14,UN,23,UN,15,UN,UN,UN, 8,UN,UN,UN,P9,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN
};

static char  z80pg6[256] = {  /* P6 == DD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*10*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*20*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*30*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*40*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*50*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*60*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*70*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*80*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*90*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN
};

static char  z80pg7[256] = {  /* P7 == FD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*10*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*20*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*30*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*40*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*50*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*60*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*70*/  UN,UN,UN,UN,UN,UN,20,UN,UN,UN,UN,UN,UN,UN,20,UN,
/*80*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*90*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,23,UN,UN,UN,UN,UN,UN,UN,23,UN
};

static char  z80pg8[256] = {  /* P8 == DD ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*UN*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*50*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*60*/  15,15,UN,UN,UN,UN,UN,UN,15,15,UN,UN,UN,UN,UN,UN,
/*70*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*80*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*90*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN
};

static char  z80pg9[256] = {  /* P9 == FD ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*UN*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*50*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*60*/  15,15,UN,UN,UN,UN,UN,UN,15,15,UN,UN,UN,UN,UN,UN,
/*70*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*80*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*90*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN
};

static char *z80Page[9] = {
    z80pg1, z80pg2, z80pg3, z80pg4,
    z80pg5, z80pg6, z80pg7, z80pg8,
    z80pg9
};

/*
 * HD64180 / Z180  Opcode Cycle Pages
 */

static char  hd64pg1[256] = {
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   3, 9, 7, 4, 4, 4, 6, 3, 4, 7, 6, 4, 4, 4, 6, 3,
/*10*/   9, 9, 7, 4, 4, 4, 6, 3, 8, 7, 6, 4, 4, 4, 6, 3,
/*20*/   8, 9,16, 4, 4, 4, 6, 4, 8, 7,15, 4, 4, 4, 6, 3,
/*30*/   8, 9,13, 4,10,10, 9, 3, 8, 7,12, 4, 4, 4, 6, 3,
/*40*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*50*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*60*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*70*/   7, 7, 7, 7, 7, 7, 3, 7, 4, 4, 4, 4, 4, 4, 6, 4,
/*80*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*90*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*A0*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*B0*/   4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4,
/*C0*/  10, 9, 9, 9,16,11, 6,11,10, 9, 9,P2,16,16, 6,11,
/*D0*/  10, 9, 9,10,16,11, 6,11,10, 3, 9, 9,16,P3, 6,11,
/*E0*/  10, 9, 9,16,16,11, 6,11,10, 3, 9, 3,16,P4, 6,11,
/*F0*/  10, 9, 9, 3,16,11, 6,11,10, 4, 9, 3,16,P5, 6,11
};

static char  hd64pg2[256] = {  /* P2 == CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*10*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*20*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN, 7, 7, 7, 7, 7, 7,13, 7,
/*40*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*50*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*60*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*70*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*80*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*90*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*A0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*B0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7, 
/*C0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*D0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*E0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7,
/*F0*/   7, 7, 7, 7, 7, 7,13, 7, 7, 7, 7, 7, 7, 7,13, 7
};

static char  hd64pg3[256] = {  /* P3 == DD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*20*/  UN,12,19, 7,UN,UN,UN,UN,UN,10,18, 7,UN,UN,UN,UN,
/*30*/  UN,UN,UN,UN,18,18,15,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*50*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*60*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*70*/  15,15,15,15,15,15,UN,15,UN,UN,UN,UN,UN,UN,14,UN,
/*80*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*90*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,P6,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,12,UN,19,UN,14,UN,UN,UN, 6,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN, 7,UN,UN,UN,UN,UN,UN
};

static char  hd64pg4[256] = {  /* P4 == ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  12,13,UN,UN, 7,UN,UN,UN,12,13,UN,UN, 7,UN,UN,UN,
/*10*/  12,13,UN,UN, 7,UN,UN,UN,12,13,UN,UN, 7,UN,UN,UN,
/*20*/  12,13,UN,UN, 7,UN,UN,UN,12,13,UN,UN, 7,UN,UN,UN,
/*30*/  UN,UN,UN,UN,10,UN,UN,UN,12,13,UN,UN, 7,UN,UN,UN,
/*40*/   9,10,10,19, 6,12, 6, 6, 9,10,10,18,17,12,UN, 6,
/*50*/   9,10,10,19,UN,UN, 6, 6, 9,10,10,18,17,UN, 6, 6,
/*60*/   9,10,10,19, 9,UN,UN,16, 9,10,10,18,17,UN,UN,16,
/*70*/  UN,10,10,19,12,UN, 8,UN, 9,10,10,18,17,UN,UN,UN,
/*80*/  UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,
/*90*/  UN,UN,UN,16,UN,UN,UN,UN,UN,UN,UN,16,UN,UN,UN,UN,
/*A0*/  12,12,12,12,UN,UN,UN,UN,12,12,12,12,UN,UN,UN,UN,
/*B0*/  14,14,14,14,UN,UN,UN,UN,14,14,14,14,UN,UN,UN,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN
};

static char  hd64pg5[256] = {  /* P5 == FD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*10*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*20*/  UN,12,19, 7,UN,UN,UN,UN,UN,10,18, 7,UN,UN,UN,UN,
/*30*/  UN,UN,UN,UN,18,18,15,UN,UN,10,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*50*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*60*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*70*/  15,15,15,15,15,15,UN,15,UN,UN,UN,UN,UN,UN,14,UN,
/*80*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*90*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,P7,UN,UN,UN,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*E0*/  UN,12,UN,19,UN,14,UN,UN,UN, 6,UN,UN,UN,UN,UN,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN, 7,UN,UN,UN,UN,UN,UN
};

static char  hd64pg6[256] = {  /* P6 == DD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*10*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*20*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*40*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*50*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*60*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*70*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*80*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*90*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN
};

static char  hd64pg7[256] = {  /* P7 == FD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*10*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*20*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*30*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*40*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*50*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*60*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*70*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*80*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*90*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,19,UN,UN,UN,UN,UN,UN,UN,19,UN
};

static char *hd64Page[7] = {
    hd64pg1, hd64pg2, hd64pg3, hd64pg4,
    hd64pg5, hd64pg6, hd64pg7
};



#include "z280opcy.h"
#include "z280ops.h"

/*
 * Process a machine op.
 */
VOID
machine(mp)
struct mne *mp;
{
	char **popcy[7];
	
/*	*popcy = z80Page;	*/
	
	if (!new_machine(mp)) {
		opcycles = OPCY_ERR;
		qerr();
		return;
	}

	if (opcycles == OPCY_NONE) {
		*popcy = (mchtyp & I_Z280) ? z280Page : (mchtyp & I_Z180) ? hd64Page : z80Page ;

		opcycles = (*popcy)[0][cb[0] & 0xFF];
		while ((opcycles & OPCY_NONE) && (opcycles & OPCY_MASK)) {
			switch (opcycles) {
			case P2:	/* CB xx	*/
			case P3:	/* DD xx	*/
			case P4:	/* ED xx	*/
			case P5:	/* FD xx	*/
				opcycles = (*popcy)[opcycles & OPCY_MASK][cb[1] & 0xFF];
				break;
			case P6:	/* DD CB -- xx	*/
			case P7:	/* FD CB -- xx	*/
				opcycles = (*popcy)[opcycles & OPCY_MASK][cb[3] & 0xFF];
				break;
			case P8:	/* DD ED xx	*/
			case P9:	/* FD ED xx	*/
				opcycles = (*popcy)[opcycles & OPCY_MASK][cb[2] & 0xFF];
				break;
			default:
				opcycles = OPCY_NONE;
				break;
			}
		}
	}
}



static
struct expr arg1, arg2;
static
a_uint   marg1, marg2;
	

/*
 *  new_machine()
 *
 * Assembly is done using the entirely new algorithms for the Z280 architecture.
 * If assembly is successful, then 1 is returned.
 * If assmebly is unsuccessful, then 0 is returned to default to the older
 * assembly method.
 *
 */

int new_machine(mp)
struct mne *mp;
{
	int instr, narg;
	struct a_table *at;
/*	char *ipsav;		not used ARB/JRC */
	int i;
	
	instr = (unsigned char)mp->m_type;

#if DEBUG
	PRINTF("\n%d  instr %d (%s)   mchtyp %d    AND %d\n", (int)pass, (int)instr, mp->m_id,
				(int)mchtyp,  (int)(mchtyp & I_Z180) );
	PRINTF("%s\n", ip);
#endif

	
/* first clear out the arguments and argument masks */
/*	ipsav = ip;	ARB/JRC */ /* save the character pointer within the line */
	clrexpr(&arg1);
	clrexpr(&arg2);
	marg1 = marg2 = narg = 0;
	
	if (!more()) {
/*
 * These instructions must all be valid in Z80 mode (i.e., valid on all processors
 */
		if (instr >= BYTE1 && instr < BYTE2) {
			if (instr > BYTE1 && !(mchtyp & I_Z80p))   return 0;	/* privileged on the Z280 */
			outab((int)mp->m_valu);
			return 1;
		}
		else if (instr >= BYTE2  &&  instr <= BYTE180_2) {
			if ( (instr > BYTE2 && instr < BYTE180_2 && !(mchtyp & I_Z80io) ) ||
			 				(instr == BYTE180_2 && !(mchtyp & I_Z180)) ) {   return 0; }
			outab(0xED);
			outab((int)mp->m_valu);
			return 1;
		}
		else if (instr == S_CPU) {
			opcycles = OPCY_CPU;
			sym[2].s_addr = mchtyp = mp->m_valu;
			lmode = SLIST;
			return 1;
		}
	} 
	else {		/* some parameter(s) on the line */
		if (instr == S_DEBUG) {
#if DEBUG
			marg1 = new_addr(&arg1);
			if (!is_abs(&arg1)) rerr();
			else {
				debug = arg1.e_addr & 7;
			}
#endif
			return 1;
		}
		if (instr >= CALL && instr <= RET) {		/* does the instruction take a conditional? */
			i = (int) admode(CONDITIONAL);		/* ARB */
			if (i) {
				i &= 7;
				if (instr == JR && i > 3) qerr();
				marg1 = MM(nCOND) | MM(i);
			}
		}

		if (!marg1) {
			marg1 = new_addr(&arg1);
		}

		if (instr >= BIT && instr <= RST) {		/* does the instruction require an absolute argument */
			if (!is_abs(&arg1)) rerr();
			i = (int) arg1.e_addr;		/* ARB */
			if (i<0)  rerr();
			if (instr <= RES && i>7)  aerr();
			else if (instr == RST) {
				if (i & ~0x38u) aerr();
				i >>= 3;
			}
			else if (instr == IM) {
				static char tran[4] = { 0, 2, 3, 1 };
				if (i > ( (mchtyp & I_Z280p) ? 3 : 2 ) ) qerr();
				i = tran[i];
			}
			marg1 = MM(nCOND) | MM(i);
		}
		narg++;
		if (comma(0)) {		/* if there is a comma, get the second argument */
			marg2 = new_addr(&arg2);
			narg++;
		}
		if (more()) {	/* there should be no more input after the second argument */
/*			ip = ipsav;	*/	/* ARB */
/*			qerr();		*/	/* ARB */ /* Note: qerr() never returns ==>> uses longjmp() internally */
			return 0;		/* ARB */ /* Let machine() process error */
		}
	}
/* search for a match within the table */
	at = atable;
#if DEBUG
	if (pass>=0) {
	    PRINTF("marg1=%08X  marg2=%08X\n", marg1, marg2);
	}
#endif
	while (at->imne) {
		if (at->imne == instr  &&  (mchtyp & at->cpulvl) &&
			( (!(at->arg1) && !(marg1)) || arg_match(at->arg1, marg1) ) &&
			( (!(at->arg2) && !(marg2)) || arg_match(at->arg2, marg2) ) )  {
#if DEBUG
			PRINTF("match:   patt %08X arg %08X   patt %08X arg %08X\n",
			at->arg1, marg1, at->arg2, marg2);
			
#endif
			for (i=0; i<4; i++) {
				if (at->ops[i])
					emit(at->ops[i]);
			}
			return 1;
		}
		at++;
	}
	
/*	ip = ipsav;	*/	/* ARB */
	return 0;		/* ARB */ /* Let machine() process error */
}







#define DONE 0x100
int generate(arg, marg, op, byt)
struct expr *arg;
a_uint marg;
INT32 op;	/* ARB */
int byt;
{
	int tmp;
	
	op &= ARG_MASK;			/* mask bits */
	switch (op) {
	
	case nREL8:
	case nREL16:
		if (mchpcr(arg)) {
			tmp = (int)(arg->e_addr - dot.s_addr);
			if (op == nREL16) {
/* byt is normally zero; but may be set to allow for extra bytes between displacement and end of instruction */
				tmp -= (byt + 2);
				outaw(tmp);
			} else {
				tmp -= (byt + 1);
				if (tmp > 127 || tmp < -128)  aerr();
				outab(tmp);
			} 
		} 
		else if (op == nREL16) {
			arg->e_addr -= byt /* (byt + 2) */;
			outrw(arg, R_PCR);
		}
		else {
			arg->e_addr -= byt /* (byt + 1) */;
			outrb(arg, R_PCR);
		}
		
		byt |= DONE;
		break;
	case PHI:
		if (marg & (REG_IX & REGmask)) byt = 0xDD;
		else if (marg & (REG_IY & REGmask)) byt = 0xFD;
		else byt |= DONE;
		break;
	case IMMED8:
/*	case DISP8:	*/
		outrb(arg, 0);
		byt |= DONE;
		break;
/*	case DISP16:	*/
	case IMMED16:
/*	case gADDRESS:	*/
		outrw(arg, 0);
		byt |= DONE;
		break;
	case REG8s:
		byt |= (nreg(marg) & 7);
		break;
	case REG8d:
		byt |= (nreg(marg) & 7) << 3;
		break;
     /* case CONDITIONAL: */
	case REG16m:
		tmp = nreg(marg);
		byt |= (tmp & 7) << 3;  /* low order octal bit is set in the template */	
		break;
	case RXXa:
		marg &= REGmask;
		if (marg & REG_IX) byt |= 1;
		else if (marg & REG_IY) byt |= 2;
		else if (marg & REG_HL) byt |= 3;
		break;
	case RXXb:
		marg &= REGmask;
		if (marg & REG_IX) byt |= (1<<3);
		else if (marg & REG_IY) byt |= (2<<3);
		else if (marg & REG_HL) byt |= (3<<3);
		break;
	case nRBX:
		marg &= REGmask;	/* clear out all but the Register bits */
		if ((marg & REG_IX)) byt |= 1;
		if ((marg & REG_IY)) byt |= 2;
		break;
	case nRBXd:
		marg &= REGmask;	/* clear out all but the Register bits */
		if ((marg & REG_IX)) byt |= (1<<3);
		if ((marg & REG_IY)) byt |= (2<<3);
		break;
	case nREGXY:
		marg &= REGmask;
		if ((marg & REG_IY)) byt |= (2<<3);
	/*	if ((marg & REG_IX)) byt |= (0<<3);	*/
		break;
	default:
		break;
	}
	return byt;
}





VOID
emit(op)
INT32 op;
{
	int byt;
	
	byt = ((int) op) & 0xFF;	/* ARB */
	byt = generate(&arg1, marg1, op>>A1, byt);
	byt = generate(&arg2, marg2, op>>A2, byt);

	if ((byt & DONE) == 0) outab(byt);
}

#undef DONE



int
arg_match(class, arg)
a_uint class, arg;
{
	a_uint am_class, am_arg;
	int okay;
	
	am_class = class & ADRMODEmask;
	am_arg = arg & ADRMODEmask;
	
	okay = am_class ? IN(am_class, am_arg) : (am_arg == 0) ;
	okay &= IN(class, arg);
	
	return okay;
}



/* nreg(a_uint mask)
 *
 * returns the register number or condition of the lowest order bit set
 * returns -1 if no bits set (error condition)
 */
int nreg(mask)
a_uint mask;
{
	int n;
	
	if (mask) {
		n = 0;
		do {
			if (mask & 1) return n;
			n++;
			mask >>= 1;
		} while (1);
	}
	return -1;
}





/*
 * Branch/Jump PCR Mode Check
 */
int
mchpcr(esp)
struct expr *esp;
{
	if (esp->e_base.e_ap == dot.s_area) {
		return(1);
	}
	if (esp->e_flag==0 && esp->e_base.e_ap==NULL) {
		/*
		 * Absolute Destination
		 *
		 * Use the global symbol '.__.ABS.'
		 * of value zero and force the assembler
		 * to use this absolute constant as the
		 * base value for the relocation.
		 */
		esp->e_flag = 1;
		esp->e_base.e_sp = &sym[1];
	}
	return(0);
}



/*
 * Machine dependent initialization
 */
VOID
minit()
{
	/*
	 * Byte Order
	 */
	hilo = 0;
	debug = 0;

	if (pass == 0) {
		mchtyp = M_Z80;
		sym[2].s_addr = M_Z80;
#if DEBUG
	PRINTF("A1/A2  %d/%d   %d Commands\n", A1, A2, nnCMD_FINAL);
#endif
	}

}


