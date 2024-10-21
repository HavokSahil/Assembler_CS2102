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

#include "tokenizer.h"


/* ***************************************************
 * Functions for Data Structures
 * ***************************************************/

/**
 * Functions for Token Data Structure
 * ---------------------------------------*/

void tk_destroy_Token(Token* tk) {
	if (tk == NULL)
		return;

	if (tk->token != NULL)
		free(tk->token);

	free(tk);
	tk = NULL;
}

Token* tk_new_Token(AType type, AString token) {
	Token* tk = (Token*)malloc(sizeof(Token));
	if (tk == NULL)
		return NULL;

	tk->type = type;
	tk->token = token;

	tk->destroy = tk_destroy_Token;

	return tk;
}

/**
 * Functions for Jar Data Structure
* ---------------------------------------*/

void tk_destroy_Jar(Jar* jar) {
	if (jar == NULL)
		return;

	int i;
	Token* prev = NULL;
	Token* cur = jar->tokens;
	for (i = 0; i<(jar->size); i++) {
		prev = cur->next;
		prev->destroy(prev);
	}

	free(jar);
	jar = NULL;
}

AErr tk_Jar_put(Jar* jar, Token* token) {
	if (jar == NULL)
		return ERR_INV_PTR;

	if (token == NULL)
		return SUCCESS;	/* Implement warning code instead (v1)   */
	
	int i;
	Token* cur = jar->tokens;
	if (cur = NULL) {
		jar->tokens = token;
		return SUCCESS;
	}

	token->next = jar->tokens;
	jar->tokens = token;
	jar->size += 1;

	return SUCCESS;
}

Token* tk_Jar_get(const Jar* jar, ASize index) {
	if (jar == NULL)
		return ERR_INV_PTR;
	if ((index>=jar->size) || (index<0))
		return NULL;

	index = jar->size-1-index;
	Token* cur = jar->tokens;
	int i;
	for (i = 0; i<index; i++) {
		if (cur == NULL)
			return ERR_INV_PTR;
		cur = cur->next;
	}

	return cur;
}

Jar* tk_new_Jar() {
	Jar*  jar = (Jar*)malloc(sizeof(Jar));
	if (jar == NULL)
		return NULL;

	jar->size = 0;
	jar->tokens = NULL;

	jar->destroy = tk_destroy_Jar;
	jar->put = tk_Jar_put;
	jar->get = tk_Jar_get;

	return jar;
}

/**
 * Functions for Packet Data Structure
 * ---------------------------------------*/

void tk_destroy_Packet(Packet* packet, ABool erase_content) {
	if (packet ==  NULL)
		return;

	if (erase_content == TRUE) {
		if (packet->content != NULL)
			free(packet->content);
	}

	free(packet);
	packet = NULL;
}

AString tk_Packet_get(const Packet* packet) {
	if (packet == NULL)
		return NULL;

	return (AString)(packet->content);
}

Packet* tk_new_Packet(void* content) {
	Packet* packet = (Packet*)malloc(sizeof(Packet));
	if (packet == NULL)
		return NULL;

	packet->content = content;
	packet->next = NULL;
	packet->destroy = tk_destroy_Packet;
	
	return packet;
}

/**
 * Functions for Cargo Data Structure
 * ---------------------------------------*/

void tk_destroy_Cargo(Cargo* cargo, ABool erase_content) {
	if (cargo == NULL)
		return;

	if (cargo->size > 0) {	/* Destroy all the packets */
		int i;
		Packet* prev = NULL;
		Packet* cur = cargo->front;
		for (i = 0; i<cargo->size; i++) {
			prev = cur;
			if (prev == NULL)
				break;
			cur = cur->next;
			prev->destroy(prev, erase_content);
		}
	}

	free(cargo);
	cargo = NULL;
}

AErr tk_Cargo_load(Cargo* cargo, void* stuff) {
	if (cargo == NULL)
		return ERR_INV_PTR;

	if (stuff == NULL)
		return SUCCESS;	/* Add Warning instead for Null pointer insertion  */

	Packet* packet = tk_new_Packet(stuff);
	if (packet == NULL)
		return ERR_ALLOC_FAILURE;

	if (cargo->front == NULL) {
		cargo->front = packet;
		cargo->back = packet;
		cargo->size = 1;

		return SUCCESS;
	}

	packet->prev = cargo->back;
	cargo->back->next = packet;
	cargo->back = packet;
	cargo->size += 1;

	return SUCCESS;
}

void* tk_Cargo_get(const Cargo* cargo, ASize index) {
	if (cargo == NULL)
		return NULL;

	if ((index>=cargo->size) || (index<0))
		return NULL;
	
	if (cargo->front == NULL)
		return NULL;

	Packet* cur = cargo->front;
	int i;
	for (i = 0; i<index; i++) {
		if (cur == NULL)
			return NULL;
		cur = cur->next;
	}

	return cur->content;
}

Cargo* tk_new_Cargo() {
	Cargo* cargo = (Cargo*)malloc(sizeof(Cargo));
	if (cargo == NULL)
		return NULL;

	cargo->destroy = tk_destroy_Cargo;
	cargo->load = tk_Cargo_load;
	cargo->get = tk_Cargo_get;

	return cargo;
}


/* ***************************************************
 * Functions for Tokenizer
 * ***************************************************/

AErr tk_TokInterface_fillCargo(TokInterface* tokInterface, Cargo* cargo) {
	/* Write the code here   */
	return 0;
}

AInt tk_TokInterface_pwDone(TokInterface* tokInterface) {
	if (tokInterface == NULL)
		return -1;

	return (AInt)((tokInterface->current_pos * 100)/tokInterface->file_size);
}

void tk_destroy_TokInterface(TokInterface* tokInterface) {
	if (tokInterface == NULL)
		return;

	free(tokInterface);
	tokInterface = NULL;
}

TokInterface* tk_new_TokInterface(FILE* file, ASize num_workers) {
	if (file == NULL)
		return NULL;

	TokInterface* ti = (TokInterface*)malloc(sizeof(TokInterface));
	if (ti == NULL)
		return NULL;

	ti->file = file;
	fseek(file, 0, SEEK_END);
	ti->file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	ti->current_pos = 0;
	ti->num_workers = num_workers;
	ti->status = 0;

	ti->destroy = tk_destroy_TokInterface;
	ti->fillCargo = tk_TokInterface_fillCargo;
	ti->percentageWorkDone = tk_TokInterface_pwDone;

	return ti;
}
