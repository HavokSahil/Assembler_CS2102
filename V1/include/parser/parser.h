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

/* -------------------------------------------------------
 * Definations for Parser
 * -------------------------------------------------------*/
#define _PSR_STATUS_READY 0xC1
#define _PSR_STATUS_ERR		0xC2

#define _PSR_TYPE_JAR_LABEL				0xC3
#define _PSR_TYPE_JAR_SET_DIRECT	0xC4
#define _PSR_TYPE_JAR_SEC_DIRECT	0xC5
#define _PSR_TYPE_JAR_DAT_DIRECT 	0xC6
#define _PSR_TYPE_JAR_TEX_DIRECT 	0xC7
#define _PSR_TYPE_JAR_INSTRUCTION	0xC8
#define _PSR_TYPE_JAR_DATA_DECL		0xC9
#define _PSR_TYPE_JAR_ERR					0xCA
#define _PSR_TYPE_JAR_LABL_INSTR	0xCB


#define _PSR_ERR_INV_MNEMO				0xD0
#define _PSR_ERR_INV_OPRND				0xD1
#define _PSR_ERR_FMT_OPRND				0xD2
#define _PSR_ERR_FMT_DDATA				0xD3
#define _PSR_ERR_MIS_SETDA				0xD4
#define _PSR_ERR_MIS_OPRND				0xD5
#define _PSR_ERR_MIS_DDATA				0xD6
#define _PSR_ERR_MMT_OPRND				0xD7
#define _PSR_ERR_INV_LABEL				0xD8
#define _PSR_ERR_DUP_LABEL				0xD9
#define _PSR_ERR_INV_JRTYP 				0xDA

#define _PSR_BUFF_OPRND_SIZE			64

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

