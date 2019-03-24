/* z280opcy.h */

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



/*
 * Z280  Opcode Cycle Pages
 */

static char  z280pg1[256] = {
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   2, 2, 8, 3, 3, 3, 2, 2, 2, 3, 7, 3, 3, 3, 2, 2,
/*10*/   7, 2, 8, 3, 3, 3, 2, 2, 4, 3, 7, 3, 3, 3, 2, 2,
/*20*/   4, 2, 8, 3, 3, 3, 2, 3, 4, 3, 7, 3, 3, 3, 2, 2,
/*30*/   4, 2, 8, 3,13,13, 8, 2, 4, 3, 7, 3, 3, 3, 2, 2,
/*40*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*50*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*60*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*70*/   8, 8, 8, 8, 8, 8,16, 8, 2, 2, 2, 2, 2, 2, 7, 2,
/*80*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*90*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*A0*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*B0*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*C0*/  13,13, 4, 4,16,13, 2,14,13,13, 4,P2,16,16, 2,14,
/*D0*/  13,13, 4,10,16,13, 2,14,13, 2, 4,14,16,P3, 2,14,
/*E0*/  13,13, 4,14,16,13, 2,14,13, 4, 4, 5,16,P4, 2,14,
/*F0*/  13,13, 4, 8,16,13, 2,14,13, 2, 4, 8,16,P5, 2,14
};

static char  z280pg2[256] = {  /* P2 == CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*10*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*20*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*30*/   3, 3, 3, 3, 3, 3,10, 3, 2, 2, 2, 2, 2, 2,13, 2,
/*40*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*50*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*60*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*70*/   2, 2, 2, 2, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 7, 2,
/*80*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*90*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*A0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*B0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2, 
/*C0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*D0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*E0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2,
/*F0*/   2, 2, 2, 2, 2, 2,13, 2, 2, 2, 2, 2, 2, 2,13, 2
};

static char  z280pg3[256] = {  /* P3 == DD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN, 8,UN,13,14,14, 9,UN,UN, 3,UN,13,15,15,10,UN,
/*10*/  UN, 8,UN,13,15,15,10,UN,UN, 3,UN,13,15,15,10,UN,
/*20*/   4, 2, 8, 3, 3, 3, 2,UN, 4, 3, 7, 3, 3, 3, 2,UN,
/*30*/  UN, 9,UN,14,14,14, 9,UN,UN, 3,UN,14,13,13, 8,UN,
/*40*/  UN,UN,UN,UN, 2, 2, 8,UN,UN,UN,UN,UN, 2, 2, 8,UN,
/*50*/  UN,UN,UN,UN, 2, 2, 8,UN,UN,UN,UN,UN, 2, 2, 8,UN,
/*60*/   2, 2, 2, 2, 2, 2, 8, 2, 2, 2, 2, 2, 2, 2, 8, 2,
/*70*/   9, 9, 9, 9, 9, 9,UN, 9, 8, 9, 9, 9, 2, 2, 8,UN,
/*80*/   8, 9, 9, 9, 2, 2, 8, 7, 8, 9, 9, 9, 2, 2, 8, 7,
/*90*/   8, 9, 9, 9, 2, 2, 8, 7, 8, 9, 9, 9, 2, 2, 8, 7,
/*A0*/   8, 9, 9, 9, 2, 2, 8, 7, 8, 9, 9, 9, 2, 2, 8, 7,
/*B0*/   8, 9, 9, 9, 2, 2, 8, 7, 8, 9, 9, 9, 2, 2, 8, 7,
/*C0*/  UN,18, 4,UN,16,18,UN,UN,UN,UN, 4,P6,16,16,UN,UN,
/*D0*/  UN,18, 4,UN,16,18,UN,UN,UN,UN, 4,UN,16,UN,UN,UN,
/*E0*/  UN,13, 4,14,16,13,UN,UN,UN, 4, 4, 5,16,P8,UN,UN,
/*F0*/  UN,19, 4,UN,16,19,UN,UN,UN, 2, 4,UN,16,UN,UN,UN
};

static char  z280pg4[256] = {  /* P4 == ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN, 5, 9, 8, 8, 7, 4,UN,UN, 6,10, 9,10, 8, 4,
/*10*/  UN,UN, 6,10, 9, 9, 7, 4,UN,UN, 6,10, 9,10, 8, 4,
/*20*/  UN,UN, 5, 9, 8, 8, 7, 4,UN,UN, 5, 9, 8, 9, 8, 4,
/*30*/  UN,UN, 5, 9, 8, 9, 7,14,UN,UN, 5, 9, 8, 9, 8, 4,
/*40*/  12, 8, 3, 8, 3, 9, 3, 2,12, 8, 3, 7, 3,12, 3, 2,
/*50*/  12, 8, 3, 8,UN,35, 3, 2,12, 8, 3, 7,UN,UN, 3, 2,
/*60*/  12, 8, 3,UN, 4, 2, 8,14,12, 8, 3,UN, 4, 3, 9,14,
/*70*/  12,64, 3, 8,UN,UN,UN, 8,12, 8, 3, 8,UN,UN,UN, 8,
/*80*/  UN,UN,22,17,50,50, 7, 2,UN,UN,22,17,51,51, 8, 2,
/*90*/  UN,UN,22,17,51,51, 7,15,UN,UN,22,17,51,51, 8, 4,
/*A0*/  18,12,22,17,50,50,49,49,18,12,22,17,50,50,49,49,
/*B0*/  18,12,22,17,50,50,UN,12,18,12,22,17,50,50,UN, 8,
/*C0*/  18,17,25,24,46,34, 3, 3,18,17,63,51,46,34, 3,UN,
/*D0*/  18,17,25,24,46,34, 3, 3,18,17,63,51,46,34, 3,UN,
/*E0*/  18,17,25,24,46,34, 3, 3,18,17,63,51,46,34, 3, 4,
/*F0*/  23,22,25,24,51,39, 3, 3,18,17,63,51,46,34, 3,UN
};

static char  z280pg5[256] = {  /* P5 == FD */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,14,14,14, 9,UN,UN, 3,UN,14,14,14, 9,UN,
/*10*/  UN,UN,UN,14,14,14, 9,UN,UN, 3,UN,14,14,14, 9,UN,
/*20*/  UN, 2, 8, 3, 3, 3, 2,UN,UN, 3, 7, 3, 3, 3, 2,UN,
/*30*/  UN,UN,UN,UN,14,14, 9,UN,UN, 3,UN,UN,UN,UN,UN,UN,
/*40*/  UN,UN,UN,UN, 2, 2, 8,UN,UN,UN,UN,UN, 2, 2, 8,UN,
/*50*/  UN,UN,UN,UN, 2, 2, 8,UN,UN,UN,UN,UN, 2, 2, 8,UN,
/*60*/   2, 2, 2, 2, 2, 2, 8, 2, 2, 2, 2, 2, 2, 2, 8, 2,
/*70*/   9, 9, 9, 9, 9, 9,UN, 9, 8, 8, 8, 8, 2, 2, 8,UN,
/*80*/   8, 8, 8, 8, 2, 2, 8,UN, 8, 8, 8, 8, 2, 2, 8,UN,
/*90*/   8, 8, 8, 8, 2, 2, 8,UN, 8, 8, 8, 8, 2, 2, 8,UN,
/*A0*/   8, 8, 8, 8, 2, 2, 8,UN, 8, 8, 8, 8, 2, 2, 8,UN,
/*B0*/   8, 8, 8, 8, 2, 2, 8,UN, 8, 8, 8, 8, 2, 2, 8,UN,
/*C0*/  UN,UN, 5, 5,17,UN,UN,UN,UN,UN, 5,P7,17,17,UN,UN,
/*D0*/  UN,UN, 5,UN,17,UN,UN,UN,UN,UN, 5,UN,17,UN,UN,UN,
/*E0*/  UN,13, 5,14,17,13,UN,UN,UN, 4, 5, 5,17,P9,UN,UN,
/*F0*/  UN,UN, 5,UN,17,13,UN,UN,UN, 2, 5,UN,17,UN,UN,UN
};

static char  z280pg6[256] = {  /* P6 == DD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*10*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*20*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*30*/  UN,UN,UN,UN,UN,UN,11,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*40*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*50*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*60*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*70*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*80*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*90*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN
};

static char  z280pg7[256] = {  /* P7 == FD CB */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*10*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*20*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*30*/  UN,UN,UN,UN,UN,UN,11,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*40*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*50*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*60*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*70*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*80*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*90*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*C0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*D0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*E0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN,
/*F0*/  UN,UN,UN,UN,UN,UN,14,UN,UN,UN,UN,UN,UN,UN,14,UN
};

static char  z280pg8[256] = {  /* P8 == DD ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN, 5,UN, 8, 9, 8,15,UN,UN, 6,UN, 9,10, 9,16,
/*10*/  UN,UN, 6,UN, 9,10, 8,16,UN,UN, 6,UN, 9,10, 9,16,
/*20*/  UN,UN, 5,UN, 8, 9, 8, 4,UN,UN, 5,UN, 8, 9, 9, 4,
/*30*/  UN,UN, 5,UN, 8, 9, 8,15,UN,UN, 5,UN, 8, 9, 9,14,
/*40*/  19,15, 3,UN,UN,UN,UN,UN,20,16, 3,UN,UN,UN,UN,UN,
/*50*/  19,15, 3,UN,UN,UN,UN,UN,20,16, 3,UN,UN,UN,UN,UN,
/*60*/  12, 8, 3,UN,UN,UN, 8,UN,12, 8, 3,UN,UN, 3, 9,UN,
/*70*/  UN,UN, 3,UN,UN,UN,UN,UN,18,14, 3,UN,UN,UN,UN,UN,
/*80*/  UN,UN,UN,UN,UN,UN, 8, 2,UN,UN,UN,UN,UN,UN, 9, 2,
/*90*/  UN,UN,UN,UN,UN,UN, 8,UN,UN,UN,UN,UN,UN,UN, 9,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*C0*/  24,23,30,29,52,40, 7, 7,25,24,68,56,51,39, 7,UN,
/*D0*/  25,24,30,29,53,41, 7, 7,25,24,68,56,51,39, 7,UN,
/*E0*/  18,17,25,24,46,34, 3, 3,18,17,63,51,46,34, 3,UN,
/*F0*/  24,23,31,30,52,40, 8, 8,23,22,69,57,49,37, 8,UN
};

static char  z280pg9[256] = {  /* P9 == FD ED */
/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
/*00*/  UN,UN, 5,UN, 8, 9, 8,15,UN,UN, 6,UN, 9,10, 9,15,
/*10*/  UN,UN, 6,UN, 9,10, 8,15,UN,UN, 6,UN, 9,10, 9,15,
/*20*/  UN,UN, 5,UN, 8, 9, 8, 4,UN,UN, 5,UN, 8, 9, 9, 4,
/*30*/  UN,UN, 5,UN, 8, 9, 8,15,UN,UN, 5,UN, 8, 9, 9,UN,
/*40*/  19,15, 3,UN,UN,UN,UN,UN,19,15, 3,UN,UN,UN,UN,UN,
/*50*/  19,15, 3,UN,UN,UN,UN,UN,19,15, 3,UN,UN,UN,UN,UN,
/*60*/  12, 8, 3,UN,UN,UN, 8,UN,12, 8, 3,UN,UN, 3, 9,UN,
/*70*/  UN,UN, 3,UN,UN,UN,UN,UN,UN,UN, 3,UN,UN,UN,UN,UN,
/*80*/  UN,UN,UN,UN,UN,UN, 8, 2,UN,UN,UN,UN,UN,UN, 9, 2,
/*90*/  UN,UN,UN,UN,UN,UN, 8,UN,UN,UN,UN,UN,UN,UN, 9,UN,
/*A0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*B0*/  UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,UN,
/*C0*/  24,23,31,30,52,40, 8, 8,24,23,69,57,51,39, 8,UN,
/*D0*/  24,23,31,30,52,40, 8, 8,24,23,69,57,51,39, 8,UN,
/*E0*/  18,17,25,24,46,34, 3, 3,18,17,63,51,46,34, 3,UN,
/*F0*/  24,23,25,24,52,40, 3, 3,18,17,63,51,46,34, 3,UN
};

static char *z280Page[9] = {
    z280pg1, z280pg2, z280pg3, z280pg4,
    z280pg5, z280pg6, z280pg7, z280pg8,
    z280pg9
};


/* end z280opcy.h */
