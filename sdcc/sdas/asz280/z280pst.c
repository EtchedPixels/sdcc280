/* z280pst.c */

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
#ifndef S_OPTSDCC
/*
 * Coding Banks
 */
struct	bank	bank[2] = {
    /*	The '_CODE' area/bank has a NULL default file suffix.	*/
    {	NULL,		"_CSEG",	NULL,		0,	0,	0,	0,	0	},
    {	&bank[0],	"_DSEG",	"_DS",		1,	0,	0,	0,	B_FSFX	}
};

/*
 * Coding Areas
 */
struct	area	area[2] = {
    {	NULL,		&bank[0],	"_CODE",	0,	0,	0,	A_1BYTE|A_BNK|A_CSEG	},
    {	&area[0],	&bank[1],	"_DATA",	1,	0,	0,	A_1BYTE|A_BNK|A_DSEG	}
};
#endif
/*
 * Basic Relocation Mode Definition
 *
 *	#define		R_NORM	0000		No Bit Positioning
 */
char	mode0[32] = {	/* R_NORM */
	'\200',	'\201',	'\202',	'\203',	'\204',	'\205',	'\206',	'\207',
	'\210',	'\211',	'\212',	'\213',	'\214',	'\215',	'\216',	'\217',
	'\220',	'\221',	'\222',	'\223',	'\224',	'\225',	'\226',	'\227',
	'\230',	'\231',	'\232',	'\233',	'\234',	'\235',	'\236',	'\237'
};

/*
 * Additional Relocation Mode Definitions
 */

/* None Required */

/*
 *     *m_def is a pointer to the bit relocation definition.
 *	m_flag indicates that bit position swapping is required.
 *	m_dbits contains the active bit positions for the output.
 *	m_sbits contains the active bit positions for the input.
 *
 *	struct	mode
 *	{
 *		char *	m_def;		Bit Relocation Definition
 *		a_uint	m_flag;		Bit Swapping Flag
 *		a_uint	m_dbits;	Destination Bit Mask
 *		a_uint	m_sbits;	Source Bit Mask
 *	};
 */
struct	mode	mode[1] = {
    {	&mode0[0],	0,	0x0000FFFF,	0x0000FFFF	}
};

/*
 * Array of Pointers to mode Structures
 */
struct	mode	*modep[16] = {
	&mode[0],	NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL
};

/*
 * Mnemonic Structure
 */
struct	mne	mne[] = {

	/* machine */
#ifndef S_OPTSDCC
    {	NULL,	"CSEG",		S_ATYP,		0,	A_CSEG|A_1BYTE	},
    {	NULL,	"DSEG",		S_ATYP,		0,	A_DSEG|A_1BYTE	},

	/* system */

    {	NULL,	"BANK",		S_ATYP,		0,	A_BNK	},
#endif
    {	NULL,	"CON",		S_ATYP,		0,	A_CON	},
    {	NULL,	"OVR",		S_ATYP,		0,	A_OVR	},
    {	NULL,	"REL",		S_ATYP,		0,	A_REL	},
    {	NULL,	"ABS",		S_ATYP,		0,	A_ABS	},
    {	NULL,	"NOPAG",	S_ATYP,		0,	A_NOPAG	},
    {	NULL,	"PAG",		S_ATYP,		0,	A_PAG	},
#ifndef S_OPTSDCC
    {	NULL,	"BASE",		S_BTYP,		0,	B_BASE	},
    {	NULL,	"SIZE",		S_BTYP,		0,	B_SIZE	},
    {	NULL,	"FSFX",		S_BTYP,		0,	B_FSFX	},
    {	NULL,	"MAP",		S_BTYP,		0,	B_MAP	},
#endif
    {	NULL,	".page",	S_PAGE,		0,	0	},
    {	NULL,	".title",	S_HEADER,	0,	O_TITLE	},
    {	NULL,	".sbttl",	S_HEADER,	0,	O_SBTTL	},
    {	NULL,	".module",	S_MODUL,	0,	0	},
    {	NULL,	".include",	S_INCL,		0,	0	},
    {	NULL,	".area",	S_AREA,		0,	0	},
#ifndef S_OPTSDCC
    {	NULL,	".bank",	S_BANK,		0,	0	},
#endif
    {	NULL,	".org",		S_ORG,		0,	0	},
    {	NULL,	".radix",	S_RADIX,	0,	0	},
    {	NULL,	".globl",	S_GLOBL,	0,	0	},
    {	NULL,	".local",	S_LOCAL,	0,	0	},
    {	NULL,	".if",		S_CONDITIONAL,	0,	O_IF	},
    {	NULL,	".iff",		S_CONDITIONAL,	0,	O_IFF	},
    {	NULL,	".ift",		S_CONDITIONAL,	0,	O_IFT	},
    {	NULL,	".iftf",	S_CONDITIONAL,	0,	O_IFTF	},
#ifndef S_OPTSDCC
    {	NULL,	".ifdef",	S_CONDITIONAL,	0,	O_IFDEF	},
    {	NULL,	".ifndef",	S_CONDITIONAL,	0,	O_IFNDEF},
#endif
    {	NULL,	".ifgt",	S_CONDITIONAL,	0,	O_IFGT	},
    {	NULL,	".iflt",	S_CONDITIONAL,	0,	O_IFLT	},
    {	NULL,	".ifge",	S_CONDITIONAL,	0,	O_IFGE	},
    {	NULL,	".ifle",	S_CONDITIONAL,	0,	O_IFLE	},
    {	NULL,	".ifeq",	S_CONDITIONAL,	0,	O_IFEQ	},
    {	NULL,	".ifne",	S_CONDITIONAL,	0,	O_IFNE	},
#ifndef S_OPTSDCC
    {	NULL,	".ifb",		S_CONDITIONAL,	0,	O_IFB	},
    {	NULL,	".ifnb",	S_CONDITIONAL,	0,	O_IFNB	},
    {	NULL,	".ifidn",	S_CONDITIONAL,	0,	O_IFIDN	},
    {	NULL,	".ifdif",	S_CONDITIONAL,	0,	O_IFDIF	},
#endif
    {	NULL,	".iif",		S_CONDITIONAL,	0,	O_IIF	},
    {	NULL,	".iiff",	S_CONDITIONAL,	0,	O_IIFF	},
    {	NULL,	".iift",	S_CONDITIONAL,	0,	O_IIFT	},
    {	NULL,	".iiftf",	S_CONDITIONAL,	0,	O_IIFTF	},
#ifndef S_OPTSDCC
    {	NULL,	".iifdef",	S_CONDITIONAL,	0,	O_IIFDEF},
    {	NULL,	".iifndef",	S_CONDITIONAL,	0,	O_IIFNDEF},
#endif
    {	NULL,	".iifgt",	S_CONDITIONAL,	0,	O_IIFGT	},
    {	NULL,	".iiflt",	S_CONDITIONAL,	0,	O_IIFLT	},
    {	NULL,	".iifge",	S_CONDITIONAL,	0,	O_IIFGE	},
    {	NULL,	".iifle",	S_CONDITIONAL,	0,	O_IIFLE	},
    {	NULL,	".iifeq",	S_CONDITIONAL,	0,	O_IIFEQ	},
    {	NULL,	".iifne",	S_CONDITIONAL,	0,	O_IIFNE	},
#ifndef S_OPTSDCC
    {	NULL,	".iifb",	S_CONDITIONAL,	0,	O_IIFB	},
    {	NULL,	".iifnb",	S_CONDITIONAL,	0,	O_IIFNB	},
    {	NULL,	".iifidn",	S_CONDITIONAL,	0,	O_IIFIDN},
    {	NULL,	".iifdif",	S_CONDITIONAL,	0,	O_IIFDIF},
#endif
    {	NULL,	".else",	S_CONDITIONAL,	0,	O_ELSE	},
    {	NULL,	".endif",	S_CONDITIONAL,	0,	O_ENDIF	},
    {	NULL,	".list",	S_LISTING,	0,	O_LIST	},
    {	NULL,	".nlist",	S_LISTING,	0,	O_NLIST	},
    {	NULL,	".equ",		S_EQU,		0,	O_EQU	},
    {	NULL,	".gblequ",	S_EQU,		0,	O_GBLEQU},
    {	NULL,	".lclequ",	S_EQU,		0,	O_LCLEQU},
    {	NULL,	".byte",	S_DATA,		0,	O_1BYTE	},
    {	NULL,	".db",		S_DATA,		0,	O_1BYTE	},
    {	NULL,	".fcb",		S_DATA,		0,	O_1BYTE	},
    {	NULL,	".word",	S_DATA,		0,	O_2BYTE	},
    {	NULL,	".dw",		S_DATA,		0,	O_2BYTE	},
    {	NULL,	".fdb",		S_DATA,		0,	O_2BYTE	},
/*    {	NULL,	".3byte",	S_DATA,		0,	O_3BYTE	},	*/
/*    {	NULL,	".triple",	S_DATA,		0,	O_3BYTE	},	*/
/*    {	NULL,	".4byte",	S_DATA,		0,	O_4BYTE	},	*/
/*    {	NULL,	".quad",	S_DATA,		0,	O_4BYTE	},	*/
    {	NULL,	".blkb",	S_BLK,		0,	O_1BYTE	},
    {	NULL,	".ds",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".rmb",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".rs",		S_BLK,		0,	O_1BYTE	},
    {	NULL,	".blkw",	S_BLK,		0,	O_2BYTE	},
/*    {	NULL,	".blk3",	S_BLK,		0,	O_3BYTE	},	*/
/*    {	NULL,	".blk4",	S_BLK,		0,	O_4BYTE	},	*/
    {	NULL,	".ascii",	S_ASCIX,	0,	O_ASCII	},
    {	NULL,	".ascis",	S_ASCIX,	0,	O_ASCIS	},
    {	NULL,	".asciz",	S_ASCIX,	0,	O_ASCIZ	},
    {	NULL,	".str",		S_ASCIX,	0,	O_ASCII	},
    {	NULL,	".strs",	S_ASCIX,	0,	O_ASCIS	},
    {	NULL,	".strz",	S_ASCIX,	0,	O_ASCIZ	},
    {	NULL,	".fcc",		S_ASCIX,	0,	O_ASCII	},
#ifndef S_OPTSDCC
    {	NULL,	".define",	S_DEFINE,	0,	O_DEF	},
    {	NULL,	".undefine",	S_DEFINE,	0,	O_UNDEF	},
#endif
    {	NULL,	".even",	S_BOUNDARY,	0,	O_EVEN	},
    {	NULL,	".odd",		S_BOUNDARY,	0,	O_ODD	},
    {	NULL,	".bndry",	S_BOUNDARY,	0,	O_BNDRY	},
#ifndef S_OPTSDCC
    {	NULL,	".msg"	,	S_MSG,		0,	0	},
    {	NULL,	".assume",	S_ERROR,	0,	O_ASSUME},
    {	NULL,	".error",	S_ERROR,	0,	O_ERROR	},
#endif
/*    {	NULL,	".msb",		S_MSB,		0,	0	},	*/
/*    {	NULL,	".lohi",	S_MSB,		0,	O_LOHI	},	*/
/*    {	NULL,	".hilo",	S_MSB,		0,	O_HILO	},	*/
/*    {	NULL,	".8bit",	S_BITS,		0,	O_1BYTE	},	*/
/*    {	NULL,	".16bit",	S_BITS,		0,	O_2BYTE	},	*/
/*    {	NULL,	".24bit",	S_BITS,		0,	O_3BYTE	},	*/
/*    {	NULL,	".32bit",	S_BITS,		0,	O_4BYTE	},	*/
    {	NULL,	".end",		S_END,		0,	0	},
#ifdef S_OPTSDCC
/* sdas specific */
    {   NULL,   ".optsdcc",     S_OPTSDCC,      0,      0       },
/* end sdas specific */
#endif

	/* Macro Processor */

    {	NULL,	".macro",	S_MACRO,	0,	O_MACRO	},
    {	NULL,	".endm",	S_MACRO,	0,	O_ENDM	},
    {	NULL,	".mexit",	S_MACRO,	0,	O_MEXIT	},

    {	NULL,	".narg",	S_MACRO,	0,	O_NARG	},
    {	NULL,	".nchr",	S_MACRO,	0,	O_NCHR	},
    {	NULL,	".ntyp",	S_MACRO,	0,	O_NTYP	},

    {	NULL,	".irp",		S_MACRO,	0,	O_IRP	},
    {	NULL,	".irpc",	S_MACRO,	0,	O_IRPC	},
    {	NULL,	".rept",	S_MACRO,	0,	O_REPT	},

    {	NULL,	".nval",	S_MACRO,	0,	O_NVAL	},

    {	NULL,	".mdelete",	S_MACRO,	0,	O_MDEL	},

	/* Machines */

    {	NULL,	".z80",		(char)S_CPU,		0,	M_Z80	},
    {	NULL,	".z80u",	(char)S_CPU,		0,	M_Z80u	},
    {	NULL,	".hd64",	(char)S_CPU,		0,	M_Z180  },
    {	NULL,	".z180",	(char)S_CPU,		0,	M_Z180  },
    {	NULL,	".z280n",	(char)S_CPU,		0,	M_Z280n	},
    {	NULL,	".z280p",	(char)S_CPU,		0,	M_Z280p },
    {	NULL,	".z280",	(char)S_CPU,		0,	M_Z280	},
    {	NULL,	".debug",	(char)S_DEBUG,	0,	0xFFF	},

	/* z80 */

/*     hash   mnemonic          type          flag     value (INT32)  */

    {	NULL,	"lda",		(char)LDA,		0,	0	},
    {	NULL,	"ldb",		(char)LD,		0,	0	},
    {	NULL,	"ldw",		(char)LDW,		0,	0	},
    {	NULL,	"ld",		(char)LD,		0,	0	},
    {	NULL,	"ldctl",	(char)LDCTL,		0,	0	},
    {	NULL,	"ldud",		(char)LDUD,		0,	0	},
    {	NULL,	"ldup",		(char)LDUP,		0,	0	},

    {	NULL,	"call",		(char)CALL,		0,	0xC4	},
    {	NULL,	"jp",		(char)JP,		0,	0xC2	},
    {	NULL,	"jr",		(char)JR,		0,	0x18	},
    {	NULL,	"djnz",		(char)DJNZ,		0,	0x10	},
    {	NULL,	"ret",		(char)RET,		0,	0xC0	},

    {	NULL,	"bit",		(char)BIT,		0,	0x40	},
    {	NULL,	"res",		(char)RES,		0,	0x80	},
    {	NULL,	"set",		(char)SET,		0,	0xC0	},

    {	NULL,	"inc",		(char)INC,		0,	0x04	},
    {	NULL,	"incw",		(char)INCW,		0,	0	},
    {	NULL,	"dec",		(char)DEC,		0,	0x05	},
    {	NULL,	"decb",		(char)DECB,		0,	0x05	},
    {	NULL,	"decw",		(char)DECW,		0,	0x05	},

    {	NULL,	"add",		(char)ADD,		0,	0x80	},
    {	NULL,	"addw",		(char)ADDW,		0,	0	},
    {	NULL,	"adc",		(char)ADC,		0,	0	},
    {	NULL,	"sub",		(char)SUB,		0,	0x90	},
    {	NULL,	"subw",		(char)SUBW,		0,	0	},
    {	NULL,	"sbc",		(char)SBC,		0,	0x98	},

    {	NULL,	"and",		(char)AND,		0,	0xA0	},
    {	NULL,	"cp",		(char)CP,		0,	0xB8	},
    {	NULL,	"cpw",		(char)CPW,		0,	0xB8	},
    {	NULL,	"or",		(char)OR,		0,	0xB0	},
    {	NULL,	"xor",		(char)XOR,		0,	0xA8	},

    {	NULL,	"ex",		(char)EX,		0,	0xE3	},
    {	NULL,	"exts",		(char)EXTS,		0,	0	},

    {	NULL,	"push",		(char)PUSH,		0,	0xC5	},
    {	NULL,	"pop",		(char)POP,		0,	0xC1	},

    {	NULL,	"in",		(char)INN,		0,	0xDB	},	/* conflicts with IN macro (#define) */
    {	NULL,	"inw",		(char)INW,		0,	0	},
    {	NULL,	"out",		(char)OUT,		0,	0xD3	},
    {	NULL,	"outw",		(char)OUTW,		0,	0	},

    {	NULL,	"rl",		(char)RL,		0,	0x10	},
    {	NULL,	"rlc",		(char)RLC,		0,	0x00	},
    {	NULL,	"rr",		(char)RR,		0,	0x18	},
    {	NULL,	"rrc",		(char)RRC,		0,	0x08	},
    {	NULL,	"sla",		(char)SLA,		0,	0x20	},
    {	NULL,	"sra",		(char)SRA,		0,	0x28	},
    {	NULL,	"srl",		(char)SRL,		0,	0x38	},
    {	NULL,	"sll",		(char)SLL,		0,	0	},	/* Z80 undocumented */

    {	NULL,	"rst",		(char)RST,		0,	0xC7	},
    {	NULL,	"jaf",		(char)JAF,		0,	0	},
    {	NULL,	"jar",		(char)JAR,		0,	0	},
    {	NULL,	"pcache",	(char)PCACHE,		0,	0	},
    {	NULL,	"sc",		(char)SC,		0,	0	},
    {	NULL,	"tset",		(char)TSET,		0,	0	},
    {	NULL,	"tsti",		(char)TSTI,		0,	0	},

    {	NULL,	"im",		(char)IM,		0,	0xED	},
    
    {	NULL,	"divuw",	(char)DIVUW,		0,	0	},
    {	NULL,	"divu",		(char)DIVU,		0,	0	},
    {	NULL,	"divw",		(char)DIVW,		0,	0	},
    {	NULL,	"div",		(char)DIV,		0,	0	},
    {	NULL,	"multuw",	(char)MULTUW,		0,	0	},
    {	NULL,	"multu",	(char)MULTU,		0,	0	},
    {	NULL,	"multw",	(char)MULTW,		0,	0	},
    {	NULL,	"mult",		(char)MULT,		0,	0	},

    {	NULL,	"ccf",		(char)BYTE1,		0,	0x3F	},
    {	NULL,	"cpl",		(char)BYTE1,		0,	0x2F	},
    {	NULL,	"daa",		(char)BYTE1,		0,	0x27	},
    {	NULL,	"di",		(char)DI,		0,	0xF3	},
    {	NULL,	"ei",		(char)EI,		0,	0xFB	},
    {	NULL,	"exx",		(char)BYTE1,		0,	0xD9	},
    {	NULL,	"nop",		(char)BYTE1,		0,	0x00	},
    {	NULL,	"halt",		(char)HALT1,		0,	0x76	},
    {	NULL,	"rla",		(char)BYTE1,		0,	0x17	},
    {	NULL,	"rlca",		(char)BYTE1,		0,	0x07	},
    {	NULL,	"rra",		(char)BYTE1,		0,	0x1F	},
    {	NULL,	"rrca",		(char)BYTE1,		0,	0x0F	},
    {	NULL,	"scf",		(char)BYTE1,		0,	0x37	},

    {	NULL,	"cpd",		(char)BYTE2,		0,	0xA9	},
    {	NULL,	"cpdr",		(char)BYTE2,		0,	0xB9	},
    {	NULL,	"cpi",		(char)BYTE2,		0,	0xA1	},
    {	NULL,	"cpir",		(char)BYTE2,		0,	0xB1	},
    {	NULL,	"ind",		(char)IND2,		0,	0xAA	},
    {	NULL,	"indw",		(char)INDW,		0,	0	},
    {	NULL,	"indrw",	(char)INDRW,		0,	0	},
    {	NULL,	"indr",		(char)INDR2,		0,	0xBA	},
    {	NULL,	"ini",		(char)INI2,		0,	0xA2	},
    {	NULL,	"iniw",		(char)INIW,		0,	0	},
    {	NULL,	"inirw",	(char)INIRW,		0,	0	},
    {	NULL,	"inir",		(char)INIR2,		0,	0xB2	},
    {	NULL,	"ldd",		(char)BYTE2,		0,	0xA8	},
    {	NULL,	"lddr",		(char)BYTE2,		0,	0xB8	},
    {	NULL,	"ldi",		(char)BYTE2,		0,	0xA0	},
    {	NULL,	"ldir",		(char)BYTE2,		0,	0xB0	},
    {	NULL,	"neg",		(char)NEG,		0,	0x44	},
    {	NULL,	"otdr",		(char)OTDR2,		0,	0xBB	},
    {	NULL,	"otdrw",	(char)OTDRW,		0,	0	},
    {	NULL,	"otir",		(char)OTIR2,		0,	0xB3	},
    {	NULL,	"otirw",	(char)OTIRW,		0,	0	},	
    {	NULL,	"outd",		(char)OUTD2,		0,	0xAB	},
    {	NULL,	"outdw",	(char)OUTDW,		0,	0	},
    {	NULL,	"outi",		(char)OUTI2,		0,	0xA3	},
    {	NULL,	"outiw",	(char)OUTIW,		0,	0	},
    {	NULL,	"reti",		(char)RETI,		0,	0x4D	},
    {	NULL,	"retil",	(char)RETIL,		0,	0	},
    {	NULL,	"retn",		(char)RETN,		0,	0x45	},
    {	NULL,	"rld",		(char)BYTE2,		0,	0x6F	},
    {	NULL,	"rrd",		(char)BYTE2,		0,	0x67	},

	/* 64180 */

    {	NULL,	"otdm",		(char)BYTE180_2,	0,	0x8B	},
    {	NULL,	"otdmr",	(char)BYTE180_2,	0,	0x9B	},
    {	NULL,	"otim",		(char)BYTE180_2,	0,	0x83	},
    {	NULL,	"otimr",	(char)BYTE180_2,	0,	0x93	},
    {	NULL,	"slp",		(char)BYTE180_2,	0,	0x76	},

    {	NULL,	"in0",		(char)IN0,		0,	0x00	},
    {	NULL,	"out0",		(char)OUT0,		0,	0x01	},

    {	NULL,	"mlt",		(char)MLT,		0,	0x4C	},

    {	NULL,	"tst",		(char)TST,		0,	0x04	},
    {	NULL,	"tstio",	(char)TSTIO,		S_EOL,	0x74	}
};



