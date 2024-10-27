/* *******************************************************
 *	Parser (HEADER)
 *	------------------------------------------------------
 *	This section contains tools to check the grammars of
 *	Jars in Cargo (A Line of Tokens for a set of Lines).
 *	It converts the Jars into Instructions and add to In-
 *	struction List.
 *
 *	The rules are defined in structures and grammars and
 *	vocabulary are present in this file.
 *
 *	There is a plan to add the functionality to add exter-
 *	nal configuration file for grammars and vocabulary.
 *
 * *******************************************************/

#ifndef _PARSER_H
#define _PARSER_H

#include <common_types.h>
#include <common_ds.h>
#include <stdio.h>
#include <tokenizer/tokenizer.h>
#include <err_codes.h>

/* -------------------------------------------------------
 * Defining Syntax Rules for Parser
 * -------------------------------------------------------*/


/* -------------------------------------------------------
 * Structures for Parser
 * -------------------------------------------------------*/
	
/* Structure for Parser Interface  */
struct psr_parser_interface_struct {
	Cargo* cargo;				/* The Cargo data structure for unprocessed tokens  */
	IList* ilist;				/* The Instruction List Pointer  */
	EWList* elist;			/* Errow-Warning List Pointer  */
	SymTable* stable;		/* Symbol Table Pointer  */
	DList* dlist;				/* Data List Pointer  */
	MnMap* mnemonic_map;
	RegMap* reg_map;
	FILE* file;					/* Input File Pointer  */
	AType status;				/* Interface Status  */

	void (*destroy)(struct psr_parser_interface_struct*);				/* Deallocate the parser interface from the memory  */
	AErr (*parse)(struct psr_parser_interface_struct*, FILE*);	/* Parser the input file */
};

typedef struct psr_parser_interface_struct ParserInterface;

/* Structure for Jar Tag */
struct psr_jartag_struct {
	AErr exec_code;	/* Execution Code   */
	ASize cno;			/* Column Number   */
};

typedef struct psr_jartag_struct JarTag;


/* Functions for Parser Interface  */
ParserInterface* psr_new_ParserInterface(IList*, EWList*, SymTable*, DList*, MnMap*, RegMap*, FILE*);

#endif

