/* asxscn.c */

/*
 * (C) Copyright 1989-2000
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */


#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#ifdef WIN32
#include <stdlib.h>
#else
#include <alloc.h>
#endif

#include "asxxxx.h"



static int inpfil;		/* Input File Counter	*/
static int radix;		/* Radix Value		*/
static int a_bytes;		/* Address Bytes	*/
static int vlines;		/* Valid Lines Scanned	*/
static int aserr;		/* Error Counter	*/

static FILE *sfp[MAXFIL];	/* Input File Handle	*/

static char scfile[80];		/* Input File Name	*/

static char scline[256];	/* Input text line	*/


/*
 *	array of character types, one per
 *	ASCII character
 */
char	ctype[128] = {
/*NUL*/	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,
/*BS*/	ILL,	SPACE,	ILL,	ILL,	SPACE,	ILL,	ILL,	ILL,
/*DLE*/	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,
/*CAN*/	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,	ILL,
/*SPC*/	SPACE,	ETC,	ETC,	ETC,	LETTER,	BINOP,	BINOP,	ETC,
/*(*/	ETC,	ETC,	BINOP,	BINOP,	ETC,	BINOP,	LETTER,	BINOP,
/*0*/	DGT2,	DGT2,	DGT8,	DGT8,	DGT8,	DGT8,	DGT8,	DGT8,
/*8*/	DGT10,	DGT10,	ETC,	ETC,	BINOP,	ETC,	BINOP,	ETC,
/*@*/	ETC,	LTR16,	LTR16,	LTR16,	LTR16,	LTR16,	LTR16,	LETTER,
/*H*/	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,
/*P*/	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,
/*X*/	LETTER,	LETTER,	LETTER,	ETC,	ETC,	ETC,	BINOP,	LETTER,
/*`*/	ETC,	LTR16,	LTR16,	LTR16,	LTR16,	LTR16,	LTR16,	LETTER,
/*h*/	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,
/*p*/	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,	LETTER,
/*x*/	LETTER,	LETTER,	LETTER,	ETC,	BINOP,	ETC,	ETC,	ETC
};

/*
 *	an array of characters which
 *	perform the case translation function
 */
char	ccase[128] = {
/*NUL*/	'\000',	'\001',	'\002',	'\003',	'\004',	'\005',	'\006',	'\007',
/*BS*/	'\010',	'\011',	'\012',	'\013',	'\014',	'\015',	'\016',	'\017',
/*DLE*/	'\020',	'\021',	'\022',	'\023',	'\024',	'\025',	'\026',	'\027',
/*CAN*/	'\030',	'\031',	'\032',	'\033',	'\034',	'\035',	'\036',	'\037',
/*SPC*/	'\040',	'\041',	'\042',	'\043',	'\044',	'\045',	'\046',	'\047',
/*(*/	'\050',	'\051',	'\052',	'\053',	'\054',	'\055',	'\056',	'\057',
/*0*/	'\060',	'\061',	'\062',	'\063',	'\064',	'\065',	'\066',	'\067',
/*8*/	'\070',	'\071',	'\072',	'\073',	'\074',	'\075',	'\076',	'\077',
/*@*/	'\100',	'\141',	'\142',	'\143',	'\144',	'\145',	'\146',	'\147',
/*H*/	'\150',	'\151',	'\152',	'\153',	'\154',	'\155',	'\156',	'\157',
/*P*/	'\160',	'\161',	'\162',	'\163',	'\164',	'\165',	'\166',	'\167',
/*X*/	'\170',	'\171',	'\172',	'\133',	'\134',	'\135',	'\136',	'\137',
/*`*/	'\140',	'\141',	'\142',	'\143',	'\144',	'\145',	'\146',	'\147',
/*h*/	'\150',	'\151',	'\152',	'\153',	'\154',	'\155',	'\156',	'\157',
/*p*/	'\160',	'\161',	'\162',	'\163',	'\164',	'\165',	'\166',	'\167',
/*x*/	'\170',	'\171',	'\172',	'\173',	'\174',	'\175',	'\176',	'\177'
};


/*)Function	int	main(argc, argv)
 *
 *		int	argc		argument count
 *		char *	argv		array of pointers to argument strings
 */

int
main(argc, argv)
int argc;
char *argv[];
{
	register char *p, *q;
	register int c, i, n, m, r;

	/*
	 * Set Defaults
	 */
	radix = 16;
	a_bytes = 2;
	inpfil = 0;
	vlines = 0;
	aserr = 0;

	printf("\n");

	for (i=1; i<argc; ++i) {
		p = argv[i];
		if (*p == '-') {
			++p;
			while ((c = *p++) != 0)
				switch(c) {

				case 'x':
				case 'X':
					radix = 16;
					break;

				case 'd':
				case 'D':
					radix = 10;
					break;

				case 'q':
				case 'Q':
					radix = 8;
					break;

				case '2':
					a_bytes = 2;
					break;

				case '3':
					a_bytes = 3;
					break;

				case '4':
					a_bytes = 4;
					break;

				default:
					usage(ER_FATAL);
				}
		} else {
			if (++inpfil > 1) {
				fprintf(stderr, "too many input files\n\n");
				asexit(ER_FATAL);
			}
			sfp[0] = fopen(p, "r");
			if (sfp[0] != NULL) {
				strcpy(scfile, p);
			} else {
				fprintf(stderr, "File %s not found\n\n", p);
				asexit(ER_FATAL);
			}
		}
	}
	if (inpfil == 0)
		usage(ER_WARNING);

	/*
	 * Scan file to find assembler errors
	 */
loop:
	while (fgets(scline, sizeof(scline), sfp[0])) {
		scline[strlen(scline)-1] = '\0';
		p = scline;

		/* The Output Formats
		| Tabs- |       |       |       |       |       |
		          11111111112222222222333333333344444-----
		012345678901234567890123456789012345678901234-----
		   |    |               |     | |
		ee XXXX xx xx xx xx xx xx LLLLL *************	HEX(16)
		ee 000000 ooo ooo ooo ooo LLLLL *************	OCTAL(16)
		ee  DDDDD ddd ddd ddd ddd LLLLL *************	DECIMAL(16)
		                     XXXX
				   OOOOOO
				    DDDDD

		| Tabs- |       |       |       |       |       |
		          11111111112222222222333333333344444-----
		012345678901234567890123456789012345678901234-----
		     |       |                  |     | |
		ee    XXXXXX xx xx xx xx xx xx xx LLLLL *********	HEX(24)
		ee   OO000000 ooo ooo ooo ooo ooo LLLLL *********	OCTAL(24)
		ee   DDDDDDDD ddd ddd ddd ddd ddd LLLLL *********	DECIMAL(24)
		                           XXXXXX
					 OOOOOOOO
					 DDDDDDDD

		| Tabs- |       |       |       |       |       |
		          11111111112222222222333333333344444-----
		012345678901234567890123456789012345678901234-----
		  |          |                  |     | |
		ee  XXXXXXXX xx xx xx xx xx xx xx LLLLL *********	HEX(32)
		eeOOOOO000000 ooo ooo ooo ooo ooo LLLLL *********	OCTAL(32)
		ee DDDDDDDDDD ddd ddd ddd ddd ddd LLLLL *********	DECIMAL(32)
		                         XXXXXXXX
				      OOOOOOOOOOO
				       DDDDDDDDDD
		*/

		/*
		 * Define File Format from [XDQ234]
		 */
		switch(radix) {
		default:
		case 16:
			r = RAD16;
			switch(a_bytes) {
			default:
			case 2: n = 3; m = 4; /* frmt = "%04X" */; break;
			case 3: n = 6; m = 6; /* frmt = "%06X" */; break;
			case 4: n = 4; m = 8; /* frmt = "%08X" */; break;
			}
			break;
		case 10:
			r = RAD10;
			switch(a_bytes) {
			default:
			case 2: n = 4; m = 5; /* frmt = "%05u" */; break;
			case 3: n = 5; m = 8; /* frmt = "%08u" */; break;
			case 4: n = 3; m = 10; /* frmt = "%010u" */; break;
			}
			break;
		case 8:
			r = RAD8;
			switch(a_bytes) {
			default:
			case 2: n = 3; m = 6; /* frmt = "%06o" */; break;
			case 3: n = 5; m = 8; /* frmt = "%08o" */; break;
			case 4: n = 2; m = 11; /* frmt = "%011o" */; break;
			}
			break;
		}

		/*
		 * Check for 'SPACE' at beginning of line
		 */
		for (i=0; i<n; i++) {
			if (*p++ != ' ')
				goto loop;
		}

		/*
		 * Require Address
		 */
		if (strncmp(&scline[n], "           ", m) == 0) {
			goto loop;
		}

		/*
		 * Address must be in proper radix
		 */
		if (!dgt(r, &scline[n], m)) {
			goto loop;
		}

		/*
		 * Position just after Address
		 */
		p += m;

		/*
		 * Data ?
		 */
		if (!dgt(r, p+1, 1)) {
			goto loop;
		}

		/*
		 * Scan until ';' is found
		 */
		q = p;
		while (*q && (*q != ';')) {
			q++;
		}
		if (*q++ != ';')
			goto loop;
		if (*q == '\0')
			goto loop;
		vlines += 1;

		/*
		 * Check Mode of Listing:
		 *	;nn nn
		 * or	; nn nn
		 */
		if (dgt(r, q, 1)) {
			p += 1;
		}

		/*
		 * Compare Data Strings
		 */
		n = strlen(q);
		for (i=0; i<n; i++) {
			if (ccase[*p++ & 0x007F] != ccase[*q++ & 0x007F]) {
				printf("%s\r\n", scline);
				aserr += 1;
				goto loop;
			}
		}
	}
	if (vlines) {
		printf("%d code error(s) found in file %s\n\n", aserr, scfile);
	} else {
		printf("?ASXSCN-E-Invalid File Format\n\n");
		aserr += 1;
	}
	asexit(aserr ? ER_ERROR : ER_NONE);
	return(0);
}

/*)Function	int	dgt(rdx,str,n)
 *
 *		int	rdx		radix bit code
 *		char	*str		pointer to the test string
 *		int	n		number of characters to check
 *
 *	The function dgt() verifies that the string under test
 *	is of the specified radix.
 *
 *	local variables:
 *		int	i		loop counter
 *
 *	global variables:
 *		ctype[]			array of character types
 *
 *	functions called:
 *		none
 *
 *	side effects:
 *		none
 */

int
dgt(rdx, str, n)
int rdx, n;
char *str;
{
	int i;

	for (i=0; i<n; i++) {
		if ((ctype[*str++ & 0x007F] & rdx) == 0) {
			return(0);
		}
	}
	return(1);
}

/*)Function	VOID	asexit(i)
 *
 *			int	i	exit code
 *
 *	The function asexit() explicitly closes all open
 *	files and then terminates the program.
 *
 *	local variables:
 *		none
 *
 *	global variables:
 *		FILE *	sfp		scan file
 *
 *	functions called:
 *		int	fclose()	c-library
 *		VOID	exit()		c-library
 *
 *	side effects:
 *		All files closed. Program terminates.
 */

VOID
asexit(i)
int i;
{
	if (sfp[0] != NULL) fclose(sfp[0]);

	exit(i);
}

char *usetxt[] = {
	"Usage: [-dqx234] file",
	"  d    decimal listing",
	"  q    octal   listing",
	"  x    hex     listing (default)",
        "  2    16-Bit  address (default)",
	"  3    24-Bit  address",
	"  4    32-Bit  address",
	"",
	NULL
};

/*)Function	VOID	usage(n)
 *
 *		int	n		exit code
 *
 *	The function usage() outputs to the stderr device the
 *	program name and version and a list of valid assembler options.
 *
 *	local variables:
 *		char **	dp		pointer to an array of
 *					text string pointers.
 *
 *	global variables:
 *		char *	usetxt[]	array of string pointers
 *
 *	functions called:
 *		VOID	asexit()	asmain.c
 *		int	fprintf()	c_library
 *
 *	side effects:
 *		program is terminated
 */

VOID
usage(n)
int n;
{
	register char   **dp;

	fprintf(stderr, "\nASxxxx Assembler Listing Scanner %s\n\n", VERSION);
	for (dp = usetxt; *dp; dp++)
		fprintf(stderr, "%s\n", *dp);
	asexit(n);
}

