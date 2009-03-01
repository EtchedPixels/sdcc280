/* aslink.h */

/*
 * (C) Copyright 1989-2000
 * All Rights Reserved
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 *
 *   With enhancements from
 *	John L. Hartman	(JLH)
 *	jhartman@compuserve.com
 *
 *	Bill McKinnon
 *	w_mckinnon@conknet.com
 */

#define	VERSION	"V03.00"

/*)Module	aslink.h
 *
 *	The module aslink.h contains the definitions for constants,
 *	structures, global variables, and LKxxxx functions
 *	contained in the LKxxxx.c files.
 */

/*)BUILD
	$(PROGRAM) =	ASLINK
	$(INCLUDE) =	ASLINK.H
	$(FILES) = {
		LKMAIN.C
		LKLEX.C
		LKAREA.C
		LKHEAD.C
		LKSYM.C
		LKEVAL.C
		LKDATA.C
		LKLIST.C
		LKRLOC.C
		LKLIBR.C
		LKS19.C
		LKIHX.C
	}
	$(STACK) = 2000
*/

/* DECUS C void definition */
/* File/extension seperator */

#ifdef	decus
#define	VOID	char
#define	FSEPX	'.'
#endif

/* PDOS C void definition */
/* File/extension seperator */

#ifdef	PDOS
#define	VOID	char
#define	FSEPX	':'
#endif

/* Default void definition */
/* File/extension seperator */

#ifndef	VOID
#define	VOID	void
#define	FSEPX	'.'
#define	OTHERSYSTEM
#endif

/*
 * Error definitions
 */
#define	ER_NONE		0	/* No error */
#define	ER_WARNING	1	/* Warning */
#define	ER_ERROR	2	/* Assembly error */
#define	ER_FATAL	3	/* Fatal error */

/*
 * This file defines the format of the
 * relocatable binary file.
 */

#define NCPS	80		/* characters per symbol */
#define	NDATA	16		/* actual data */
#define	NINPUT	200		/* Input buffer size */
#define	NHASH	64		/* Buckets in hash table */
#define	HMASK	077		/* Hash mask */
#define	NLPP	60		/* Lines per page */
#define	NTXT	16		/* T values */
#define	NMAX	78		/* Maximum S19/IHX line length */
#define	FILSPC	80		/* File spec length */

/*
 *	The "R_" relocation constants define values used in
 *	generating the assembler relocation output data for
 *	areas, symbols, and code.
 *
 *
 *	Relocation types.
 *
 *	       7     6     5     4     3     2     1     0
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 *	    | MSB | PAGn| PAG0| USGN| BYT2| PCR | SYM | BYT |
 *	    +-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define	R_WORD	0000		/* 16 bit */
#define	R_BYTE	0001		/*  8 bit */

#define	R_AREA	0000		/* Base type */
#define	R_SYM	0002

#define	R_NORM	0000		/* PC adjust */
#define	R_PCR	0004

#define	R_BYT1	0000		/* Byte count for R_BYTE = 1 */
#define	R_BYTX	0010		/* Byte count for R_BYTE = X */

#define	R_SGND	0000		/* Signed value */
#define	R_USGN	0020		/* Unsigned value */

#define	R_NOPAG	0000		/* Page Mode */
#define	R_PAG0	0040		/* Page '0' */
#define	R_PAG	0100		/* Page 'nnn' */

#define	R_LSB	0000		/* output low byte */
#define	R_MSB	0200		/* output high byte */

/*
 *	Additional "R_" functionality is required to support
 *	some microprocesssor architectures.   The 'illegal'
 *	"R_" mode of R_WORD | R_BYTX is used as a designator
 *	of the extended R_ modes.  The extended modes replace
 *	the PAGING modes and are being added in an adhoc manner
 *	as follows:
 *
 * Extended Mode relocation flags
 *
 *	   7     6     5     4     3     2     1     0
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 *	| MSB |  x  |  x  | USGN|  1  | PCR | SYM |  0  |
 *	+-----+-----+-----+-----+-----+-----+-----+-----+
 */

#define	R_ECHEK	0011		/* Extended Mode Check Bits */
#define	R_EXTND	0010		/* Extended Mode Code */
#define	R_EMASK	0151		/* Extended Mode Mask */

/* #define R_AREA 0000 */	/* Base type */
/* #define R_SYM  0002 */

/* #define R_NORM 0000 */	/* PC adjust */
/* #define R_PCR  0004 */

/* #define R_SGND 0000 */	/* Signed value */
/* #define R_USGN 0020 */	/* Unsigned value */

/* #define R_LSB  0000 */	/* output low byte */
/* #define R_MSB  0200 */	/* output high byte */

#define	R_J11	0010		/* JLH: 11 bit JMP and CALL (8051) */
#define R_J19   0050		/* BM:	19 bit JMP and CALL (DS80C390) */
#define R_3BYTE	0110		/* 	24 bit */
#define R_4BYTE	0150		/* 	32 bit */

/*
 * Global symbol types.
 */
#define	S_REF	1		/* referenced */
#define	S_DEF	2		/* defined */

/*
 * Area types
 */
#define	A_CON	000		/* concatenate */
#define	A_OVR	004		/* overlay */
#define	A_REL	000		/* relocatable */
#define	A_ABS	010		/* absolute */
#define	A_NOPAG	000		/* non-paged */
#define	A_PAG	020		/* paged */

/*
 * File types
 */
#define	F_STD	1		/* stdin */
#define	F_LNK	2		/* File.lnk */
#define	F_REL	3		/* File.rel */

/*
 *	General assembler address type
 */
typedef unsigned int addr_t;

/*
 *	The structures of head, area, areax, and sym are created
 *	as the REL files are read during the first pass of the
 *	linker.  The struct head is created upon encountering a
 *	H directive in the REL file.  The structure contains a
 *	link to a link file structure (struct lfile) which describes
 *	the file containing the H directive, the number of data/code
 *	areas contained in this header segment, the number of
 *	symbols referenced/defined in this header segment, a pointer
 *	to an array of pointers to areax structures (struct areax)
 *	created as each A directive is read, and a pointer to an
 *	array of pointers to symbol structures (struct sym) for
 *	all referenced/defined symbols.  As H directives are read
 *	from the REL files a linked list of head structures is
 *	created by placing a link to the new head structure
 *	in the previous head structure.
 */
struct	head
{
	struct	head   *h_hp;	/* Header link */
	struct	lfile  *h_lfile;/* Associated file */
	int	h_narea;	/* # of areas */
	struct	areax **a_list;	/* Area list */
	int	h_nglob;	/* # of global symbols */
	struct	sym   **s_list;	/* Globle symbol list */
	char *	m_id;		/* Module name */
};

/*
 *	A structure area is created for each 'unique' data/code
 *	area definition found as the REL files are read.  The
 *	struct area contains the name of the area, a flag byte
 *	which contains the area attributes (REL/CON/OVR/ABS),
 *	the area base address set flag byte (-b option), and the
 *	area base address and total size which will be filled
 *	in at the end of the first pass through the REL files.
 *	As A directives are read from the REL files a linked
 *	list of unique area structures is created by placing a
 *	link to the new area structure in the previous area structure.
 */
struct	area
{
	struct	area	*a_ap;	/* Area link */
	struct	areax	*a_axp;	/* Area extension link */
	addr_t	a_addr;		/* Beginning address of area */
	addr_t	a_size;		/* Total size of the area */
	char	a_bset;		/* Area base address set */
	char	a_flag;		/* Flag byte */
	char *	a_id;		/* Name */
};

/*
 *	An areax structure is created for every A directive found
 *	while reading the REL files.  The struct areax contains a
 *	link to the 'unique' area structure referenced by the A
 *	directive and to the head structure this area segment is
 *	a part of.  The size of this area segment as read from the
 *	A directive is placed in the areax structure.  The beginning
 *	address of this segment will be filled in at the end of the
 *	first pass through the REL files.  As A directives are read
 *	from the REL files a linked list of areax structures is
 *	created for each unique area.  The final areax linked
 *	list has at its head the 'unique' area structure linked
 *	to the linked areax structures (one areax structure for
 *	each A directive for this area).
 */
struct	areax
{
	struct	areax	*a_axp;	/* Area extension link */
	struct	area	*a_bap;	/* Base area link */
	struct	head	*a_bhp;	/* Base header link */
	addr_t	a_addr;		/* Beginning address of section */
	addr_t	a_size;		/* Size of the area in section */
};

/*
 *	A sym structure is created for every unique symbol
 *	referenced/defined while reading the REL files.  The
 *	struct sym contains the symbol's name, a flag value
 *	(not used in this linker), a symbol type denoting
 *	referenced/defined, and an address which is loaded
 *	with the relative address within the area in which
 *	the symbol was defined.  The sym structure also
 *	contains a link to the area where the symbol was defined.
 *	The sym structures are linked into linked lists using
 *	the symbol link element.
 */
struct	sym
{
	struct	sym	*s_sp;	/* Symbol link */
	struct	areax	*s_axp;	/* Symbol area link */
	char	s_type;		/* Symbol subtype */
	char	s_flag;		/* Flag byte */
	addr_t	s_addr;		/* Address */
	char	*s_id;		/* Name (JLH) */
	char	*m_id;		/* Module symbol define in */
};

/*
 *	The structure lfile contains a pointer to a
 *	file specification string, an index which points
 *	to the file name (past the 'path'), the file type,
 *	an object output flag, and a link to the next
 *	lfile structure.
 */
struct	lfile
{
	struct	lfile	*f_flp;	/* lfile link */
	int	f_type;		/* File type */
	char	*f_idp;		/* Pointer to file spec */
	int	f_idx;		/* Index to file name */
	int	f_obj;		/* Object output flag */
};

/*
 *	The struct base contains a pointer to a
 *	base definition string and a link to the next
 *	base structure.
 */
struct	base
{
	struct	base  *b_base;	/* Base link */
	char	      *b_strp;	/* String pointer */
};

/*
 *	The struct globl contains a pointer to a
 *	global definition string and a link to the next
 *	global structure.
 */
struct	globl
{
	struct	globl *g_globl;	/* Global link */
	char	      *g_strp;	/* String pointer */
};

/*
 *	A structure sdp is created for each 'unique' paged
 *	area definition found as the REL files are read.
 *	As P directives are read from the REL files a linked
 *	list of unique sdp structures is created by placing a
 *	link to the new sdp structure in the previous area structure.
 */
struct	sdp
{
	struct	area  *s_area;	/* Paged Area link */
	struct	areax *s_areax;	/* Paged Area Extension Link */
	addr_t	s_addr;		/* Page address offset */
};

/*
 *	The structure rerr is loaded with the information
 *	required to report an error during the linking
 *	process.  The structure contains an index value
 *	which selects the areax structure from the header
 *	areax structure list, a mode value which selects
 *	symbol or area relocation, the base address in the
 *	area section, an area/symbol list index value, and
 *	an area/symbol offset value.
 */
struct	rerr
{
	int	aindex;		/* Linking area */
	int	mode;		/* Relocation mode */
	addr_t	rtbase;		/* Base address in section */
	int	rindex;		/* Area/Symbol reloaction index */
	addr_t	rval;		/* Area/Symbol offset value */
};

/*
 *	The structure lbpath is created for each library
 *	path specification input by the -k option.  The
 *	lbpath structures are linked into a list using
 *	the next link element.
 */
struct lbpath {
	struct	lbpath	*next;
	char		*path;
};

/*
 *	The structure lbname is created for all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file.  The element path points to
 *	the path string, element libfil points to the library
 *	file string, and the element libspc is the concatenation
 *	of the valid path and libfil strings.
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 *
 *	Each library file contains a list of object files
 *	that are contained in the particular library. e.g.:
 *
 *		\iolib\termio
 *		\inilib\termio
 *
 *	Only one specification per line is allowed.
 */
struct lbname {
	struct	lbname	*next;
	char		*path;
	char		*libfil;
	char		*libspc;
	int		f_obj;
};

/*
 *	The function fndsym() searches through all combinations of the
 *	library path specifications (input by the -k option) and the
 *	library file specifications (input by the -l option) that
 *	lead to an existing file for a symbol definition.
 *
 *	The structure lbfile is created for the first library
 *	object file which contains the definition for the
 *	specified undefined symbol.
 *
 *	The element libspc points to the library file path specification
 *	and element relfil points to the object file specification string.
 *	The element filspc is the complete path/file specification for
 *	the library file to be imported into the linker.  The f_obj
 *	flag specifies if the object code from this file is
 *	to be output by the linker.  The file specification
 *	may be formed in one of two ways:
 *
 *	(1)	If the library file contained an absolute
 *		path/file specification then this becomes filspc.
 *		(i.e. C:\...)
 *
 *	(2)	If the library file contains a relative path/file
 *		specification then the concatenation of the path
 *		and this file specification becomes filspc.
 *		(i.e. \...)
 *
 *	The lbpath structures are linked into a list
 *	using the next link element.
 */
struct lbfile {
	struct	lbfile	*next;
	char		*libspc;
	char		*relfil;
	char		*filspc;
	int		f_obj;
};

/*
 *	External Definitions for all Global Variables
 */

extern	char	*_abs_;		/*	= { ".  .ABS." };
				 */
extern	int	lkerr;		/*	ASLink error flag
				 */
extern	char	*ip;		/*	pointer into the REL file
				 *	text line in ib[]
				 */
extern	char	ib[NINPUT];	/*	REL file text line
				 */
extern	char	*rp;		/*	pointer into the LST file
				 *	text line in rb[]
				 */
extern	char	rb[NINPUT];	/*	LST file text line being
				 *	address relocated
				 */
extern	char	ctype[];	/*	array of character types, one per
				 *	ASCII character
				 */

/*
 *	Character Type Definitions
 */
#define	SPACE	'\000'
#define ETC	'\000'
#define	LETTER	'\001'
#define	DIGIT	'\002'
#define	BINOP	'\004'
#define	RAD2	'\010'
#define	RAD8	'\020'
#define	RAD10	'\040'
#define	RAD16	'\100'
#define	ILL	'\200'

#define	DGT2	DIGIT|RAD16|RAD10|RAD8|RAD2
#define	DGT8	DIGIT|RAD16|RAD10|RAD8
#define	DGT10	DIGIT|RAD16|RAD10
#define	LTR16	LETTER|RAD16

extern	char	ccase[];	/*	an array of characters which
				 *	perform the case translation function
				 */

extern	struct	lfile	*filep;	/*	The pointers (lfile *) filep,
				 *	(lfile *) cfp, and (FILE *) sfp
				 *	are used in conjunction with
				 *	the routine getline() to read
				 *	asmlnk commands from
				 *	(1) the standard input or
				 *	(2) or a command file
				 *	and to read the REL files
				 *	sequentially as defined by the
				 *	asmlnk input commands.
				 *
				 *	The pointer *filep points to the
				 *	beginning of a linked list of
				 *	lfile structures.
				 */
extern	struct	lfile	*cfp;	/*	The pointer *cfp points to the
				 *	current lfile structure
				 */
extern	struct	lfile	*startp;/*	asmlnk startup file structure
				 */
extern	struct	lfile	*linkp;	/*	pointer to first lfile structure
				 *	containing an input REL file
				 *	specification
				 */
extern	struct	lfile	*lfp;	/*	pointer to current lfile structure
				 *	being processed by parse()
				 */
extern	struct	head	*headp;	/*	The pointer to the first
				 *	head structure of a linked list
				 */
extern	struct	head	*hp;	/*	Pointer to the current
				 *	head structure
				 */
extern	struct	area	*areap;	/*	The pointer to the first
				 *	area structure of a linked list
				 */
extern	struct	area	*ap;	/*	Pointer to the current
				 *	area structure
				 */
extern	struct	areax	*axp;	/*	Pointer to the current
				 *	areax structure
				 */
extern	struct	sym *symhash[NHASH]; /*	array of pointers to NHASH
				      *	linked symbol lists
				      */
extern	struct	base	*basep;	/*	The pointer to the first
				 *	base structure
				 */
extern	struct	base	*bsp;	/*	Pointer to the current
				 *	base structure
				 */
extern	struct	globl	*globlp;/*	The pointer to the first
				 *	globl structure
				 */
extern	struct	globl	*gsp;	/*	Pointer to the current
				 *	globl structure
				 */
extern	struct	sdp	sdp;	/*	Base Paged structure
				 */
extern	struct	rerr	rerr;	/*	Structure containing the
				 *	linker error information
				 */
extern	FILE	*ofp;		/*	Linker Output file handle
				 */
extern	FILE	*mfp;		/*	Map output file handle
				 */
extern	FILE	*rfp;		/*	File handle for output
				 *	address relocated ASxxxx
				 *	listing file
				 */
extern	FILE	*sfp;		/*	The file handle sfp points to the
				 *	currently open file
				 */
extern	FILE	*tfp;		/*	File handle for input
				 *	ASxxxx listing file
				 */
extern	int	oflag;		/*	Output file type flag
				 */
extern	int	objflg;		/*	Linked file/library object output flag
				 */
extern	int	mflag;		/*	Map output flag
				 */
extern	int	xflag;		/*	Map file radix type flag
				 */
extern	int	pflag;		/*	print linker command file flag
				 */
extern	int	uflag;		/*	Listing relocation flag
				 */
extern	int	wflag;		/*	Enable wide format listing
				 */
extern	int	zflag;		/*	Enable symbol case sensitivity
				 */
extern	int	radix;		/*	current number conversion radix:
				 *	2 (binary), 8 (octal), 10 (decimal),
				 *	16 (hexadecimal)
				 */
extern	int	line;		/*	current line number
				 */
extern	int	page;		/*	current page number
				 */
extern	int	lop;		/*	current line number on page
				 */
extern	int	pass;		/*	linker pass number
				 */
extern	int	rtcnt;		/*	count of elements in the
				 *	rtval[] and rtflg[] arrays
				 */
extern	addr_t	rtval[];	/*	data associated with relocation
				 */
extern	int	rtflg[];	/*	indicates if rtval[] value is
				 *	to be sent to the output file.
				 */
extern	char	rtbuf[];	/*	S19/IHX output buffer
				 */
extern	int	rtaflg;		/*	rtbuf[] processing
				 */
extern	addr_t	rtadr0;		/*
				 */
extern	addr_t	rtadr1;		/*
				 */
extern	addr_t	rtadr2;		/*
				 */
extern	int	obj_flag;	/*	Linked file/library object output flag
				 */
extern	int	a_bytes;	/*	REL file T Line address length
				 */
extern	int	hilo;		/*	REL file byte ordering
				 */
extern	addr_t	a_mask;		/*	Address Mask
				 */
extern	addr_t	s_mask;		/*	Sign Mask
				 */
extern	addr_t	v_mask;		/*	Value Mask
				 */
extern	int	gline;		/*	LST file relocation active
				 *	for current line
				 */
extern	int	gcntr;		/*	LST file relocation active
				 *	counter
				 */
extern	struct lbpath *lbphead;	/*	pointer to the first
				 *	library path structure
				 */
extern	struct lbname *lbnhead;	/*	pointer to the first
				 *	library name structure
				 */
extern	struct lbfile *lbfhead;	/*	pointer to the first
				 *	library file structure
				 */

/* C Library function definitions */
/* for reference only
extern	VOID		exit();
extern	int		fclose();
extern	char *		fgets();
extern	FILE *		fopen();
extern	int		fprintf();
extern	VOID		free();
extern	VOID *		malloc();
extern	char		putc();
extern	char *		strcpy();
extern	int		strlen();
extern	char *		strncpy();
extern	char *		strrchr();
*/

/* Program function definitions */

#ifdef	OTHERSYSTEM

/* lkmain.c */
extern	FILE *		afile(char *fn, char *ft, int wf);
extern	VOID		bassav(void);
extern	int		fndidx(char *str);
extern	VOID		gblsav(void);
extern	VOID		link(void);
extern	VOID		lkexit(int i);
extern	int		main(int argc, char *argv[]);
extern	VOID		map(void);
extern	int		parse(void);
extern	VOID		doparse(void);
extern	VOID		setbas(void);
extern	VOID		setgbl(void);
extern	VOID		usage(int n);

/* lklex.c */
extern	char		endline(void);
extern	int		get(void);
extern	VOID		getfid(char *str, int c);
extern	VOID		getid(char *id, int c);
extern	int		getline(void);
extern	int		getmap(int d);
extern	int		getnb(void);
extern	int		more(void);
extern	VOID		skip(int c);
extern	VOID		unget(int c);

/* lkarea.c */
extern	VOID		lkparea(char *id);
extern	VOID		lnkarea(void);
extern	VOID		lnksect(struct area *tap);
extern	VOID		newarea(void);

/* lkhead.c */
extern	VOID		module(void);
extern	VOID		newhead(void);

/* lksym.c */
extern	int		hash(char *p, int cflag);
extern	struct	sym *	lkpsym(char *id, int f);
extern	char *		new(unsigned int n);
extern	struct	sym *	newsym(void);
extern	char *		strsto(char *str);
extern	VOID		symdef(FILE *fp);
extern	int		symeq(char *p1, char *p2, int cflag);
extern	VOID		syminit(void);
extern	VOID		symmod(FILE *fp, struct sym *tsp);
extern	addr_t		symval(struct sym *tsp);

/* lkeval.c */
extern	int		digit(int c, int r);
extern	addr_t		eval(void);
extern	addr_t		expr(int n);
extern	int		oprio(int c);
extern	addr_t		term(void);

/* lklist.c */
extern	int		dgt(int rdx, char *str, int n);
extern	VOID		newpag(FILE *fp);
extern	VOID		slew(struct area *xp);
extern	VOID		lstarea(struct area *xp);
extern	VOID		lkulist(int i);
extern	VOID		lkalist(addr_t pc);
extern	VOID		lkglist(addr_t pc, int v);

/* lkrloc.c */
extern	addr_t		adb_1b(addr_t v, int i);
extern	addr_t		adb_2b(addr_t v, int i);
extern	addr_t		adb_3b(addr_t v, int i);
extern	addr_t		adb_4b(addr_t v, int i);
extern	addr_t		adb_xb(addr_t v, int i);
extern	addr_t		adb_hi(addr_t v, int i);
extern	addr_t		adb_lo(addr_t v, int i);
extern	addr_t		adw_xb(int x, addr_t v, int i);
extern	addr_t		evword(void);
extern	VOID		rele(void);
extern	VOID		reloc(int c);
extern	VOID		relt(void);
extern	VOID		relr(void);
extern	VOID		relp(void);
extern	VOID		relerr(char *str);
extern	char *		errmsg[];
extern	VOID		errdmp(FILE *fptr, char *str);
extern	VOID		relerp(char *str);
extern	VOID		erpdmp(FILE *fptr, char *str);
extern	VOID		prntval(FILE *fptr, addr_t v);

/* lklibr.c */
extern	VOID		addfile(char *path, char *libfil);
extern	VOID		addlib(void);
extern	VOID		addpath(void);
extern	int		fndsym(char *name);
extern	VOID		library(void);
extern	VOID		loadfile(char *filspc);
extern	VOID		search(void);

/* lkout.c */
extern	VOID		lkout(int i);
extern	VOID		ixx(int i);
extern	VOID		iflush(void);
extern	VOID		sxx(int i);
extern	VOID		sflush(void);

#else

/* lkmain.c */
extern	FILE *		afile();
extern	VOID		bassav();
extern	int		fndidx();
extern	VOID		gblsav();
extern	VOID		link();
extern	VOID		lkexit();
extern	int		main();
extern	VOID		map();
extern	int		parse();
extern	VOID		doparse();
extern	VOID		setbas();
extern	VOID		setgbl();
extern	VOID		usage();

/* lklex.c */
extern	char		endline();
extern	int		get();
extern	VOID		getfid();
extern	VOID		getid();
extern	int		getline();
extern	int		getmap();
extern	int		getnb();
extern	int		more();
extern	VOID		skip();
extern	VOID		unget();

/* lkarea.c */
extern	VOID		lkparea();
extern	VOID		lnkarea();
extern	VOID		lnksect();
extern	VOID		newarea();

/* lkhead.c */
extern	VOID		module();
extern	VOID		newhead();

/* lksym.c */
extern	int		hash();
extern	struct	sym *	lkpsym();
extern	VOID *		new();
extern	struct	sym *	newsym();
extern	char *		strsto();
extern	VOID		symdef();
extern	int		symeq();
extern	VOID		syminit();
extern	VOID		symmod();
extern	addr_t		symval();

/* lkeval.c */
extern	int		digit();
extern	addr_t		eval();
extern	addr_t		expr();
extern	int		oprio();
extern	addr_t		term();

/* lklist.c */
extern	int		dgt();
extern	VOID		newpag();
extern	VOID		slew();
extern	VOID		lstarea();
extern	VOID		lkulist();
extern	VOID		lkalist();
extern	VOID		lkglist();

/* lkrloc.c */
extern	addr_t		adb_1b();
extern	addr_t		adb_2b();
extern	addr_t		adb_3b();
extern	addr_t		adb_4b();
extern	addr_t		adb_xb();
extern	addr_t		adb_hi();
extern	addr_t		adb_lo();
extern	addr_t		adw_xb();
extern	addr_t		evword();
extern	VOID		rele();
extern	VOID		reloc();
extern	VOID		relt();
extern	VOID		relr();
extern	VOID		relp();
extern	VOID		relerr();
extern	char *		errmsg[];
extern	VOID		errdmp();
extern	VOID		relerp();
extern	VOID		erpdmp();
extern	VOID		prntval();

/* lklibr.c */
extern	VOID		addfile();
extern	VOID		addlib();
extern	VOID		addpath();
extern	int		fndsym();
extern	VOID		library();
extern	VOID		loadfile();
extern	VOID		search();

/* lkout.c */
extern	VOID		lkout();
extern	VOID		ixx();
extern	VOID		iflush();
extern	VOID		sxx();
extern	VOID		sflush();

#endif

