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
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section: 013
* Assignment: A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */

static Token lookahead;
carthage_int matchType;
carthage_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern carthage_int line;
extern Token tokenizer();
extern carthage_chr* keywordTable[];

#define STR_LANGNAME "Carthage"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	prime,
	VAR,
	RUN,
	YIELD,
	STRING,
	IF,
	STOP,
	EL,
	WHILE,
	DO,
	LOOP,
	output,
	input
};

/* Function definitions */
carthage_nul startParser();
carthage_nul matchToken(carthage_int, carthage_int);
carthage_nul syncErrorHandler(carthage_int);
carthage_nul printError();

/* TODO: Place ALL non-terminal function declarations */
carthage_nul assignmentExpression();
carthage_nul assignmentStatement();
carthage_nul runSession();
carthage_nul varSession();
carthage_nul yieldSession();
carthage_nul optVarListDeclarations();
carthage_nul optVarListChecker();
carthage_nul optionalStatements();
carthage_nul outputStatement();
carthage_nul inputStatement();
carthage_nul outputVariableList();
carthage_nul inputVariableList();
carthage_nul program();
carthage_nul statement();
carthage_nul statements();
carthage_nul statementsPrime();
carthage_nul expressionId();
carthage_nul expressionIdPrime();
carthage_nul optionalReturnStatement();
#endif
