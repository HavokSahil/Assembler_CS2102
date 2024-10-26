/* *******************************************************
 *	Parser (SOURCE)
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


#include <parser/parser.h>

/**
 * Function for Jar Classifier 
 * ---------------------------------*/
AType psr_jar_classify(Jar* jar) {
	if (jar == NULL)
		return _PSR_TYPE_JAR_ERR;
	
	ASize size = jar->size;
	if (!size)
		return _PSR_TYPE_JAR_ERR;

	ASize cur = 0;
	Token* token = jar->get(jar, cur);
	if (token == NULL)
		return _PSR_TYPE_JAR_ERR;

	if (token->type == _TOK_TYPE_COMMENT) {
		cur++;
		token = jar->get(jar, cur);
		if (token == NULL)
			return _PSR_TYPE_JAR_ERR;
	}
	
	if (token->type == _TOK_TYPE_LABEL) {
	/* Token can be Label, Data Declaration or SET directive   */
		cur++;
		if (jar->get(jar, cur) == NULL) {
	/* Token has no next item, meaning label type  */
			return _PSR_TYPE_JAR_LABEL;
		}
		token = jar->get(jar, cur);
		AString stok = token->token;
		if (strcmp(stok, "SET") == 0)
			return _PSR_TYPE_JAR_SET_DIRECT;
		if (strcmp(stok, "data") == 0)
			return _PSR_TYPE_JAR_DATA_DECL;

		return _PSR_TYPE_JAR_LABL_INSTR;
	}
	/* Token can be section directive or instruction */
	AString stok = token->token;
	if (strcmp(stok, ".data") == 0)
		return _PSR_TYPE_JAR_DAT_DIRECT;
	if (strcmp(stok, ".text") == 0)
		return _PSR_TYPE_JAR_TEX_DIRECT;

	/* Treat all other cases as instruction type: errors handled later  */
	return _PSR_TYPE_JAR_INSTRUCTION;
}

/* Function for Grammar Checking  
 * ---------------------------------*/

static AString _psr_alloc_astring(AString string, ASize max_size) {	/* Function to allocate a string into memory   */
	if (string == NULL)
		return NULL;
	ASize length = strlen(string);
	length = (length+1 > max_size)? max_size: length+1;
	AString buff = (AString)malloc(length);
	memcpy(buff, string, length-1);
	buff[length-1] = '\0';

	return buff;
}

static AErr _psr_insert_error(EWList* elist, ASize lno, ASize cno, AErr code) {
	EWItem* eitem = ds_new_EWItem(lno, cno, code);
	if (eitem == NULL)
		return FAILURE;

	return elist->insert(elist, eitem);
}

static AErr _psr_verify_instruction(AAddr address, IList* ilist, EWList* elist, Jar* jar, MnMap* mnemonic_map) {	/* Function to check and insert instructions into instruction list   */
	if ((jar == NULL) || (elist == NULL) || (ilist == NULL) || (mnemonic_map == NULL))
		return FAILURE;
	if (jar->size == 0)
		return FAILURE;

	ASize cur = 0;
	ASize jsize = jar->size;
	Token* token = jar->get(jar, cur);
	if (token == NULL)
		return FAILURE;

	if (token->type == _TOK_TYPE_COMMENT) {
		jsize-=1;
		cur++;
		token = jar->get(jar, cur);
	}

	if (token == NULL)
		return FAILURE;

	AString stok = token->token;
	if (stok == NULL)
		return FAILURE;

	MnItem* mitem = mnemonic_map->find(mnemonic_map, stok);
	if (mitem == NULL) {
		/* Invalid Mnemonic  */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_INV_MNEMO);
	}
	
	jsize-=1;	/* Operand Count = Total Tokens - 1 # for mnemonic   */
	IItem* iitem = ds_new_IItem(address);
	if (iitem == NULL)
		return FAILURE;

	iitem->opcode = mitem->key;
	iitem->n_op = mitem->n_operand;
	iitem->lno = jar->lno;
	
	if (iitem->n_op != jsize) {
		/* If Number of operands is not equal to required number of operands   */
		iitem->destroy(iitem);
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_MMT_OPRND);
	}

	cur++;	/* Move to the operand tokens   */
	/* Insert operands if required   */
	ASize i;
	for (i = 0; i<iitem->n_op; i++) {
		token = jar->get(jar, cur++);
		AString operand = token->token;
		
		AString buff = _psr_alloc_astring(operand, _PSR_BUFF_OPRND_SIZE);
		if (buff == NULL) {
			iitem->destroy(iitem);
			return FAILURE;
		}
		if (i == 0)
			iitem->operand_1 = buff;
		if (i == 1)
			iitem->operand_2 = buff;
	}

	if (ilist->insert(ilist, iitem) == SUCCESS)
		return SUCCESS;

	iitem->destroy(iitem);
	return FAILURE;
}

static AErr _psr_verify_labels(AAddr address, Jar* jar, SymTable* stable, EWList* elist) {	/* Function to verify labels and add into symbol table   */
	if ((jar == NULL) || (stable == NULL) || (elist == NULL))
		return FAILURE;

	ASize jsize = jar->size;
	ASize cur = 0;
	if (jsize == 0)
		return FAILURE;

	Token* token = jar->get(jar, 0);
	if (token == NULL)
		return FAILURE;

	if (token->type == _TOK_TYPE_COMMENT) {
		cur++;
		token = jar->get(jar, cur);
		jsize--;
	}

	if (token == NULL)
		return FAILURE;
	
	if (jsize != 1) {
		/* Redundant Tokens  */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_MMT_OPRND);
	}

	AString stok = token->token;
	if (stable->find(stable, stok) != INVALID_ADDRESS) {
	/* Duplicate label case   */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_DUP_LABEL);
	}

	/* TO-DO: Add the functionality to filter invalid labels   */
	AString string = _psr_alloc_astring(stok, _PSR_BUFF_OPRND_SIZE);
	if (string == NULL)
			return FAILURE;

	return stable->insert(stable, string, address);
}

static AErr _psr_verify_labl_instr(AAddr address, Jar* jar, IList* ilist, SymTable* stable, EWList* elist, MnMap* mnemonic_map) {
	if ((jar == NULL) || (ilist == NULL) || (stable == NULL) || (elist == NULL))
		return FAILURE;

	ASize cur = 0;
	if (jar->size == 0)
		return FAILURE;

	Token* token = jar->get(jar, cur);
	if (token->type == _TOK_TYPE_COMMENT) {
		cur++;
		token = jar->get(jar, cur);
		if (token == NULL)
			return FAILURE;
	}

	AString label = token->token;
	if (token == NULL) return FAILURE;

	AString string = _psr_alloc_astring(label, _PSR_BUFF_OPRND_SIZE);
	if (string == NULL) return FAILURE;

	if (stable->find(stable, string) != INVALID_ADDRESS) {
		/* Case of invalid label */
		free(string);
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_DUP_LABEL);
	}

	if (stable->insert(stable, string, address) != SUCCESS)
		return FAILURE;

	cur++;	/* Move to the instruction insertion   */
	ASize jsize = jar->size-cur;
	token = jar->get(jar, cur);
	if (token == NULL)
		return FAILURE;

	AString stok = token->token;
	if (stok == NULL)
		return FAILURE;

	MnItem* mitem = mnemonic_map->find(mnemonic_map, stok);
	if (mitem == NULL) {
		/* Invalid Mnemonic  */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_INV_MNEMO);
	}
	
	jsize-=1;	/* Operand Count = Total Tokens - 1 # for mnemonic   */
	IItem* iitem = ds_new_IItem(address);
	if (iitem == NULL)
		return FAILURE;

	iitem->opcode = mitem->key;
	iitem->n_op = mitem->n_operand;
	iitem->lno = jar->lno;
	
	if (iitem->n_op != jsize) {
		/* If Number of operands is not equal to required number of operands   */
		iitem->destroy(iitem);
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_MMT_OPRND);
	}

	cur++;	/* Move to the operand tokens   */
	/* Insert operands if required   */
	ASize i;
	for (i = 0; i<iitem->n_op; i++) {
		token = jar->get(jar, cur++);
		AString operand = token->token;
		
		AString buff = _psr_alloc_astring(operand, _PSR_BUFF_OPRND_SIZE);
		if (buff == NULL) {
			iitem->destroy(iitem);
			return FAILURE;
		}
		if (i == 0)
			iitem->operand_1 = buff;
		if (i == 1)
			iitem->operand_2 = buff;
	}

	if (ilist->insert(ilist, iitem) == SUCCESS)
		return SUCCESS;

	iitem->destroy(iitem);
	return FAILURE;
}

static AErr _psr_verify_data_decl(Jar* jar, DList* dlist, SymTable* stable, EWList* elist) {	/* Function to add data allocations to memory and maintain symbol table for that   */
	/* Add functionality of data address record keeping in dlist   */
	if ((jar == NULL) || (dlist == NULL) || (stable == NULL))
		return FAILURE;

	ASize cur = 0;
	ASize jsize = jar->size;
	Token* token = jar->get(jar, cur);

	if (token->type == _TOK_TYPE_COMMENT) {
		cur++;
		token = jar->get(jar, cur);
		jsize-=1;
	}
	
	AString label = token->token;
	if (stable->find(stable, label) != INVALID_ADDRESS)  {
	/* Case of Duplicate label   */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_DUP_LABEL);		
	}

	cur = cur+2;
	token = jar->get(jar, cur);
	if (token == NULL) {
	/* Data Missing   */
		return _psr_insert_error(elist, jar->lno, 3, _PSR_ERR_MIS_DDATA);
	}
	
	AString data = token->token;
	AInt value = strtol(data, NULL, 0);

	if (value == 0) {
	/* Format error in data  */
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_FMT_DDATA);
	}

	AAddr address;
	if (dlist->insert(dlist, data, &address) != SUCCESS) {
		return FAILURE;
	}


	AString string = _psr_alloc_astring(label, _PSR_BUFF_OPRND_SIZE);
	if (stable->insert(stable, label, address) != SUCCESS) {
		return FAILURE;
	}
	return SUCCESS;
}

static AErr _psr_verify_set_directive(Jar* jar, SymTable* stable, EWList* elist) {
	if ((jar == NULL) || (stable == NULL) || (elist == NULL))
		return FAILURE;

	ASize cur = 0;
	ASize jsize = jar->size;

	if (jsize == 0)
		return FAILURE;

	Token* token = jar->get(jar, cur);

	if (token->type == _TOK_TYPE_COMMENT) {
		cur++;
		token = jar->get(jar, cur);
		if (token == NULL)
			return FAILURE;
	}

	AString label = _psr_alloc_astring(token->token, _PSR_BUFF_OPRND_SIZE);
	if (label == NULL)
		return FAILURE;

	cur+=2;	/* Jump the cursor to the value of set directive   */
	token = jar->get(jar, cur);

	if (token == NULL) {
		/* Case of missing set directive data   */
		return _psr_insert_error(elist, jar->lno, 3, _PSR_ERR_MIS_SETDA);
	}

	AString value = token->token;
	if (value == NULL) {
		free(label);
		return FAILURE;
	}

	AInt operand_value = strtol(value, NULL, 0);
	if (operand_value == 0) {
			/* Operand format error   */
		free(label);
		return _psr_insert_error(elist, jar->lno, token->cno, _PSR_ERR_FMT_OPRND);
	}

	if (stable->insert(stable, label, operand_value) != SUCCESS) {
		free(label);
		return FAILURE;
	}

	return SUCCESS;
}

/* *
 * Debugging Functions Ahead
 * ---------------------------*/

static void _psr_debug_print_jar(Jar* jar) {
	if (jar == NULL)
		return;

	if (jar->size == NULL)
		return;

	int i;
	for (i = 0; i<jar->size; i++) {
		Token* token = jar->get(jar, i);
		if (token == NULL)
			continue;

		printf("Token: (%s), ", token->token);
	}
	printf("\n");
}

static void _psr_debug_print_symbol_table(SymTable* stable) {
	if (stable == NULL)
		return;

	printf("Symbol Table\n---------------------------\n");
	SymItem* item = stable->get(stable);
	while (item != NULL) {
		printf("%s -> %d\n", item->key, item->address);
		item = stable->get(NULL);
	}
	printf("\n");
}

static void _psr_debug_print_ewlist(EWList* elist) {
	if (elist == NULL)
		return;

	printf("\nError/Warning List\n----------------------\n");
	EWItem* eitem = elist->get(elist);
	while (eitem != NULL) {
		printf("%d: %d ==> %d\n", eitem->line, eitem->col, eitem->code);
		eitem = elist->get(NULL);
	}
	printf("\n");
}

/**
 * Functions for Parser Interface 
 * ---------------------------------*/
void psr_destroy_ParserInterface(ParserInterface* pi) {
	if (pi == NULL)
		return;

	if (pi->cargo != NULL)
		pi->cargo->destroy(pi->cargo, TRUE);

	free(pi);
	pi = NULL;
}

AErr psr_ParserInterface_parse(ParserInterface* pi, FILE* file) {
	if (pi == NULL)
		return FAILURE;

	if (file == NULL)
		return FAILURE;

	TokInterface* ti = tk_new_TokInterface(file, 1);		/* 1 Worker number: currently no multi threading implemented. */
	if (ti == NULL)
		return FAILURE;

	Cargo* cargo = tk_new_Cargo();
	if (cargo == NULL) {
		ti->destroy(ti);
		return FAILURE;
	}

	if (ti->fillCargo(ti, cargo) != SUCCESS) {
		ti->destroy(ti);
		cargo->destroy(cargo, TRUE);
		return FAILURE;
	}

	/* Cargo is filled with jars   */
	ASize sz = cargo->size;
	int i;
	AAddr address_counter = 0;
	for (i = 0; i<sz; i++) {
		Jar* jar = cargo->get(cargo, i);
		if (jar == NULL) {
			printf("DEBUG: A jar is missing.\n");
		}
		
		AType jar_type = psr_jar_classify(jar);

		if (jar_type == _PSR_TYPE_JAR_INSTRUCTION) {
			/* Jar has instrucitons  */
			_psr_verify_instruction(address_counter, pi->ilist, pi->elist, jar, pi->mnemonic_map);
			address_counter += 4;	/* Fixed size instrucions   */
		} else if (jar_type == _PSR_TYPE_JAR_LABEL) {
			/* Jar has labels  */
			_psr_verify_labels(address_counter, jar, pi->stable, pi->elist);
		} else if (jar_type == _PSR_TYPE_JAR_DATA_DECL) {
			/* Jar has data declaration  */
			AErr eno = _psr_verify_data_decl(jar, pi->dlist, pi->stable, pi->elist);
		} else if (jar_type == _PSR_TYPE_JAR_SET_DIRECT) {
			/* Jar has set directive  */
			_psr_verify_set_directive(jar, pi->stable, pi->elist);
		} else if (jar_type == _PSR_TYPE_JAR_LABL_INSTR) {
			/* Jar has label with instruction on same line  */
			_psr_verify_labl_instr(address_counter, jar, pi->ilist, pi->stable, pi->elist, pi->mnemonic_map);
		} else {
			/* Jar has Error  */
			printf("DEBUG: Invalid jar type (%d)\n", jar_type);
		}
	}

	_psr_debug_print_symbol_table(pi->stable);	/* Debug Purpose   */
	_psr_debug_print_ewlist(pi->elist);

	return SUCCESS;
}

ParserInterface* psr_new_ParserInterface(IList*  ilist, EWList* elist, SymTable* stable, DList* dlist, MnMap* mnemonic_map, RegMap* reg_map, FILE* file) {
	if ((ilist == NULL) || (elist == NULL) || (stable == NULL) || (file == NULL) || (dlist == NULL) || (mnemonic_map == NULL) || (reg_map == NULL))
		return NULL;

	ParserInterface* pi = (ParserInterface*)malloc(sizeof(ParserInterface));
	if (pi == NULL)
		return NULL;

	pi->ilist = ilist;
	pi->elist = elist;
	pi->stable = stable;
	pi->dlist = dlist;
	pi->file = file;
	pi->reg_map = reg_map;
	pi->mnemonic_map = mnemonic_map;
	
	pi->destroy = psr_destroy_ParserInterface;
	pi->parse = psr_ParserInterface_parse;
	
	return pi;
}
