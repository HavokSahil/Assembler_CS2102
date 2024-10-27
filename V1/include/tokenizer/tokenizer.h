/******************************************************
 *	Tokenizer
 *	--------------------------------------------------
 *	This section contains the declaration of data-
 *	structures and functions for string tokenizer.
 *
 *	Tokenizer split the lines into tokens.
 *	Various supported token formats are:
 *	- `comments`: starts with `#`.
 *
 *	- `labels`: word with its first character the first
 *		non-whitespace character of the line and ending
 *		with `:` symbol.
 *	
 *	- `directive`: word starting with a `.` character
 *
 * 	- `begin words`: The words that are neither labels
 * 		nor comments.
 ******************************************************/


#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <common_ds.h>
#include <common_types.h>
#include <err_codes.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

/*----------------------------------------
 *	Data structures for Tokenizer
 *----------------------------------------*/

/* The structure for tokens  */
struct tk_token {
	AType type;								/* Type of Token  */
	ASize lno;								/* The line number of token  */
	ASize cno;								/* The column number of token  */
	AString token;						/* Token Content  */
	struct tk_token*  next;		/* Next Item in Linked List of Token  */

	void (*destroy)(struct tk_token*);
};

typedef struct tk_token Token;

/* The Structure for Jar/List of Tokens for a line.  */
struct tk_jar {
	ASize size;								/* Size of Jar [Number of Tokens in Jar]  */
	Token* tokens;						/* Tokens Linked List  */
	ASize lno;

	void (*destroy)(struct tk_jar*);
	AErr (*put)(struct tk_jar*, Token*);
	Token* (*get)(struct tk_jar*, ASize);
};

typedef struct tk_jar Jar;


/* The structure for packets in the cargo  */
struct tk_packet {
	void* content;						/* The Content of the Packet.  */
	ASize lno;									/* The line number of the packet  */
	struct tk_packet* prev;		/* The Previous Item in the Packet Queue. */
	struct tk_packet* next;		/* The Next item in the Packet Queue. */
	
	void (*destroy)(struct tk_packet*, ABool);
	AString (*get)(struct tk_packet*);
};

typedef struct tk_packet Packet;


/* The structure for Cargo/List of Jar/List of for complete file */
struct tk_cargo {
	ASize size;								/* Reflects the number of packet in the Cargo   */
	ASize crate_num;					/* The Number of Crates in the Cargo  */
	ASize crate_size;					/* The Crate Size: Number of Packets in each Crate.  */
	Packet* front;						/* The Front of the Packet Queue.  */
	Packet* back;							/* The Back of the Packet Queue.   */
	AType status;							/* The Status of the Cargo.  */

	void (*destroy)(struct tk_cargo*, ABool);
	void* (*get)(struct tk_cargo*, ASize);
	AErr (*load)(struct tk_cargo*, ASize, void*);
};

typedef struct tk_cargo Cargo;

/**
 * The Main Structure of Tokenizer 
 * ------------------------------*/

struct tk_tokenizer_interface {
	FILE* file;						/* The File Descriptor of the Tokenizer Interface. */
	Cargo* cargo;					/* The Cargo Pointer. */
	ASize file_size;			/* The Total File Size.  */
	ASize current_pos;		/* The Current Position of the Tokenizer Interface.  */
	ASize num_workers;		/* The Number of Workers in the Tokenizer Interface  */
	AType status;					/* The Status of the Tokenizer Interface.  */
	
	void (*destroy)(struct tk_tokenizer_interface*);
	AErr (*fillCargo)(struct tk_tokenizer_interface*, Cargo*);
	AInt (*percentageWorkDone)(struct tk_tokenizer_interface*);
};

typedef struct tk_tokenizer_interface TokInterface;


int test_tokenizer();

/*----------------------------------------
 *	Main Functions for Tokenizer
 *----------------------------------------*/

TokInterface* tk_new_TokInterface(FILE*, ASize);

/*----------------------------------------
 *	Other Functions for Tokenizer
 *----------------------------------------*/


/**
 * Functions for Token Data Structure
 * ---------------------------------------*/
Token* tk_new_Token(AType, AString);				/* Allocate New Token in Heap.  */
/**
 * Functions for Jar Data Structure
 * ---------------------------------------*/
Jar* tk_new_Jar();													/* Allocate New Jar in Memory.  */
/**
 * Functions for Cargo Data Structure
 * ---------------------------------------*/
Cargo* tk_new_Cargo();											/* Allocate New Cargo in Memory.  */
/**
 * Functions for Packet Data Structure
 * ---------------------------------------*/
Packet* tk_new_Packet(void*, ASize);				/* Allocate New Packet in Memory.  */

#endif
