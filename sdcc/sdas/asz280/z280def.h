/* z280def.h -- Created from A.B.'s z80.h */
#define DEBUG 2
#define PRINTF if(pass>=DEBUG && debug > 1)printf
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
 *  Include the oddball 2 argument shift/rotate instructions
 *    (0 for strict Z80, 1 for 'sdasz80/asz80' compatibility)
 */
#define ODDBALL_SHIFT_ROTATE 1


/*
 * Indirect Addressing delimeters
 */
 
/* Used for Short indeX (SX) addressing */
#define	LFIND	'('
#define RTIND	')'

/* User for long indeX (X or LX) addressing and PC-relative (RA) addressing */
#define LFBKT	'['
#define RTBKT	']'


#if 0	/* ARB */
/* define "Unity" (UN) based on the assembly variable LONGINT */
#ifdef LONGINT
# define U 1L
# error "This may not compile until \'asxxxx.h\' has more INT32\'s"
#else
# define U 1
#endif

/* make a bit mask from a number */
# define MM(x) (U<<(x))
#else	/* ARB */
#define	MM(x) (((INT32) 1) << (x))	/* ARB */
#endif	/* ARB */

/* here is the set inclusion macro */
#define IN(class_,item) ((item) && ((~(class_))&(item))==0)

/*
 * Register numbers
 */
enum { nB=0, nC, nD, nE, nH, nL, nSP, nA,	/* 0..7 note SP=6 */
       nIXH, nIXL, nPC, nI, 			/* 8..11 */
       nIYH, nIYL, nAF, nAFP, nR, nUSP,		/* 12..17 note AF=14=016  also note IX=8=010, IY=12=140	*/
/* Register attributes */
       nB8=18, nB16, nB32,	/* sizes, 8-bit, 16-bit, 32-bit */
/* Addressing Modes */
       nIMM,		/* immediate -- may not ever be used */
       nIR,		/* indirect thru register:  (HL), (BC), (DE)	*/
       nDA,		/* direct address				*/
       nLX,		/* long indexed [IX+dd], [IY+dd], [HL+dd]	*/
       nSX,		/* short indexed (IX+d), (IY+d)			*/
       nRA,		/* PC relative (PC+dd) (PC+addr) [addr]		*/
       nSR,		/* Stack relative (SP+dd)			*/
       nBX,		/* Both indexes (HL+IX) (HL+IY) (IX+IY)		*/

       nCOND,		/* conditional -- attribute, not a register	*/
       
       nEXT=30		/* external -- may not ever be used */
       };


/*
 * Registers -- jrc
 */
#define B8 MM(nB8)		/* bit-8 and bit-16 register size mask bits */
#define B16 MM(nB16)
#define B32 MM(nB32)
#define REGmask (~(-B8))	/* mask to register bits below B8 */
#define ADRMODEmask (-MM(nIMM))	/* mask the addressing mode */
#define REGandSIZEmask (~ADRMODEmask)	/* register and size mask */

/*	#define IND MM(nIND)		*/
/*	#define IMM MM(nIMM)		*/

#define M_IM MM(nIMM)		/* Immediate addressing mode		*/
#define M_IR MM(nIR)		/* Indirect thru register		*/
#define M_DA MM(nDA)		/* Direct address mode			*/
#define M_LX MM(nLX)		/* Long Indexed off of IX, IY, HL	*/
#define M_SX MM(nSX)		/* Short Indexed off of IX, IY		*/
#define M_RA MM(nRA)		/* PC relative				*/
#define M_SR MM(nSR)		/* Stack relative			*/
#define M_BX MM(nBX)		/* Both indexes; 2 of HL, IX, IY	*/

#define REG_A (MM(nA)|B8)
#define	REG_B (MM(nB)|B8)	/* bit 0 (Breg) plus bit 20 (8-bit flag) */
#define REG_C (MM(nC)|B8)
#define REG_BC (REG_B+REG_C)	/* bit 0,1 BC plus bit 21 (16-bit flag) */
#define REG_D (MM(nD)|B8)
#define	REG_E (MM(nE)|B8)
#define REG_DE (REG_D+REG_E)
#define REG_H (MM(nH)|B8)
#define REG_L (MM(nL)|B8)
#define REG_HL (REG_H+REG_L)
#define REG_DEHL (REG_DE+REG_HL)	/* B16 carries into B32 */
#define REG_SP (MM(nSP)|B16)
#define REG_IXH (MM(nIXH)|B8)
#define REG_IXL (MM(nIXL)|B8)
#define REG_IX (REG_IXH+REG_IXL)	/* B8 carries into B16 for index IX */
#define REG_IYH (MM(nIYH)|B8)
#define REG_IYL (MM(nIYL)|B8)
#define REG_IY (REG_IYH+REG_IYL)	/* B8 carries into B16 for index IY */
#define REG_PC (MM(nPC)|B16)
#define REG_I (MM(nI)|B8)
#define REG_R (MM(nR)|B8)
#define REG_USP (MM(nUSP)|B16)
#define REG_AF (MM(nAF)|B16)
#define REG_AFP (MM(nAFP)|B16)

#define R_R8 (REG_A | REG_B | REG_C | REG_D | REG_E | REG_H | REG_L)
#define R_R8AE (REG_A | REG_B | REG_C | REG_D | REG_E )
#define R_R8X (REG_IXH | REG_IXL | REG_IYH | REG_IYL)
#define R_R8HX (REG_IXH | REG_IXL)
#define R_R8HY (REG_IYH | REG_IYL)
#define R_R16 (REG_BC | REG_DE | REG_HL | REG_SP)
#define R_R16X (REG_IX | REG_IY)
#define R_R16HX (REG_HL | REG_IX | REG_IY)
#define R_R16PP (REG_BC | REG_DE | REG_HL | REG_AF)
#define R_R16BX (REG_HL | REG_IX | REG_IY)

#define A_COND (MM(nCOND) | 0xFF)

#define A_IM (M_IM)
#define A_DA (M_DA)
#define A_IR (M_IR | REG_HL)
#define A_IRC (M_IR | REG_C)
#define A_IRX (M_IR | REG_HL | REG_IX | REG_IY)
	/*   (sp)  alone */
#define A_IRSP (M_IR | REG_SP)
#define A_IRBD (M_IR | REG_BC | REG_DE)
#define A_IRBDH (M_IR | REG_BC | REG_DE | REG_HL)
#define A_LX (M_LX | REG_HL | REG_IX | REG_IY )
#define A_LXY (M_LX | REG_IX | REG_IY)
#define A_SX (M_SX | M_IR | REG_IX | REG_IY)
	/*   (sp) or (sp+offset)  */
#define A_SR (M_SR | REG_SP | A_IRSP)
#define A_BX (M_BX | R_R16BX)
#define A_RA (M_RA | REG_PC)



/*
 * argument masks
 */

enum { nLITERAL0, REG8s, REG8d, PHI, /*DISP8,*/ IMMED8, IMMED16,	/*  0..5   */
	REG16m, /*gADDRESS, DISP16,*/ RXXa, RXXb, nRBX, nRBXd, nREL8, nREL16,	/*  6..12  */
	nREGXY,
	nnCMD_FINAL
    };

#if (nnCMD_FINAL<16)
# define CMD_WIDTH 8
# define ARG_CMD INT32		/* was 'short' */
# define ARG_MASK 0x0F
# define A1 8
# define A2 12
#else
# define CMD_WIDTH 16
# define ARG_CMD INT32
# define A1 8
# define A2 16
# define ARG_MASK 0xFF
#endif

#define gADDRESS IMMED16
#define DISP16	IMMED16
#define DISP8	IMMED8

#define r_1s (REG8s<<A1)
#define r_2s (REG8s<<A2)
#define r_1d (REG8d<<A1)
#define r_2d (REG8d<<A2)
#define rx_1s r_1s
#define rx_2s r_2s
#define rx_1d r_1d
#define rx_2d r_2d
#define phi1 (PHI<<A1)
#define phi2 (PHI<<A2)
#define disp1s (DISP8<<A1)
#define disp2s (DISP8<<A2)
#define immed1b (IMMED8<<A1)
#define immed2b (IMMED8<<A2)
#define immed1w (IMMED16<<A1)
#define immed2w (IMMED16<<A2)
#define rr_2s (REG16m<<A2)
#define rr_1d (REG16m<<A1)
#define xxa_1 (RXXa<<A1)
#define xxa_2 (RXXa<<A2)
#define xxb_1 (RXXb<<A1)
#define xxb_2 (RXXb<<A2)
#define xy_1m (nREGXY<<A1)
#define xy_2m (nREGXY<<A2)
#define addr1 (gADDRESS<<A1)
#define addr2 (gADDRESS<<A2)
#define disp1lx (DISP16<<A1)
#define disp2lx (DISP16<<A2)
#define bx_1 (nRBX<<A1)
#define bx_2 (nRBX<<A2)
#define bx_1d (nRBXd<<A1)
#define bx_2d (nRBXd<<A2)
#define rdispb1 (nREL8<<A1)
#define rdispb2 (nREL8<<A2)
#define rdispw1 (nREL16<<A1)
#define rdispw2 (nREL16<<A2)
#define cond1 (REG16m<<A1)	/* note overlap in operation */


/*
 * Symbol types
 */
#define	S_IMMED	30
#define	S_R8	31
#define	S_R8X	32
#define	S_R16	33
#define	S_R16X	34
#define	S_CND	35
#define	S_FLAG	36

/*
 * Indexing modes
 */
#define	S_INDB	40
#define	S_IDC	41
#define	S_INDR	50
#define	S_IDBC	50
#define	S_IDDE	51
#define	S_IDHL	52
#define	S_IDSP	53
#define	S_IDIX	55
#define	S_IDIY	56
#define	S_INDM	57
/* PC relative addressing */
#define S_RA	58
/* (HL+IX), (HL+IY), (IX+IY) */
#define S_BX	59

/*
 * Instruction types
 */

/*
 * Instruction types defined another way
 */
enum {
	S_LDXXX=60,

/* HD64180 instructions below S_CPU */
	S_CPU,
	S_DEBUG,

	 
	ADC, ADD, ADDW, AND,
	BIT, SET, RES, IM, RST,		/* first arg must be an absolute expression; bit ops are first */
	CALL, JP, JR, RET,
	CP, CPW, DEC, DECB, DECW, DI,
	DIV, DIVU, DIVUW, DIVW, DJNZ, EI,
	EX, EXTS, INC, INCW, INN, INDW, INDRW, INIW, INIRW, INW,
	JAF, JAR,
	LD, LDA, LDW, NEG, OR, OUT, OUTDW, OUTIW, OTDRW, OTIRW, OUTW,
	PCACHE, POP, PUSH, RETI, RETN, RETIL,
	RL, RLC, RR, RRC, SLA, SRA, SRL,
					SLL,	/* Z80 undocumented */
	SBC, SC, SUB, SUBW, TSET, TSTI, XOR,
	LDCTL, LDUD, LDUP,
	MULT, MULTU, MULTUW, MULTW,
	IN0, MLT, OUT0, TST, TSTIO,		/* Z180/HD64 */
	
	BYTE1, HALT1,
	BYTE2, IND2, INDR2, INI2, INIR2, OUTD2, OUTI2, OTDR2, OTIR2,
	BYTE180_2,
	nnMNEMONIC
};



/*
 * Instruction classes
 */
#define	I_Z80	1
#define	I_Z80io	2	/* Z80 I/O which may be excluded on the Z280 */
#define	I_Z80u	4	/* Z80 undocumented */
#define	I_Z180	8	/* HD64 and Z180 including I/O */
#define	I_Z280	16	/* Z280 user mode */
#define	I_Z280io 32	/* Z280 I/O	*/
#define	I_Z280p	64	/* Z280 privileged  */
#define I_Z80p	128	/* Z80, may be excluded on the Z280 */

/*
 *  Machine definitions // Processor classes
 */
#define	M_Z80 (I_Z80|I_Z80io|I_Z80p)			/* vanilla Z80	*/
#define M_Z80u (M_Z80|I_Z80u)				/* allow Z80 undocumented instructions */
#define M_Z180 (M_Z80|I_Z180)				/* Z180 or HD64180 is a Z80 plus a few instructions */
#define M_Z280n (I_Z80|I_Z280)				/* Z280 with no I/O instructions */
#define M_Z280	(I_Z80|I_Z80io|I_Z280|I_Z280io)		/* Z280 with all but privileged instructions */
#define M_Z280p	(M_Z280|I_Z280p|I_Z80p)			/* Z280 with all privileged instructions */


/*
 * Opcode Cycle Definitions
 */
#define	OPCY_SDP	((char) (0xFF))
#define	OPCY_ERR	((char) (0xFE))

/*	OPCY_NONE	((char) (0x80))	*/
/*	OPCY_MASK	((char) (0x7F))	*/

#define	OPCY_CPU	((char) (0xFD))


struct adsym
{
	char	a_str[8];	/* addressing string */
	INT32	a_val;		/* addressing mode value */	/* ARB */
};


struct a_table {
	int	cpulvl;		/* this may become a short or an int32 in the future */
	int	imne;		/* integer mnemonic; e.g., ADD, LD, BIT  */
	INT32	arg1, arg2;	/* argument templates */
	ARG_CMD	ops[4];		/* how to handle output bytes/words */
    };


extern	struct	adsym	R8[];
extern	struct	adsym	R8X[];
extern	struct	adsym	R16[];
extern	struct	adsym	R16X[];
extern	struct	adsym	CND[];
extern	struct	adsym	TOKEN[];
extern	struct	adsym	CONDITIONAL[];
extern	struct	a_table atable[];
extern	a_uint	mchtyp;		/* ARB */


	/* machine dependent functions */

#ifdef	OTHERSYSTEM
	
	/* z280adr.c */
extern	VOID 		dump_expr(struct expr *ep);	/* ARB */
extern	VOID		list_indx(INT32 indx);		/* ARB */
extern	INT32		new_addr(struct expr *esp);
extern	INT32		admode(struct adsym *sp);
extern	int		srch(char *str);

	/* z280mch.c */
extern	a_uint		debug;	/* ARB */
extern	VOID		machine(struct mne *mp);
extern	int		mchpcr(struct expr *esp);
extern	VOID		minit(void);
extern	int		nreg(a_uint m);
extern	int		generate(struct expr *arg, a_uint marg, INT32 op, int byt);	/* ARB */
extern	VOID		emit(INT32 op);
extern	int		new_machine(struct mne *mp);
extern	int		arg_match(a_uint class, a_uint arg);

#else

	/* z280adr.c */
extern	VOID 		dump_expr();	/* ARB */
extern	VOID		list_indx();	/* ARB */
extern	int		new_addr();
extern	int		admode();
extern	int		srch();

	/* z280mch.c */
extern a_uint debug;	/* ARB */
extern	VOID		machine();
extern	int		mchpcr();
extern	VOID		minit();
extern	int		nreg();
extern	int		generate();
extern	VOID		emit();
extern	int		new_machine();
extern	int		arg_match();

#endif

