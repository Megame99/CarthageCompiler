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
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: Megan Machkouri
* Course: CST 8152 – Compilers, Lab Section:013
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define CARTHAGE_TRUE	1
#define CARTHAGE_FALSE 0

/* You should add your own constant definitions here */
#define CARTHAGE_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
/* TO_DO: Define your typedefs */
typedef char			carthage_chr;
typedef short			carthage_int;
typedef float			carthage_flt;
typedef void			carthage_nul;

typedef unsigned char	carthage_bol;
typedef unsigned char	carthage_flg;

typedef long			carthage_lng;
typedef double			carthage_dbl;

/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
carthage_int mainBuffer	(carthage_int argc, carthage_chr** argv);
carthage_int mainScanner   (carthage_int argc, carthage_chr** argv);
/*

carthage_int mainParser	(carthage_int argc, carthage_chr** argv);
*/
#endif
