/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/******************************************************************
* File name: scanner.c
* Compiler: MS Visual Studio 2022
* Author: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22
* Date: March 18 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list:*Token tokenizer(void)
* carthage_int nextState(carthage_int state, carthage_chr c)
* carthage_int nextClass(carthage_chr c)
* Token funcIL(carthage_chr lexeme[]);
* Token funcCL(carthage_chr lexeme[]);
* Token funcFPL(carthage_chr lexeme[]);
* Token funcCOM(carthage_chr lexeme[]);
* Token funcID(carthage_chr lexeme[]);
* Token varID(carthage_chr lexeme[]);
* Token funcKEY(carthage_chr lexeme[]);
* Token funcErr(carthage_chr lexeme[]);
****************************************************************/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
carthage_int line;								/* Current line number of the source code */
extern carthage_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern carthage_int stateType[];
extern carthage_chr* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern carthage_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

carthage_int startScanner(BufferPointer psc_buf) {
	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	carthage_chr c;	/* input symbol */
	carthage_int state = 0;		/* initial state of the FSM */
	carthage_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	carthage_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	carthage_int lexLength;		/* token length */
	carthage_int i;				/* counter */
	carthage_chr newc;			/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
		case ' ':
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;
			/* Cases for arithmetic operators */
		case '+':
			currentToken.code = ARIT_T;
			currentToken.attribute.charValue = '+';
			return currentToken;
		case '-':
			currentToken.code= ARIT_T;
			currentToken.attribute.charValue = '-';
			return currentToken;
		case '/':
			currentToken.code = ARIT_T;
			currentToken.attribute.charValue = '/';
			return currentToken;
		case '*':
			currentToken.code = ARIT_T;
			currentToken.attribute.charValue = '*';
			return currentToken;
			/* Cases for relational operators */
		case '=':
			currentToken.code = ASSI_T;
			return currentToken;
		case '~':
			currentToken.code = RELA_T;
			return currentToken;
		case '>':
			currentToken.code = RELA_T;
			return currentToken;
		case '<':
			currentToken.code = RELA_T;
			return currentToken;
			/* Cases for logical operators */
		case '&':
			currentToken.code = LOGI_T;
			return currentToken;
		case '|':
			currentToken.code = LOGI_T;
			return currentToken;
		case 'N':
			currentToken.code = LOGI_T;
			return currentToken;
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
		case ',':
			currentToken.code = COMA_T;
			return currentToken;
		case '?':
			newc = bGetChar(sourceBuffer);
			do {
				c = bGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					bRetract(sourceBuffer);
					//return currentToken;
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '?' && c != CHARSEOF0 && c != CHARSEOF255);
			break;
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

			/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR)
				bRetract(sourceBuffer);
			lexEnd = bGetReadPos(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bCreate((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			bAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			bDestroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

carthage_int nextState(carthage_int state, carthage_chr c) {
	carthage_int col;
	carthage_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */

carthage_int nextClass(carthage_chr c) {
	carthage_int val = -1;
	switch (c) {
	/* Underscore */
	case CHRCOL2:
		val = 2;
		break;
	/* Variable Identifier/Delimiter # */
	case CHRCOL3:
		val = 3;
		break;
	/* String Literal " */
	case CHRCOL4:
		val = 4;
		break;
	/* Integer Delimiter # */
	case CHRCOL5:
		val = 5;
		break;
	/* Comment/ Char Delimiter ? */
	case CHRCOL6:
		val = 6;
		break;
	/* Float Delimiter $ */
	case CHRCOL7:
		val = 7;
	/* String Delimiter @ */
	case CHRCOL8:
		val = 8;
		break;
	/* Period . */
	case CHRCOL9:
		val = 9;
		break;
	/* Char Literal ' */
	case CHRCOL10:
		val = 10;
		break;
	/* SEOF */
	case CHARSEOF0:
	case CHARSEOF255:
		val = 11;
		break;
	default:
		/*	[A-z](0) */
		if (isalpha(c))
			val = 0;
		/* [0-9](1) */
		else if (isdigit(c))
			val = 1;
		/* Other */
		else
			val = 12;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(carthage_chr lexeme[]) {
	Token currentToken = { 0 };

	size_t length = strlen(lexeme);
	carthage_chr lastCh = (char)lexeme[length - 1];
	int isID = CARTHAGE_FALSE;
	switch (lastCh) {
	case KEYMARK:
		lexeme = lexeme + 1; /* removes first character(#) */ 
		lexeme[strlen(lexeme) - 1] = '\0'; /* removes last character(#) */ 
		/* Remove Earmarks and pass lexeme string in between to keyword identifier func. */ 
		currentToken = funcKEY(lexeme);
		break;
	default:
		/* Error Case */
		currentToken.code=ERR_T;
		break;
	}
	/*if (isID == CARTHAGE_TRUE) {

		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}*/
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcSL(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	/* Add \0 to end of lexeme to mark end of string */
	carthage_chr end = '\0';
	strncat(lexeme, &end, 1);
	/* Set i and string length for iteration control */
	carthage_int i = 0, len = (carthage_int)strlen(lexeme);
	
	/* Get location where string should be written to table  */
	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	/* Iterate String Content  */
	for (i = 1; i < len - 1; i++) {
		/* Case when line number should be incremented */
		if (lexeme[i] == '\n')
			line++;
		/* If char cannot be added throw run time error */
		if (!bAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	/* If char cannot be added throw run time error */
	if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	/* Assign Token Code */
	currentToken.code = STR_T;
	return currentToken;
}



/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	carthage_int kwindex = -1, j = 0;
	/* For loop to iterate keyword table */
	for (j = 0; j < KWT_SIZE; j++)
		/* Compare lexeme string against keyword table */
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if ((kwindex != -1)) {
		/* Assign token code and id lexeme to lexeme */
		if (kwindex == 0) {
			currentToken.code = KW_P_T;
		}
		else {
			currentToken.code = KW_T;
		}
		
		strcpy(currentToken.attribute.idLexeme, lexeme);
		currentToken.attribute.codeType = kwindex;
		
		
	}
	/* Otherwise if no match found in string table pass to error handler func */
	else {
		currentToken = funcErr(lexeme);
	}
	/* Otherwise if no match found in string table pass to error handler func */
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	carthage_int i = 0, len = (carthage_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

carthage_nul printToken(Token t) {
	extern carthage_chr* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case CVID_T: 
		printf("CVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case IVID_T:
		printf("IVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case FVID_T:
		printf("FVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case SVID_T:
		printf("SVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case CHR_T:
		printf("CHR_T\t\t%c\n", t.attribute.charValue);
		break;
	case INT_T:
		printf("INT_T\t\t%d\n", t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", bGetContent(stringLiteralTable,
			(short)t.attribute.codeType));
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case KW_P_T:
		printf("KW_P_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case ARIT_T:
		printf("ARIT_T\t\t%c\n", t.attribute.charValue);
		break;
	case RELA_T:
		printf("RELA_T\t\t%c\n", t.attribute.charValue);
		break;
	case LOGI_T:
		printf("LOGI_T\t\t%c\n", t.attribute.charValue);
		break;
	case COMA_T:
		printf("COMA_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}

}



/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/

/*
***********************************************************
* Function name: varID
* Purpose: To identify and classify valid variable identifier patterns.
* Author: Megan Machkouri
* History/Versions: Ver 2.1.1
* Called functions: strlen(), strncpy()
* Parameters:
*   lexeme = char string
* Return value: currentToken
* Algorithm: Identification of variable identifier type according to lexeme suffix.
*************************************************************
*/
Token varID(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	/* Get Delimiter */
	char lastch = lexeme[length - 1];
	int isID = CARTHAGE_FALSE;
	/* Assign to appropriate variable identifier according to delimiter */
	switch (lastch) {
	case CVIDPREFIX: /* Character Delimiter */
		currentToken.code = CVID_T;
		isID = CARTHAGE_TRUE;
		break;
	case KEYMARK: /* Integer Delimiter */
		currentToken.code = IVID_T;
		isID = CARTHAGE_TRUE;
		break;
	case FVIDPREFIX: /* Float Delimiter */
		currentToken.code = FVID_T;
		isID = CARTHAGE_TRUE;
		break;
	case SVIDPREFIX: /* String Delimiter */
		currentToken.code = SVID_T;
		isID = CARTHAGE_TRUE;
		break;
	default:
		/* Invalid Delimiter */
		currentToken.code = ERR_T;
		break;
	}
	/* If it is a valid delimiter */
	if (isID == CARTHAGE_TRUE) {
		/* Copy variable identifier string content */
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		/* Add \0 to end */
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}


 /*
 ***********************************************************
 * Function name: funcCL
 * Purpose: Function responsible to identify CL (character literals).
 * Author: Megan Machkouri
 * History/Versions: Ver 2.1.1
 * Called functions: null
 * Parameters:
 *   lexeme = char string
 * Return value: currentToken
 * Algorithm: Basic error and boundary checks in order to identify valid CL.
 *************************************************************
 */
Token funcCL(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	/* Convert to integer val for range check  */
	int intV = (int)(lexeme[1]);
	/* ASCII Boundary Check and no whitespace token */
	if (intV < 0 || intV > 255 || isspace(lexeme[1])) {
		/* Error State */
		currentToken = (*finalStateTable[ES])(lexeme);
	}

	else {
		/* Assign token code + attribute */
		currentToken.code = CHR_T;
		currentToken.attribute.charValue = lexeme[1];
	}
	return currentToken;
}

/*
 ***********************************************************
 * Function name: funcCOM
 * Purpose: Function responsible to identify COM (comments).
 * Author: Megan Machkouri
 * History/Versions: Ver 2.1.1
 * Called functions: strlen(), b
 * Parameters:
 *   lexeme = char string
 * Return value: currentToken
 * Algorithm: Basic error and boundary checks in order to identify valid comments.
 *************************************************************
 */
Token funcCOM(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	int i = 1;
	carthage_int len = (carthage_int)strlen(lexeme);
	carthage_chr c = (char)lexeme[i];
	do {
		/* Check lexeme content */
		if (c == CHARSEOF0 || c == CHARSEOF255) {
			/* Assign as COM */
			currentToken.code = COM_T;
			return currentToken;
		}
		/* Newline Case */
		else if (c == '\n') {
			line++;
		}
		currentToken.code = COM_T;
		/* Increment i (iteration control) */
		i++;
		/* Continue until delimiter detected, invalid char or length of lexeme exceeded */
	} while (c != '?' && c != CHARSEOF0 && c != CHARSEOF255 && i < len - 1);

	return currentToken;
}


/*
***********************************************************
* Function name: funcIL
* Purpose: Function responsible to identify IL (integer literals).
* Author: Megan Machkouri
* History/Versions: Ver 2.1.1
* Called functions: strlen() & atoi()
* Parameters:
*   lexeme = char string
* Return value: currentToken
* Algorithm: Basic error and boundary checks in order to identify valid IL.
*************************************************************
*/
Token funcIL(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	/* Error Check */
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		/* Assign to Error State */
		currentToken = (*finalStateTable[ES])(lexeme);
	}

	else {
		/* Convert lexeme to int */
		carthage_int integerL = atoi(lexeme);
		/* Boundary Check */
		if (integerL < 0 || integerL > SHRT_MAX) {
			/* Assign to Error State */
			currentToken = (*finalStateTable[ES])(lexeme);
		}
		else {
			/* Assign code + attribute */
			currentToken.code = INT_T;
			currentToken.attribute.intValue = integerL;
		}

	}
	return currentToken;
}

/*
***********************************************************
* Function name: funcFPL
* Purpose: Function responsible to identify FPL (floating point literals).
* Author: Megan Machkouri
* History/Versions: Ver 2.1.1
* Called functions: strlen() & atof()
* Parameters:
*   lexeme = char string
* Return value: currentToken
* Algorithm: Basic error and boundary checks in order to identify valid FPL.
*************************************************************
*/
Token funcFPL(carthage_chr lexeme[]) {
	Token currentToken = { 0 };
	/* Error Check */
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		/* Assign to Error State */
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		/* Convert lexeme to float */
		carthage_flt floatL = (carthage_flt)atof(lexeme);
		/* Boundary Check */
		if (floatL < 0 || floatL > SHRT_MAX) {
			/* Assign to Error State */
			currentToken = (*finalStateTable[ES])(lexeme);
		}
		else {
			/* Assign code + attribute */
			currentToken.code = FPL_T;
			currentToken.attribute.floatValue = floatL;
		}
	}
	return currentToken;
}
/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
