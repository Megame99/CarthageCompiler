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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: A32
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TODO: This is the function to start the parser - check your program definition */

carthage_nul startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TODO: This is the main code for match - check your definition */
carthage_nul matchToken(carthage_int tokenCode, carthage_int tokenAttribute) {
	carthage_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TODO: This is the function to handler error - adjust basically datatypes */
carthage_nul syncErrorHandler(carthage_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TODO: This is the function to error printing - adjust basically datatypes */
carthage_nul printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case KW_P_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case COM_T:
		printf("%s%s\n", STR_LANGNAME, ": Program parsed");
		break;
	case '?':
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> CARTHAGE { <opt_statements> }
 * FIRST(<program>)= {KW_T (prime)}.
 ***********************************************************
 */
carthage_nul program() {
	switch (lookahead.code) {
		/*  Main program identifier */
	case KW_P_T:
		if (strncmp(keywordTable[lookahead.attribute.codeType], "prime", 5) == 0) {
			/*  Main program Structure */
			matchToken(KW_P_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			varSession(); /* VAR */
			runSession(); /* RUN */
			yieldSession(); /*  YIELD */
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	case '?':
	case COM_T:
		printf("%s%s\n", STR_LANGNAME, ": Com parsed");
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ***********************************************************
 */
carthage_nul varSession() {
	matchToken(KW_T, VAR); /* Match Key */
	optVarListDeclarations(); /*  Check optional declarations/initializations */
	printf("%s%s\n", STR_LANGNAME, ": Var Session parsed");
}

/*
 ************************************************************
 * optVarListCheck
 * Helper Method to allow for identification of variable lists
 * FIRST(<program>)= {KW_T (DATA)}.
 ***********************************************************
 */
carthage_nul optVarListChecker(carthage_int tokenCode) {
	/* In case of varlist prohibit initialization of variables */
	carthage_int initializeFlag = 1;
	/*  Check for initialization operator  */
	if ((lookahead.code == ASSI_T) && (initializeFlag == 1)) {
		matchToken(ASSI_T, NO_ATTR);
		/*  Check if initialized to corresponding datatype */
		if (lookahead.code == tokenCode) {
			matchToken(tokenCode, NO_ATTR);
			/*  Next token must be EOS */
			if (lookahead.code == EOS_T) {
				matchToken(EOS_T, NO_ATTR);
				printf("%s\n", "Var Initialization Parsed");
				optVarListDeclarations();
			}
			/*  Throw Error   */
			else {
				printError();

			}
		}
		/*  Throw Error if incorrect datatype */
		else {
			printError();
		}
	}
	/*  End of statement */
	else if (lookahead.code == EOS_T) {
		matchToken(EOS_T, NO_ATTR);
		printf("%s\n", "Declaration Parsed");
		optVarListDeclarations();
	}
	/* Comma case */
	else if (lookahead.code == COMA_T) { 
		matchToken(COMA_T, NO_ATTR);
		printf("%s\n", "Comma Parsed");
		/*  Set Flag if comma found  */
		initializeFlag = 0;
		if (lookahead.code == EOS_T) {
			printError();
			
		}

		optVarListDeclarations();
	
	}
	/*  Error */
	else {
		printError();
		
	}
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, CVID, FVID, SVID, IVID}.
 ***********************************************************
 */
carthage_nul optVarListDeclarations() {
	/*  Check for variable id's  */
	switch (lookahead.code) {
		/*  Int Var */
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output list (int identifier) parsed");
		/*  Call helper */
		optVarListChecker(INT_T);
		break;
		/*  Char Var */
	case CVID_T:
		matchToken(CVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output list (char identifier) parsed");
		/*  Call helper */
		optVarListChecker(CHR_T);
		break;
		/*  String Var */
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output list (string identifier) parsed");
		/*  Call helper */
		optVarListChecker(STR_T);
		break;
		/*  Float Var */
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output list (float identifier) parsed");
		/*  Call helper */
		optVarListChecker(FPL_T);
		break;
		/*  Empty Case */
	default:
		printf("%s\n", "Carthage: No variables declared");
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> RUN { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (RUN)}.
 ***********************************************************
 */
carthage_nul runSession() {
	matchToken(KW_T, RUN);
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Run Session parsed");
}

/*
 ************************************************************
 * yieldSession statement
 * BNF: <codeSession> -> YIELD { <opt_statement> }
 * FIRST(<codeSession>)= {KW_T (YIELD)}.
 ***********************************************************
 */
carthage_nul yieldSession() {
	matchToken(KW_T, YIELD);
	optionalReturnStatement();
	printf("%s%s\n", STR_LANGNAME, ": Yield Session parsed");
	
}


/* TODO_205: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, CVID,
	 KW_T(output), KW_T(input) }
 ***********************************************************
 */
carthage_nul optionalStatements() {
	switch (lookahead.code) {

	case KW_T:
		/* Input Statement */
		if ((strncmp(keywordTable[lookahead.attribute.codeType], "output", 6) == 0)) {
			statements();

		}
		/* Output Statement */
		else if ((strncmp(keywordTable[lookahead.attribute.codeType], "input", 5) == 0)) {
			statements();

		}
		break;
		/* Int Var */
	case IVID_T:
		printf("%s%s\n", STR_LANGNAME, ": found an int");
		assignmentStatement();
		break;
		/* Float Var */
	case FVID_T:
		printf("%s%s\n", STR_LANGNAME, ": found a float");
		assignmentStatement();
		break;
		/* String Var */
	case SVID_T:
		printf("%s%s\n", STR_LANGNAME, ": found a string");
		assignmentStatement();
		break;
		/* Char Var */
	case CVID_T:
		printf("%s%s\n", STR_LANGNAME, ": found a char");
		assignmentStatement();
		break;

	default:
		break; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
carthage_nul statements() {	
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		CVID, KW_T(input), KW_T(output) }
 ***********************************************************
 */
carthage_nul statementsPrime() {
	switch (lookahead.code) {
	case KW_T:
		/* Output Statement */
		if (strncmp(keywordTable[lookahead.attribute.codeType], "output", 6) == 0) {
			outputStatement();
		}
		/* Input Statement */
		else if (strncmp(keywordTable[lookahead.attribute.codeType], "input", 5) == 0) {
			inputStatement();
		}
		break;
		/* Int Var */
	case IVID_T:
		assignmentStatement();
		break;
		/* Float Var */
	case FVID_T:
		assignmentStatement();
		break;
		/* String Var */
	case SVID_T:
		assignmentStatement();
		break;
		/* Char Var */
	case CVID_T:
		assignmentStatement();
		break;
		/* Otherwise throw Error */
	default:
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement P parsed");
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, CVID_T
 *			KW_T(output), KW_T(input) }
 ***********************************************************
 */
carthage_nul statement() {
	switch (lookahead.code) {
	case KW_T:
		/* Output Statement */
		if (strncmp(keywordTable[lookahead.attribute.codeType], "output", 6) == 0) {
			outputStatement();
		}
		/* Input Statement */
		else if(strncmp(keywordTable[lookahead.attribute.codeType], "input", 5) == 0) {
			inputStatement();
		}
		break;
		/* Int Var */
	case IVID_T:
		assignmentStatement();
		break;
		/* Float Var */
	case FVID_T:
		assignmentStatement();
		break;
		/* String Var */
	case SVID_T:
		assignmentStatement();
		break;
		/* Char Var */
	case CVID_T:
		assignmentStatement();
		break;
		/* Otherwise throw Error */
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> <assignment expression>
 * FIRST(<assignment statement>) = { IVID_T, FVID_T, SVID_T, CVID_T,
 *		 }
 ***********************************************************
 */
carthage_nul assignmentStatement() {
	/* Method to Identify Arithemtic Expressions */
	assignmentExpression();
	/* Continue until EOS */
	while (lookahead.code != EOS_T) {
		/* Helper Methods to classify */
		expressionId();
		expressionIdPrime();
	}
	/* End of statement */
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
	optionalStatements();
}

/*
 ************************************************************
 * Expression Identifier Prime
 * BNF: <expression> -> <additive> | multiplicaive .. 
 * FIRST(<assignment statement>) = { IVID_T, FVID_T,
 *		}
 ***********************************************************
 */
carthage_nul expressionIdPrime() {
	/* Integer Cases (Check flag)*/
	if ((matchType == INT_T) | (matchType == IVID_T)) 
	{
		/* Assign to corresponding TOKEN */
		if (lookahead.code == (IVID_T)) {
			matchToken(IVID_T, NO_ATTR);
			
		}
		else if (lookahead.code == INT_T) {
			matchToken(INT_T, NO_ATTR);
			
		}
		/* Error Case */
		else {
			printError();
		}
	}
	/* Float Cases */
	else if ((matchType == FPL_T) | (matchType == FVID_T))
	{
		/* Assign to corresponding TOKEN */
		if (lookahead.code == (FVID_T)) {
			matchToken(FVID_T, NO_ATTR);
		
		}
		
		else if (lookahead.code == FPL_T) {
			matchToken(FPL_T, NO_ATTR);
		
		}
		/* Error Case */
		else {
			printError();
		}
	}
	/* Error Case */
	else 
	{
		printError();
	}
}

/*
 ************************************************************
 * Expression Idenitifier
 * BNF: <expression> -> <additive> | multiplicaive ..
 * FIRST(<assignment statement>) = { IVID_T, FVID_T,
 *		}
 ***********************************************************
 */
carthage_nul expressionId() {
	/* Integer Cases (Check flag)*/
	if ((matchType == IVID_T) | (matchType == INT_T))
	{
		/* Perform Check to determine operator type */
		if (lookahead.code == ARIT_T) {
			if (lookahead.attribute.charValue == '*') {
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '/') {
				printf("%s%s\n", STR_LANGNAME, ": Division Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '+') {
				printf("%s%s\n", STR_LANGNAME, ": Additive Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '-') {
				printf("%s%s\n", STR_LANGNAME, ": Subtractive Expression Parsed");
			}
			/* MATCH TOKEN */
			matchToken(ARIT_T, NO_ATTR);
			
		}
		/* Error Case */
		else {
			printError();
		}
	}
	/* Float Cases (Check flag)*/
	else if ((matchType == FVID_T) | (matchType == FPL_T))
	{
		/* Perform Check to determine operator type */
		if (lookahead.code == ARIT_T) {
			if (lookahead.attribute.charValue == '*') {
				printf("%s%s\n", STR_LANGNAME, ": Multiplicative Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '/') {
				printf("%s%s\n", STR_LANGNAME, ": Division Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '+') {
				printf("%s%s\n", STR_LANGNAME, ": Additive Expression Parsed");
			}
			else if (lookahead.attribute.charValue == '-') {
				printf("%s%s\n", STR_LANGNAME, ": Subtractive Expression Parsed");
			}
			/* MATCH TOKEN */
			matchToken(ARIT_T, NO_ATTR);
			
		}
		/* Error Case */
		else {
			printError();
		}
	}
	/* Error Case */
	else
	{
		printError();
	}
}


/*
 ************************************************************
 * Assignment Expression
 * BNF: <assignment expression> -> <integer_variable> = <arithmetic expression>
 *						| <float_variable> = <arithmetic expression>
 *						| <string_variable>= <string expression>
 * FIRST(<assignment expression>) = { IVID_T, FVID_T, SVID_T }
 ***********************************************************
 */
carthage_nul assignmentExpression() {
	switch (lookahead.code) {
		/* Int Var */
	case IVID_T:
		/* Match var plus op */
		matchToken(IVID_T, NO_ATTR);
		matchToken(ASSI_T, NO_ATTR);
		/* Switch and check for correct right hand value */
		switch (lookahead.code) {
			/* Int Lit/Var Acceptable*/
		case IVID_T:
			matchToken(IVID_T, NO_ATTR);
			/* Set Flag */
			matchType = IVID_T;
			break;
		case INT_T:
			matchToken(INT_T, NO_ATTR);
			/* Set Flag */
			matchType = INT_T;
			break;
			/* Throw Error */
		default:
			matchToken(lookahead.code, NO_ATTR);
			/* Set Flag */
			matchType = ERR_T;
			printError();
			break;
		}
		break;
		/* Float Var */
	case FVID_T:
		/* Match var plus op */
		matchToken(FVID_T, NO_ATTR);
		matchToken(ASSI_T, NO_ATTR);
		/* Switch and check for correct right hand value */
		switch (lookahead.code) {
			/* Float Lit/Var Acceptable*/
		case FVID_T:
			matchToken(FVID_T, NO_ATTR);
			/* Set Flag */
			matchType = FVID_T;
			break;
		case FPL_T:
			matchToken(FPL_T, NO_ATTR);
			/* Set Flag */
			matchType = FPL_T;
			break;
			/* Throw Error */
		default:
			matchToken(lookahead.code, NO_ATTR);
			/* Set Flag */
			matchType = ERR_T;
			printError();
			break;
		}
		break;
		/* String Var */
	case SVID_T:
		/* Match var plus op */
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASSI_T, NO_ATTR);
		/* Switch and check for correct right hand value */
		switch (lookahead.code) {
			/* String Lit/Var Acceptable*/
		case SVID_T:
			matchToken(SVID_T, NO_ATTR);
			/* Set Flag */
			matchType = SVID_T;
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			/* Set Flag */
			matchType = STR_T;
			break;
			/* Throw Error */
		default:
			matchToken(lookahead.code, NO_ATTR);
			/* Set Flag */
			matchType = ERR_T;
			printError();
			break;
		}
		break;
		/* Char Var */
	case CVID_T:
		/* Match var plus op */
		matchToken(CVID_T, NO_ATTR);
		matchToken(ASSI_T, NO_ATTR);
		/* Switch and check for correct right hand value */
		switch (lookahead.code) {
			/* Char Lit/Var Acceptable*/
		case CVID_T:
			matchToken(CVID_T, NO_ATTR);
			/* Set Flag */
			matchType = CVID_T;
			break;
		case CHR_T:
			matchToken(CHR_T, NO_ATTR);
			/* Set Flag */
			matchType = CHR_T;
			break;
			/* Throw Error */
		default:
			matchToken(lookahead.code, NO_ATTR);
			/* Set Flag */
			matchType = ERR_T;
			printError();
			break;
		}
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> output(<output statementPrime>);
 * FIRST(<output statement>) = { KW_T(output) }
 ***********************************************************
 */
carthage_nul outputStatement() {
	matchToken(KW_T, output);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/************************************************************
* Input Statement
* BNF: <output statement>->input(<output statementPrime>);
*FIRST(<output statement>) = { KW_T(input) }
***********************************************************
*/
carthage_nul inputStatement() {
	matchToken(KW_T, input);
	matchToken(LPR_T, NO_ATTR);
	inputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, CVID, ϵ }
 ***********************************************************
 */
carthage_nul outputVariableList() {
	switch (lookahead.code) {
		/* String Literal */
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output (string literal) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			outputVariableList();
		}
		break;
		/* Int Var */
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output (int identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			outputVariableList();
		}
		break;
		/* Char Var */
	case CVID_T:
		matchToken(CVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output (char identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			outputVariableList();
		}
		break;
		/* String Var */
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Output (string identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			outputVariableList();
		}
		break;
		/* Float Var */
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE:Output (float identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			outputVariableList();
		}
		break;
		/* Empty Case */
	default:
		printf("%s\n", "CARTHAGE: Output (empty) parsed");
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/*
 ************************************************************
 * Input Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, CVID_T ϵ }
 ***********************************************************
 */
carthage_nul inputVariableList() {
	/* Check var ids */
	switch (lookahead.code) {
		/* Int Var */
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Input list (int identifier) parsed");
		if (lookahead.code == COMA_T) {
			/* Comma Case (List) */
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			inputVariableList();
		}
		break;
		/* Char Var */
	case CVID_T:
		matchToken(CVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Input list (char identifier) parsed");
		if (lookahead.code == COMA_T) {
			/* Comma Case (List) */
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			inputVariableList();
		}
		break;
		/* String Var */
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Input list (string identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			inputVariableList();
		}
		break;
		/* Float Var */
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		printf("%s\n", "CARTHAGE: Input list (float identifier) parsed");
		/* Comma Case (List) */
		if (lookahead.code == COMA_T) {
			matchToken(COMA_T, NO_ATTR);
			printf("%s\n", "Comma Parsed");
			/* Back to top */
			inputVariableList();
		}
		break;
		/* Empty Case */
	default:
		printf("%s\n", "CARTHAGE: Input list (empty) parsed");
		return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Input variable list parsed");
}

/*
 ************************************************************
 * Optional Return Statement
 * BNF: <opt_variable> -> <variable> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T | FVID_T | SVID_T | CVID_T ϵ }
 ***********************************************************
 */
carthage_nul optionalReturnStatement() {
	/*  Check lookahead token for single variable identifier (porgram return var)*/
	switch (lookahead.code) {
		/*  Int Var */
	case IVID_T:
		matchToken(IVID_T, NO_ATTR);
		if (lookahead.code == EOS_T) 
		{
			/*  IF next token is EOS exit cleanly with no error */
			matchToken(EOS_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Program Returns Integer Var");
		}
		else 
		{
			/*  THROW Errrror for anything else  */
			printError();
		}
		
		break;
		/*  Float Var */
	case FVID_T:
		matchToken(FVID_T, NO_ATTR);
		if (lookahead.code == EOS_T)
		{
			/*  IF next token is EOS exit cleanly with no error */
			matchToken(EOS_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Program Returns Integer Var");
		}
		else
		{
			/*  THROW Errrror for anything else  */
			printError();
		}
		break;
		/*  String Var */
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		if (lookahead.code == EOS_T)
		{
			/*  IF next token is EOS exit cleanly with no error */
			matchToken(EOS_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Program Returns Integer Var");
		}
		else
		{
			/*  THROW Errrror for anything else  */
			printError();
		}
		break;
		/*  Char Var */
	case CVID_T:
		matchToken(CVID_T, NO_ATTR);
		if (lookahead.code == EOS_T)
		{
			/*  IF next token is EOS exit cleanly with no error */
			matchToken(EOS_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Program Returns Integer Var");
		}
		else
		{
			/*  THROW Errrror for anything else  */
			printError();
		}
		break;
	default:
		printf("%s%s\n", STR_LANGNAME, ": Program Returns No Variables");
	}
	
}