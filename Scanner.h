/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: A22
* Date: March 18 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Scanner (.h)
*************************************************************/
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

#pragma warning(1:4001) 

#pragma warning(error:4001)

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	KEY_T,		/*  Method name identifier token (start: &) */
	CVID_T,		/* Character Variable Identifier Token */
	IVID_T,		/* Integer Variable Identifier Token */
	FVID_T,		/* Float Variable Identifier Token */
	SVID_T,		/* String Variable Identifier Token */
	ASSI_T,
	ARIT_T,		/* Arithmetic Operator Token */
	RELA_T,		/* Relational Operator Token */
	LOGI_T,		/* Logical Operator Token */
	COM_T,		/* Comment token */
	COMA_T,		/* Comma token */
	INT_T,		/* Integer literal token */
	FPL_T,		/* Floating Point literal token */
	STR_T,		/* String literal token */
	CHR_T,		/* Character literal token */
	LPR_T,		/* Left parenthesis token */
	RPR_T,		/* Right parenthesis token */
	LBR_T,		/* Left brace token */
	RBR_T,		/* Right brace token */
	KW_T,		/* Keyword token */
	KW_P_T,
	EOS_T,		/* End of statement (semicolon) */
	RTE_T,		/* Run-time error token */
	SEOF_T		/* Source end-of-file token */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	carthage_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	carthage_int intValue;						/* integer literal attribute (value) */
	carthage_int keywordIndex;					/* keyword index in the keyword table */
	carthage_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	carthage_flt floatValue;					/* floating-point literal attribute (value) */
	carthage_chr charValue;
	carthage_chr idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	carthage_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	carthage_flg flags;			/* Flags information */
	union {
		carthage_int intValue;				/* Integer value */
		carthage_flt floatValue;			/* Float value */
		carthage_chr* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	carthage_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO: Error states and illegal state */
#define ES  19	     /* Error state with no retract */
#define ER  20		/* Error state with retract */
#define IS -1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 13

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '!'
#define CHRCOL4 '\"'
#define CHRCOL5 '#'
#define CHRCOL6 '?'
#define CHRCOL7 '$'
#define CHRCOL8 '@'
#define CHRCOL9 '.'
#define CHRCOL10 '\''
/* These constants will be used on VID / MID function */
#define KEYMARK '#'
#define VARMARK '!'
#define COMMARK '?'
#define CVIDPREFIX '?'
#define FVIDPREFIX '$'
#define SVIDPREFIX '@'

/* Transition table - follows same logic as A22 however have been rearranged for neatness and clarity. */
static carthage_int transitionTable[][TABLE_COLUMNS] = {
	/*   [A-z] , [0-9],    _,    !,    ",         SEOF, other
		   L(0),  D(1), U(2), V(3),  Q(4), I(5)  H(6)  F(7)  S(8)  P(9)  X(10) E(11), O(12) */
		{    ES,     9,   ES,    1,    8,    2,   13,   ES,   ES,   ES,   11,   ER,   ES}, // S0: NOAS
		{     1,     1,    1,   ES,   ES,    5,    4,    6,    7,   ER,   ES,   ES,   ES}, // S1: NOAS
		{     2,    ES,   ES,   ES,   ES,    3,   ES,   ES,   ES,   ES,   ES,   ER,   ES}, // S2: NOAS 
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S3: ASNR (KEYVID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S4: ASNR (CVID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S5: ASNR (IVID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S6: ASNR (FVID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S7: ASNR (SVID)
		{     8,     8,    8,    8,   14,    8,    8,    8,    8,    8,   ES,   ER,    8}, // S8: NOAS
		{    ES,     9,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   10,   ES,   ER,   15}, // S9: NOAS
		{    ES,    10,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   ER,   16}, // S10: NOAS
		{    12,    12,   12,   12,   12,   12,   12,   12,   12,   12,   12,   ER,   12}, // S11: NOAS
		{    ES,    ES,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   ES,   17,   ER,   ES}, // S12: NOAS
		{    13,    13,   13,   13,   13,   13,   18,   13,   13,   13,   13,   ER,   13}, // S13: NOAS
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S14: ASNR (SL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S15: ASWR (IL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S16: ASWR (FPL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S17: ASWR (CL)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S18: ASWR (COMM)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}, // S19: ASNR (ES)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS,   IS}  // S20: ASWR (ER)
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static carthage_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	NOAS, /* 02 */
	ASNR, /* 03 (KVID) - Keyword Identifier */
	ASNR, /* 04 (CVID) - Character Identifier */
	ASNR, /* 05 (IVID) - Integer Identifier */
	ASNR, /* 06 (FVID) - FLoat Identifier */
	ASNR, /* 07 (SVID) - String Identifier */
	NOAS, /* 08 (NULL)*/
	NOAS, /* 09 (NULL)*/
	NOAS, /* 10 (NULL)*/
	NOAS, /* 11 (NULL)*/
	NOAS, /* 12 (NULL)*/
	NOAS, /* 13 (NULL)*/
	ASNR, /* 14 (String - no retract) */
	ASWR, /* 15 (Integer - no retract) */
	ASWR, /* 16 (Float - no retract) */
	ASNR, /* 17 (Character - no retract)*/
	ASNR, /* 18 (Comment - no retract) */
	ASNR, /* 19 (Err1 - no retract) */
	ASWR  /* 20 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
carthage_int startScanner(BufferPointer psc_buf);
static carthage_int nextClass(carthage_chr c);			/* character class function */
static carthage_int nextState(carthage_int, carthage_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(carthage_chr* lexeme);

/* Declare accepting states functions */
Token funcSL(carthage_chr lexeme[]);
Token funcIL(carthage_chr lexeme[]);
Token funcCL(carthage_chr lexeme[]);
Token funcFPL(carthage_chr lexeme[]);
Token funcCOM(carthage_chr lexeme[]);
Token funcID(carthage_chr lexeme[]);
Token varID(carthage_chr lexeme[]);
Token funcKEY(carthage_chr lexeme[]);
Token funcErr(carthage_chr lexeme[]);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	NULL,		/* -    [02] */
	funcID,		/* Keyword Identifier*/
	varID,		/* Char Identifier  */
	varID,		/* Int Identifier  */
	varID,		/* Float Identifier  */
	varID,		/* String Identifier  */
	NULL,		
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	funcSL,		/* SL   [14] */
	funcIL,		/* IL   [15] */
	funcFPL,	/* FPL  [16] */
	funcCL,		/* CL   [17] */
	funcCOM,	/* COM  [18] */
	funcErr,	/* ERR1 [19] */
	funcErr		/* ERR2 [20] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 13

/* TO_DO: Define the list of keywords */
static carthage_chr* keywordTable[KWT_SIZE] = {
	"prime",
	"VAR",
	"RUN",
	"YIELD",
	"STRING",
	"IF",
	"STOP",
	"EL",
	"WHILE",
	"DO",
	"LOOP",
	"output",
	"input"
};

#endif
