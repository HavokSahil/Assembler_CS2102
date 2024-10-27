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
 *
 * 	- (Documentation Left)
 ******************************************************/

#include <tokenizer/tokenizer.h>

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
	tk->lno = UNSET_TOK_LINE;
	tk->cno = UNSET_TOK_COL;

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
		return ERR_TOK_INVALID_JAR;

	if (token == NULL)
		return ERR_TOK_INVALID_JAR;
	
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
		return ERR_TOK_INVALID_JAR;
	if ((index>=jar->size) || (index<0))
		return NULL;

	index = jar->size-1-index;
	Token* cur = jar->tokens;
	int i;
	for (i = 0; i<index; i++) {
		if (cur == NULL)
			return ERR_TOK_INVALID_TOKEN;
		cur = cur->next;
	}

	return cur;
}

Jar* tk_new_Jar(ASize lno) {
	Jar*  jar = (Jar*)malloc(sizeof(Jar));
	if (jar == NULL)
		return NULL;

	jar->size = 0;
	jar->tokens = NULL;
	jar->lno = lno;

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

Packet* tk_new_Packet(void* content, ASize lno) {
	Packet* packet = (Packet*)malloc(sizeof(Packet));
	if (packet == NULL)
		return NULL;

	packet->content = content;
	packet->lno = lno;
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

AErr tk_Cargo_load(Cargo* cargo, ASize lno, void* stuff) {
	if (cargo == NULL)
		return ERR_TOK_INVALID_CARGO;
	if (stuff == NULL)
		return SUCCESS;	/* Add Warning instead for Null pointer insertion  */

	Packet* packet = tk_new_Packet(stuff, lno);
	if (packet == NULL)
		return ERR_TOK_INVALID_PACKET;

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

static Packet* _tk_Cargo_get_packet(const Cargo* cargo, ASize index) {
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

	return cur;
}

void* tk_Cargo_get(const Cargo* cargo, ASize index) {
	Packet* cur = _tk_Cargo_get_packet(cargo, index);
	if (cur == NULL)
		return NULL;

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

static ABool _tk_TokInterface_ignorable_check(AString line) {	/* Returns True if line is all just comments or Black line   */
	if (line == NULL)
		return TRUE;

	ASize length = strlen(line);
	ASize i;

	/* Check for Black Lines  */
	ABool white_space = TRUE;
	for (i = 0; i<length; i++) {
		if (!isspace(line[i]))
			white_space = FALSE;
	}

	if (white_space)
		return TRUE;

	/* Check for All Comment Line   */
	ABool non_comment = FALSE;

	for (i = 0; i<length; i++) {
		if (line[i] == SEPARATOR_COMMENT) {
			if (non_comment == TRUE)
				return FALSE;
			else
				return TRUE;
		}
		if (!((line[i] == ' ') || (line[i] == '\t')))
			non_comment = TRUE;
	}

	return FALSE;
}

static void _tk_Tokenizer_remove_newlines(AString line) {
	if (line == NULL)
		return;

	ASize length = strlen(line);
	ASize i;
	for (i = 0; i<length; i++) {
		if (line[i] == '\n')
			line[i] = ' ';
	}
}

static void _tk_Tokenizer_remove_comment(AString line) {
	if (line == NULL)
		return;

	ASize length = strlen(line);
	ASize i;
	for (i = 0; i<length; i++) {
		if (line[i] == SEPARATOR_COMMENT) {
			line[i] = '\0';
			return;
		}
	}
}

static ABool _tk_Tokenizer_check_label(const AString line) {
	if (line == NULL)
		return FALSE;

	ASize buffSize = strlen(line);
	buffSize = (buffSize+1 >= SZ_TOK_LINE_BUFF)? SZ_TOK_LINE_BUFF: buffSize+1;
	AString buffer = (AString)malloc(buffSize);
	/* Do something about this   */
	if (buffer == NULL)
		return FALSE;

	memcpy(buffer, line, buffSize-1);
	buffer[buffSize - 1] = '\0';

	const AString delim = " \t\n";
	AString token;
	char* save_ptr;
	token = strtok_r(buffer, delim, &save_ptr);
	if (token == NULL) {
		free(buffer);
		return FALSE;
	}

	ASize length = strlen(token);
	if (token[length - 1] == ':') {
		free(buffer);
		return TRUE;
	}

	token = strtok_r(NULL, delim, &save_ptr);
	if (token == NULL) {
		free(buffer);
		return FALSE;
	}
	
	if (token[0] == ':') {
		free(buffer);
		return TRUE;
	}

	return FALSE;
}

/* Remaining: To fix the edge case when line starts with `:`  */
static AErr _tk_Tokenizer_put_tokens(Jar* jar, const AString line) {	/* Function to add non comments tokens into jar   */

	if (jar == NULL)
		return ERR_TOK_INVALID_JAR;

	if (line == NULL)
		return ERR_STR_INVALID_STRING;
	
	ASize buffSize = strlen(line) + 1;
	buffSize = (buffSize > SZ_TOK_LINE_BUFF)? SZ_TOK_LINE_BUFF: buffSize;
	AString buff = (AString)malloc(buffSize);
	/* Do something about this  repeated buffer allocations if possible */
	if (buff == NULL)
		return ERR_MEM_ALLOC_FAIL;

	memcpy(buff, line, buffSize);
	buff[buffSize - 1] = '\0';

	/* Remove comments from the line   */
	_tk_Tokenizer_remove_comment(buff);

	AString delim = "\t :\n,";
	AString token;
	AString save_ptr;
	token = strtok_r(buff, delim, &save_ptr);

	if (token == NULL)
		return ERR_TOK_INVALID_TOKEN;

	ASize col = 1;
	/* Label type token detected. Put it first into the jar   */
	ABool result = _tk_Tokenizer_check_label(line);
	if (result == TRUE) {
		ASize length = strlen(token);
		length = (length+1 >= SZ_TOK_TOKEN_LABEL)? SZ_TOK_TOKEN_LABEL: length+1;
		AString label = (AString)malloc(length);
		memcpy(label, token, length);
		label[length-1] = '\0';	/* Pay careful attention here. Some bug fix might be needed   */

		Token* tkn = tk_new_Token(TYPE_TOK_LABEL, label);
		if (tkn == NULL) {
			free(label);
			return ERR_TOK_INVALID_TOKEN;
		}
		tkn->lno = jar->lno;
		tkn->cno = col++;
		tkn->token = label;
		if (jar->put(jar, tkn) != SUCCESS) {
			tkn->destroy(tkn);
			return ERR_DS_INSERT_FAIL;
		}
		token = strtok_r(NULL, delim, &save_ptr);	/* Handle the case of double `:`   */
	} 

	ABool directive = TRUE;
	/* Add other mnemonics into the jar   */
	while (token != NULL) {
		ASize length = strlen(token);
		length = (length+1 >= SZ_TOK_TOKEN_MNEMONIC)? SZ_TOK_TOKEN_MNEMONIC: length+1;
		AString mnemo = (AString)malloc(length);
		memcpy(mnemo, token, length);
		mnemo[length-1] = '\0';	/* Pay careful attention here. Some bug fix might be needed   */

		Token* tkn = tk_new_Token(TYPE_TOK_WORD, mnemo);
		if (tkn == NULL) {
			free(mnemo);
			return ERR_DS_STRUCT_GEN_FAIL;
		}
		tkn->lno = jar->lno;
		tkn->cno = col++;
		tkn->token = mnemo;
		if (jar->put(jar, tkn) != SUCCESS) {
			tkn->destroy(token);
			return ERR_DS_INSERT_FAIL;
		}
		token = strtok_r(NULL, delim, &save_ptr);
	}

	return SUCCESS;
}

static AErr _tk_TokInterface_loadPackets(TokInterface* ti) {	/* Function to load Lines into Cargo as Packets. */

	if (ti == NULL)
		return ERR_INVALID_INTERFACE;

	FILE* file = ti->file;
	if (file == NULL)
		return ERR_FILE_INVALID_FILE;
	
	ASize lines_read = 0;
	ASize cargo_size = 0;
	AString buff = NULL;
	while (lines_read<SZ_TOK_CARGO_PKT_WIN) {

		if (ti->cargo->size != SZ_TOK_CARGO_PKT_WIN) {
			buff = (AString)malloc(SZ_TOK_LINE_BUFF);
		} else {
			buff = ti->cargo->get(ti->cargo, lines_read);
			if (buff == NULL)
				buff = (AString)malloc(SZ_TOK_LINE_BUFF);
		}

		if (buff == NULL)
			return ERR_MEM_ALLOC_FAIL;

		if (fgets(buff, SZ_TOK_LINE_BUFF, file) == NULL) {	/* If file ends and no content is read.  */
			free(buff);
			buff = NULL;
			break;
		}
		
		if (_tk_TokInterface_ignorable_check(buff) == TRUE) {
			lines_read++;
			continue;
		}
		if (ti->cargo->load(ti->cargo, lines_read + 1, (void*)buff) != SUCCESS)
			return ERR_TOK_CARGO_LOAD_FAIL;

		cargo_size++;
		lines_read++;
	}

	ti->current_pos = ftell(file);
	if (ti->current_pos == ti->file_size)
		ti->status = 1;
	else ti->status = 0;

	ti->cargo->size = cargo_size;

	return SUCCESS;
}

static AString _tk_TokInterface_token_comment(const AString line) {
	if (line == NULL)
		return NULL;

	ASize length = strlen(line);
	int i;
	ABool comment = FALSE;
	char* cur = line;
	for (i = 0; i<length; i++) {
		if (line[i] == SEPARATOR_COMMENT) {
			comment = TRUE;
			break;
		}
		cur = cur+1;
	}
	if (*cur != SEPARATOR_COMMENT)
		return NULL;
	
	length = strlen(cur);
	length = (length >= SZ_TOK_TOKEN_COMMENT)? SZ_TOK_TOKEN_COMMENT-1: length;
	if (comment = FALSE)
		return NULL;

	AString buff = (AString)malloc(SZ_TOK_TOKEN_COMMENT);
	if (buff == NULL)
		return NULL;

	memcpy(buff, cur, length);
	buff[length] = '\0';
	_tk_Tokenizer_remove_newlines(buff);
	return buff;
}

static void* _tk_TokInterface_jar_maker(void* line_ptr, ASize lno) {	/* Function to convert a string buffer to token Returns `NULL` on error  */
	if (line_ptr == NULL)
		return NULL;
	
	AString line = line_ptr;


	/* To be removed   */
	ABool label = _tk_Tokenizer_check_label(line);

	Jar* jar = tk_new_Jar(lno);
	if (jar == NULL)
		return NULL;

	AString string = line;
	/* Find comments and filter out comments   */
	ASize length = strlen(line);

	AString comment = _tk_TokInterface_token_comment(line);
	if (comment != NULL) {

		Token* token = tk_new_Token(TYPE_TOK_COMMENT, comment);	/* Always put token->token = value to since pointer is deallocated from stack   */
		token->token = comment;

		if (token == NULL)
			return NULL;
		if (jar->put(jar, token) != SUCCESS)
			return NULL;
	}

	if (_tk_Tokenizer_put_tokens(jar, line) != SUCCESS)
		return NULL;

	return (void*)jar;
}

static AErr _tk_TokInterface_jarify(TokInterface* ti) {	/* The function to convert the Line Packets into Packets of Jars of Tokens   */

	if (ti == NULL)
		return ERR_INVALID_INTERFACE;

	if (ti->cargo == NULL)
		return ERR_TOK_INVALID_CARGO;

	Cargo* cargo = ti->cargo;

	int i;
	for (i = 0; i<cargo->size; i++) {
		Packet* packet = _tk_Cargo_get_packet(cargo, i);
		if (packet == NULL)
			return ERR_DS_STRUCT_GEN_FAIL;
	
		Jar* jar = _tk_TokInterface_jar_maker(cargo->get(cargo, i), packet->lno);
		if (jar == NULL) {
			return ERR_DS_STRUCT_GEN_FAIL;
		}
		packet->content = (void*)jar;
	}

	return SUCCESS;
}

AErr tk_TokInterface_fillCargo(TokInterface* ti, Cargo* cargo) {
	ti->cargo = cargo;

	if (_tk_TokInterface_loadPackets(ti) != SUCCESS)
		return ERR_TOK_CARGO_LOAD_FAIL;

	if (_tk_TokInterface_jarify(ti) != SUCCESS)
		return ERR_TOK_JARIFICATION_FAIL;

	return SUCCESS;
}

AInt tk_TokInterface_pwDone(TokInterface* ti) {		/* Function to calculate the percentage of work done by the tokenizer   */
	if (ti == NULL)
		return -1;

	return (AInt)((ti->current_pos * 100)/ti->file_size);
}

void tk_destroy_TokInterface(TokInterface* ti) {	/* Function to destroy the Tokenizer Interface   */
	if (ti == NULL)
		return;

	free(ti);
	ti = NULL;
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
