/* z280adr.c */

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


#if DEBUG

VOID dump_expr(ep)
struct expr *ep;
{
	PRINTF("EXPR:  mode %d   flag %d   addr %d %08X   rlcf %02X   base_ptr %08lX\n", 
	(int)(ep->e_mode), (int)(ep->e_flag), ep->e_addr, ep->e_addr,
	(int)((unsigned char)(ep->e_rlcf)), (long int)(ep->e_base.e_ap) );
}




static char *iname[32] = {
	"B", "C", "D", "E", "H", "L", "SP", "A",
	"IXH", "IXL", "PC", "I", "IYH", "IYL", "AF", "AF'", "R", "USP",
	"8", "16", "32",
	"IM", "IR", "DA", "LX", "SX", "RA", "SR", "BX", "COND",
	"-30-", "-31-"
};


VOID list_indx(indx)
INT32 indx;
{
	unsigned INT32 mask;
	int i;
	
	mask = 0x80000000uL;
	printf("  ");
	for (i=31; i>=18; i--) {
		if (mask&indx) printf("%s", iname[i]);
		mask >>= 1;
		if (i==21) printf(" ");
	}
	printf("  ");
	for (i=0, mask=1; i<18; i++) {
		if (mask&indx) printf("%s", iname[i]);
		mask <<= 1;
	}
	printf("\n");
}

#endif




INT32
new_addr(esp)	/* from addr */
struct expr *esp;
{
	int c;
	INT32 mode;	/* ARB */
	INT32 indx;
	int close;

	mode = indx = 0;
	if ((c = getnb()) == '#') {
		expr(esp, 0);
		esp->e_mode = S_IMMED;
		indx = A_IM;
	} else
	if (c == LFIND || c == LFBKT) {
		close = (c==LFIND) ? RTIND : RTBKT ;	/* close is set to the closing ) or ]  */
		if ( (indx = admode(TOKEN)) != 0 ) {
			if (IN(R_R8, indx))  mode = S_INDB;
			if ((mchtyp & I_Z280) && IN(R_R16BX, indx)) {
				int indx2;
				char *ips;
				
				ips = ip;
				if (getnb() == '+' && (indx2 = (int) admode(TOKEN)) != 0 &&	/* ARB */
							IN(R_R16BX, indx2) && indx != indx2 &&
							(c = getnb()) == RTIND)  {

					indx |= (indx2 | M_BX);
					mode = S_BX;
					unget(c);
				}
				else ip = ips;
			}
			else if (IN(R_R16|R_R16X, indx))  mode = S_INDR;
			else if (IN(REG_AF|REG_AFP, indx))  { mode = S_R16X; aerr(); }
			else if (IN(REG_I|REG_R, indx))  { mode = S_R8X; aerr(); }
			else if (IN(REG_PC, indx))  mode = S_RA;
			if (mode != S_BX) {
				if ( (c = getnb()) == close) {
					indx |= /* IN(REG_IX|REG_IY, indx) ? M_SX :*/ M_IR;
				}
				unget(c);
			}
		/* indx is known to be set */
#if DEBUG
	PRINTF("new_adr1:  mode %d   indx %08X\n", mode, indx);
#endif
			if (mode == S_INDR) mode += nreg(indx)/2;
			esp->e_mode = mode;
		/*	esp->e_base.e_ap = NULL;  */
#if DEBUG
	PRINTF("new_adr2:  mode %d   indx %08X\n", esp->e_mode, indx);
#endif
		}
		else {
			if (close == RTIND) {
				mode = S_INDM;
				indx = A_DA;
			} else {
				mode = S_RA;
				indx = A_RA;
			}
			expr(esp, 0);
			esp->e_mode = mode;
		}
#if DEBUG
	PRINTF("new_adr3:  mode %d   indx %08X\n", esp->e_mode, indx);
	dump_expr(esp);
#endif

		if ( (c = getnb()) != close ) {
			unget(c);
			if (IN((REG_IX|REG_IY|REG_HL|REG_SP|REG_PC), indx)) {
				expr(esp, 0);
				esp->e_mode = S_INDR + nreg(indx)/2;
				switch (indx) {
				  case REG_HL:
				  	indx |= M_LX;
				  	break;
				  case REG_SP:
				  	indx |= M_SR;
				  	break;
				  case REG_IX:
				  case REG_IY:
#if 0
				  	if (esp->e_base.e_ap || (int)(esp->e_addr) < -128 || (int)esp->e_addr > 127 )
				  		indx |= M_LX;
				  	else  indx |= M_SX;
#else
				/* Let us try this again 			JRC */
					if (esp->e_base.e_ap)
						indx |= (mchtyp & I_Z280) ? M_LX : M_SX;
					else
					if ( (int)(esp->e_addr) < -128 || (int)(esp->e_addr) > 127 )
						indx |= M_LX;
					else
						indx |= M_SX;
#endif
				  	break;
				  case REG_PC:
				  	indx |= M_RA;
				  	break;
				}  
			}
			if ( (c = getnb()) != close )  qerr();
		}
			

	} else {	/* no parens or bracket */
		unget(c);
		if ( (indx = admode(TOKEN)) != 0) {
			if (IN(R_R8, indx))  mode = S_R8;
			else if (IN(R_R16|R_R16X, indx))  mode = S_R16;
			else if (IN(REG_AF|REG_AFP, indx))  mode = S_R16X;
			else if (IN(REG_I|REG_R, indx))  mode = S_R8X;
			else if (IN(REG_PC, indx))  mode = S_RA;
		/* indx is known to be set */
			esp->e_addr = nreg(indx);
			esp->e_mode = mode;
		/*	esp->e_base.e_ap = NULL;	*/
		} else {
			mode = S_USER;
			expr(esp, 0);
			esp->e_mode = mode;
#if 0			
			indx = is_abs(esp) ? A_IM : A_DA ;
#else
			indx = A_IM;
#endif
		}
		if ((c = getnb()) == LFIND) {
			if ((indx = admode(TOKEN)) != 0  &&  IN((REG_IX|REG_IY|REG_HL|REG_SP|REG_PC), indx) ) {
				esp->e_mode = S_INDR + nreg(indx)/2;
			/*	esp->e_base.e_ap = NULL;		*/
				switch (indx) {
				  case REG_HL:
				  	indx |= M_LX;
				  	break;
				  case REG_SP:
				  	indx |= M_SR;
				  	break;
				  case REG_IX:
				  case REG_IY:
#if 0
				  	if (esp->e_base.e_ap || (int)(esp->e_addr) < -128 || (int)esp->e_addr > 127 )
				  		indx |= M_LX;
				  	else  indx |= M_SX;
#else
				/* Let us try this again 			JRC */
					if (esp->e_base.e_ap)
						indx |= (mchtyp & I_Z280) != 0 ? M_LX : M_SX;
					else
					if ( (int)(esp->e_addr) < -128 || (int)(esp->e_addr) > 127 )
						indx |= M_LX;
					else
						indx |= M_SX;
#endif
				  	break;
				  case REG_PC:
				  	indx |= M_RA;
				  	break;
				}  
			}
			else aerr();

			if ((c = getnb()) != RTIND)
				qerr();
		} else {
			unget(c);
		}
	}
/*	return (esp->e_mode);	*/
#if DEBUG
	PRINTF("new_addr9: ");
	if (pass>=DEBUG && debug > 0) list_indx(indx);
	dump_expr(esp);
#endif
	return indx;
}



/*
 * Enter admode() to search a specific addressing mode table
 * for a match. Return the addressing value on a match or
 * zero for no match.
 */
INT32
admode(sp)
struct adsym *sp;
{
	char *ptr;
	int i;
	char *ips;

	ips = ip;
	unget(getnb());

	i = 0;
	while ( *(ptr = &sp[i].a_str[0]) ) {
		if (srch(ptr)) {
			return(sp[i].a_val);
		}
		i++;
	}
	ip = ips;
	return(0);
}



/*
 *      srch --- does string match ?
 */
int
srch(str)
char *str;
{
	char *ptr;
	ptr = ip;

	while (*ptr && *str) {
		if (ccase[*ptr & 0x007F] != ccase[*str & 0x007F])
			break;
		ptr++;
		str++;
	}
	if (ccase[*ptr & 0x007F] == ccase[*str & 0x007F]) {
		ip = ptr;
		return(1);
	}

	if (!*str)
		if (!(ctype[*ptr & 0x007F] & LTR16)) {
			ip = ptr;
			return(1);
		}
	return(0);
}

/*
 * Registers
 */

struct	adsym	TOKEN[] = {
    {	"hl",	REG_HL	},
    {	"de",	REG_DE	},
    {	"bc",	REG_BC	},
    {	"sp",	REG_SP	},
    {	"pc",	REG_PC	},
    {	"af'",	REG_AFP	},
    {	"af",	REG_AF	},
    {	"a",	REG_A	},
    {	"b",	REG_B	},
    {	"c",	REG_C	},
    {	"d",	REG_D	},
    {	"e",	REG_E	},
    {	"h",	REG_H	},
    {	"l",	REG_L	},
    {	"ixh",	REG_IXH	},
    {	"ixl",	REG_IXL	},
    {	"iyh",	REG_IYH	},
    {	"iyl",	REG_IYL	},
    {	"ix",	REG_IX	},
    {	"iy",	REG_IY	},
    {	"i",	REG_I	},
    {	"r",	REG_R	},
    {	"usp",	REG_USP	},
    {	"dehl",	REG_DEHL },
    {	"",	0	}
};


struct	adsym	CONDITIONAL[] = {
    {	"nz",	0400	},	/* will mask to Zero */
    {	"z",	1	},
    {	"nc",	2	},
    {	"c",	3	},
    {	"po",	4	},
    {	"nv",	4	},
    {	"pe",	5	},
    {	"v",	5	},
    {	"ns",	6	},
    {	"p",	6	},
    {	"s",	7	},
    {	"m",	7	},
    {	"",	0	}
};



