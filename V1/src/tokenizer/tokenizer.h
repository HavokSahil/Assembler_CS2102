/******************************************************
 *	Tokenizer
 *
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

/*----------------------------------------
 *	Types and definations for Tokenizer
 *----------------------------------------*/

#define _TOK_TYPE_COMMENT 	0xF1	/*   */
#define _TOK_TYPE_LABEL 		0xF2	/*   */
#define _TOK_TYPE_MNENOMIC 	0xF3	/*   */
#define _TOK_TYPE_OPERATOR	0xF4	/*   */
#define _TOK_TYPE_DIRECTIVE	0xF5	/*   */
#define _TOK_TYPE_BLANK			0xF6	/*   */

/*----------------------------------------
 *	Data structures for Tokenizer
 *----------------------------------------*/

/* The structure for tokens  */
struct tk_token {
	AType type;								/*   */
	AString token;						/*   */
	struct tk_token*  next;		/*   */

	void (*destroy)(struct tk_token*);
};

typedef struct tk_token Token;

/* The Structure for Jar/List of Tokens for a line.  */
struct tk_jar {
	ASize size;								/*   */
	Token* tokens;						/*   */

	void (*destroy)(struct tk_jar*);
	AErr (*put)(struct tk_jar*, Token*);
	Token* (*get)(struct tk_jar*, ASize);
};

typedef struct tk_jar Jar;


/* The structure for packets in the cargo  */
struct tk_packet {
	void* content;						/*   */
	struct tk_packet* prev;		/*   */
	struct tk_packet* next;		/*   */
	
	void (*destroy)(struct tk_packet*, ABool);
	AString (*get)(struct tk_packet*);
};

typedef struct tk_packet Packet;


/* The structure for Cargo/List of Jar/List of for complete file */
struct tk_cargo {
	ASize size;	/* Reflects the number of packet in the Cargo   */
	ASize crate_num;					/*   */
	ASize crate_size;					/*   */
	Packet* front;						/*   */
	Packet* back;							/*   */
	AType status;							/*   */

	void (*destroy)(struct tk_cargo*, ABool);
	void* (*get)(struct tk_cargo*, ASize);
	AErr (*load)(struct tk_cargo*, void*);
};

typedef struct tk_cargo Cargo;

/**
 * The Main Structure of Tokenizer 
 * ------------------------------*/
struct tk_tokenizer_interface {
	FILE* file;
	Cargo* cargo;
	ASize file_size;
	ASize current_pos;
	ASize num_workers;
	AType status;
	
	void (*destroy)(struct tk_tokenizer_interface*);
	AErr (*fillCargo)(Cargo*);
	AInt (*percentageWorkDone)(void);
};

typedef struct tk_tokenizer_interface TokInterface;

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
Token* tk_new_Token(AType, AString);				/*   */
void tk_destroy_Token(Token*);							/*   */

/**
 * Functions for Jar Data Structure
 * ---------------------------------------*/
Jar* tk_new_Jar();													/*   */
void tk_destroy_Jar(Jar*);									/*   */
AErr tk_Jar_put(Jar*, Token*);							/*   */
Token* get(const Jar*, ASize);							/*   */

/**
 * Functions for Cargo Data Structure
 * ---------------------------------------*/
Cargo* tk_new_Cargo();											/*   */
void tk_destroy_Cargo(Cargo*, ABool);				/*   */
AErr tk_Cargo_load(Cargo*, void*);					/*   */
void* tk_Cargo_get(const Cargo*, ASize);		/*   */

/**
 * Functions for Packet Data Structure
 * ---------------------------------------*/
Packet* tk_new_Packet();										/*   */
void tk_destroy_Packet(Packet*, ABool);			/*   */
AString tk_Packet_get(const Packet*);				/*   */


#endif
