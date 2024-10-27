#include <logger/logger.h>

/* Error Code Description for Parser */
struct parser_error_description {
	AErr code;
	AString description;
};

typedef struct parser_error_description PEDesc;


PEDesc parser_error_description[] = {
	{PSR_ERR_INV_MNEMO, "Invalid Mnemonic"},
	{PSR_ERR_INV_OPRND, "Invalid Operand"},
	{PSR_ERR_FMT_OPRND, "Invalid Format of Operand"},
	{PSR_ERR_FMT_DDATA, "Invalid Format of Data"},
	{PSR_ERR_MIS_SETDA, "Missing Data for SET Directive"},
	{PSR_ERR_MIS_OPRND, "Missing Operand"},
	{PSR_ERR_MIS_DDATA, "Missing Data"},
	{PSR_ERR_MMT_OPRND, "Mismatch in Number of Operands"},
	{PSR_ERR_INV_LABEL, "Invalid Label"},
	{PSR_ERR_DUP_LABEL, "Duplicate Label"},
	{PSR_ERR_INV_JRTYP, "Invalid Jar Type"},
    {DEC_ERR_LBL_UNDEF, "Undefined Label"},
    {DEC_ERR_INV_OPRND, "Invalid Operand"},
    {DEC_ERR_INV_OFFST, "Invalid Offset"},
    {WARN_ASM_DUPLICATE_LABEL, "Duplicate Label"},
    {WARN_ASM_INFINITE_LOOP, "Infinite Loop"},
};

typedef struct _err_description {
    AErr code;
    AString description;
} ErrDesc;

ErrDesc err_description[] = {
    {SUCCESS, "Success"},
    {ERR_MEM_ALLOC_FAIL, "Memory Allocation Failed"},
    {ERR_MEM_FREE_FAIL, "Memory Free Failed"},
    {ERR_MEM_REALLOC_FAIL, "Memory Reallocation Failed"},
    {ERR_MEM_INVALID_ADDR, "Invalid Memory Address"},
    {ERR_MEM_INVALID_SIZE, "Invalid Memory Size"},
    {ERR_MEM_INVALID_BLOCK, "Invalid Memory Block"},
    {ERR_FILE_OPEN_FAIL, "File Open Failed"},
    {ERR_FILE_CLOSE_FAIL, "File Close Failed"},
    {ERR_FILE_READ_FAIL, "File Read Failed"},
    {ERR_FILE_WRITE_FAIL, "File Write Failed"},
    {ERR_FILE_INVALID_PATH, "Invalid File Path"},
    {ERR_FILE_INVALID_MODE, "Invalid File Mode"},
    {ERR_FILE_INVALID_FILE, "Invalid File"},
    {ERR_FILE_INVALID_DIR, "Invalid Directory"},
    {ERR_STR_INVALID_STRING, "Invalid String"},
    {ERR_STR_INVALID_LENGTH, "Invalid String Length"},
    {ERR_STR_INVALID_INDEX, "Invalid String Index"},
    {ERR_STR_INVALID_CHAR, "Invalid Character"},
    {ERR_STR_INVALID_FORMAT, "Invalid String Format"},
    {ERR_INVALID_INTERFACE, "Invalid Interface"},
    {ERR_INTERFACE_GEN_FAIL, "Interface Generation Failed"},
    {ERR_DS_INVALID_STRUCT, "Invalid Data Structure"},
    {ERR_DS_STRUCT_GEN_FAIL, "Data Structure Generation Failed"},
    {ERR_DS_INSERT_FAIL, "Data Structure Insertion Failed"},
    {ERR_TOK_INVALID_JAR, "Invalid Jar"},
    {ERR_TOK_INVALID_TOKEN, "Invalid Token"},
    {ERR_TOK_INVALID_CARGO, "Invalid Cargo"},
    {ERR_TOK_INVALID_PACKET, "Invalid Packet"},
    {ERR_TOK_CARGO_LOAD_FAIL, "Cargo Load Failed"},
    {ERR_TOK_JARIFICATION_FAIL, "Jarification Failed"},
    {ERR_PSR_NULL_ARG, "Null Argument"},
    {ERR_PSR_INVALID_JAR, "Invalid Jar"},
    {ERR_PSR_INVALID_TOKEN, "Invalid Token"},
    {ERR_PSR_TOK_STRING_TEMPERED, "Tempered Token String"},
    {ERR_MAP_DUP_KEY, "Duplicate Key"},
    {ERR_MAP_INVALID_STRUCT, "Invalid Map Structure"},
    {WARN_PSR_INVALID_JAR_TYPE, "Invalid Jar Type"},
    {ERR_ASM_INVALID_MNEMONIC, "Invalid Mnemonic"},
    {ERR_ASM_INVALID_OPCODE, "Invalid Opcode"},
    {ERR_ASM_INVALID_OPERAND, "Invalid Operand"},
    {ERR_ASM_INVALID_LABEL, "Invalid Label"},
    {ERR_ASM_INVALID_DIRECTIVE, "Invalid Directive"},
    {ERR_ASM_INVALID_SET_DIRECTIVE, "Invalid Set Directive"},
    {ERR_ASM_INVALID_DATA_DIRECTIVE, "Invalid Data Directive"},
    {ERR_ASM_INVALID_DATA, "Invalid Data"},
    {ERR_ASM_INVALID_INSTRUCTION, "Invalid Instruction"},
    {ERR_ASM_INVALID_LABEL_INSTRUCTION, "Invalid Label Instruction"},
    {ERR_ASM_INVALID_LABEL_DIRECTIVE, "Invalid Label Directive"},
    {ERR_LOG_FAIL, "Log Failed"},
    {ERR_LOG_INVALID_STREAM, "Invalid Stream"},
};

#define ERR_DESC_SIZE (sizeof(err_description)/sizeof(ErrDesc))
#define PSR_ERR_DESC_SIZE sizeof(parser_error_description)/sizeof(PEDesc)


const AString red = "\033[1;31m";
const AString green = "\033[1;32m";
const AString yellow = "\033[1;33m";
const AString blue = "\033[1;34m";
const AString magenta = "\033[1;35m";
const AString cyan = "\033[1;36m";
const AString reset = "\033[1;0m";
const AString white_background = "\033[4;34;47m";
const AString err_msg = "\033[3;36m";


static AString get_error_description(AErr code) {
    int i;
	for (i = 0; i < ERR_DESC_SIZE; i++) {
		if (err_description[i].code == code) {
			return err_description[i].description;
		}
	}
	return NULL;
}

static AString get_parser_error_description(AErr code) {
    int i;
	for (i = 0; i < PSR_ERR_DESC_SIZE; i++) {
		if (parser_error_description[i].code == code) {
			return parser_error_description[i].description;
		}
	}
	return NULL;
}

static ABool is_error(AErr code) {
    if (code >= 0xE0)
        return TRUE;
    return FALSE;
}

static ABool is_warning(AErr code) {
    if (code < 0xE0 )
        return TRUE;
    return FALSE;
}

static ABool is_info(AErr code) {
    if ((code >= THRESHOLD_WARN) && (code < THRESHOLD_INFO))
        return TRUE;
    return FALSE;
}

static ABool is_debug(AErr code) {
    if ((code >= THRESHOLD_INFO) && (code < THRESHOLD_DEBUG))
        return TRUE;
    return FALSE;
}

/**
 * Function for Logger Interface
 * ---------------------------------*/

AErr lg_log(LoggerInterface* li, AType level) {
    if (li == NULL)
        return ERR_INVALID_INTERFACE;


    if (li->stream1 == NULL)
        return ERR_LOG_INVALID_STREAM;

    if (li->elist == NULL)
        return ERR_LOG_FAIL;

    EWList* elist = li->elist;
    EWItem* item = li->elist->get(li->elist);

    while (item != NULL) {
        if (item->code >= level) {
            /* Print error in red, warning in magenta, info in yellow and debug in white*/
            if (is_error(item->code) == TRUE)
                fprintf(li->stream1, "%sError%s\t%s%-*s%d%s %s%-*s %-*d%s\t", red, reset,white_background, 6, "line", item->line, reset, white_background, 6, "column", 1, item->col, reset);
            else if (is_warning(item->code) == TRUE)
                fprintf(li->stream1, "%sWarning%s\t%s%-*s%d%s %s%-*s %-*d%s\t", magenta, reset,white_background, 6, "line", item->line, reset, white_background, 6, "column", 1, item->col, reset);
            else if (is_info(item->code) == TRUE)
                fprintf(li->stream1, "%sInfo%s\t%s%-*s%d%s %s%-*s %-*d%s\t", yellow, reset,white_background, 6, "line", item->line, reset, white_background, 6, "column", 1, item->col, reset);
            else if (is_debug(item->code) == TRUE)
                fprintf(li->stream1, "%Debug%s\t%s%-*s%d%s %s%-*s %-*d%s\t", cyan, reset,white_background, 6, "line", item->line, reset, white_background, 6, "column", 1, item->col, reset);
            else
                fprintf(li->stream1, "%sUnknown%s line%d:%d\n", reset, reset, item->line, item->col);   
            fprintf(li->stream1, "%s%s%s\n", err_msg, get_parser_error_description(item->code), reset);
        }
        item = li->elist->get(NULL);
    }


    if (level > li->level)
        return SUCCESS;

    

    return ERR_LOG_FAIL;
}

AErr lg_log_mnemonic(LoggerInterface* li) {
    if (li == NULL)
        return ERR_INVALID_INTERFACE;

    if (li->mnmap == NULL)
        return ERR_INVALID_INTERFACE;
    
    MnMap* map = li->mnmap;
    if (map == NULL)
        return ERR_INVALID_INTERFACE;

    MnItem* item = map->get(map);

    fprintf(li->stream1, "\n%-*s %-*s %-*s\n-------------------------------\n", 10, "Mnemonic", 10, "Operands", 10, "Opcode");
    while (item != map->end()) {
        fprintf(li->stream1, "%-*s %-*d %s%04X%s\n", 10, item->key, 10, item->n_operand, green, item->encoding, reset);
        item = map->get(NULL);
    }
    printf("\n");
    return SUCCESS;
}

AErr lg_log_register(LoggerInterface* li) {
    if (li == NULL)
        return ERR_INVALID_INTERFACE;

    if (li->rmap == NULL)
        return ERR_INVALID_INTERFACE;

    printf("\n%-*s %-*s\n--------------------\n", 10, "Register", 10, "Address");
    RegMap* map = li->rmap;
    RegItem* item = map->get(map);
    while (item != NULL) {
        printf("%-*s %s%04X%s\n",  10, item->key, magenta, item->encoding, reset);
        item = map->get(NULL);
    }

    printf("\n");
    return SUCCESS;
}

static AErr _lg_dump_symbol_table(SymTable* stable, FILE* file) {
    if (stable == NULL || file == NULL)
        return ERR_INVALID_INTERFACE;
    
    fprintf(file, "\nSymbol Table\n------------------------------\n");
    fprintf(file, "%-*s %-*s\n------------------------------\n", 10, "Label", 10, "Address");
    SymItem* sitem = stable->get(stable);
    while (sitem != stable->end()) {
        fprintf(file, "%-*s %08X\n", 10, sitem->key, sitem->address);
        sitem = stable->get(NULL);
    }

    return SUCCESS;
}

static AErr _lg_dump_ewlist(EWList* elist, FILE* file) {
    if (elist == NULL || file == NULL)
        return ERR_INVALID_INTERFACE;

    EWItem* eitem = elist->get(elist);
    fprintf(file, "\nError/Warning List\n--------------------------------------------------\n");
    fprintf(file, "%-*s %-*s %-*s %-*s %-*s\n", 6, "Flag", 4, "Line", 4, "Col", 6, "Code", 32, "Description");
    fprintf(file, "--------------------------------------------------\n");
    while (eitem != elist->end()) {
        AString flag = (is_error(eitem->code) == TRUE)? "ERROR": "WARN";
        AString desc = get_parser_error_description(eitem->code);
        fprintf(file, "%-*s %-*d %-*d %04X\t%-*s\n", 6, flag, 4, eitem->line, 4, eitem->col, eitem->code, 32, desc);
        eitem = elist->get(NULL);
    }
    return SUCCESS;
}

static AErr _lg_dump_dlist(DList* dlist, FILE* file) {
    if (dlist == NULL || file == NULL)
        return ERR_INVALID_INTERFACE;

    DItem* ditem = dlist->get(dlist);
    fprintf(file, "\nMemory Map\n----------------------------------------\n");
    fprintf(file, "%-*s %-*s\n----------------------------------------\n", 10, "Offset", 10, "Data");
    while (ditem != dlist->end()) {
        fprintf(file, "%08X\t%d\n", ditem->address, ditem->data);
        ditem = dlist->get(NULL);
    }
    return SUCCESS;
}

AErr lg_generate_alf(LoggerInterface* li, DecoderInterface* di, FILE* file) {
    if ((li == NULL) || (file == NULL) || (di == NULL))
        return ERR_INVALID_INTERFACE;

    if ((li->dlist == NULL) || (li->ilist == NULL) || (li->elist == NULL) || (li->mnmap == NULL))
        return ERR_INVALID_INTERFACE;

    
    /* Print Instruction Table */
    fprintf(file, "Instruction Table\n-----------------------------------------------------------------------------\n");
    fprintf(file, "%-*s %-*s %-*s %-*s %-*s %-*s\n", 10, "Address", 10, "Line", 10, "Opcode", 10, "Operand", 10, "Machine-Code", 15, "Decoder-Status");
    fprintf(file, "-----------------------------------------------------------------------------\n");
    IItem* item = li->ilist->get(li->ilist);
    while (item != li->ilist->end()) {
        AAddr address = item->address;
        ASize line = item->lno;
        ASize n_op = item->n_op;
        AString opcode = item->opcode;
        AString operand1 = (item->operand_1 != NULL)? item->operand_1 : "";
        AAddr machine_code;
        AErr err = di->decode_instruction(di, item, &machine_code, DECODER_MODE_ALF);
        AString status = "OK";
        if (err != SUCCESS) {
            if (is_error(err)) {
                status = "ERROR";
                machine_code = 0xFFFFFFFF;
            }
    
            else if (is_warning(err))
                status = "WARN";
        }
        fprintf(file, "%-*d %-*d %-*s %-*s %08X\t\t%-*s\n", 10, address, 10, line, 10, opcode, 10, operand1, machine_code, 10, status);
        item = li->ilist->get(NULL);
    }

    AErr eno = _lg_dump_symbol_table(li->stable, file);
    if (eno != SUCCESS)
        return eno;

    eno = _lg_dump_ewlist(li->elist, file);
    if (eno != SUCCESS)
        return eno;

    eno = _lg_dump_dlist(li->dlist, file);
    if (eno != SUCCESS)
        return eno;

    return SUCCESS;
}

void lg_destroy_LoggerInterface(LoggerInterface* li) {
    if (li == NULL)
        return;

    free(li);
}

LoggerInterface* lg_new_LoggerInterface(FILE* stream1, FILE* stream2, AType level, EWList* elist, IList* ilist, SymTable* stable, DList* dlist, MnMap* mnmap, RegMap* rmap) {
    if ((stream1 == NULL) || (stream2 == NULL) || (elist == NULL) || (ilist == NULL) || (stable == NULL) || (dlist == NULL) || (mnmap == NULL) || (rmap == NULL))
        return NULL;

    LoggerInterface* li = (LoggerInterface*)malloc(sizeof(LoggerInterface));
    if (li == NULL)
        return NULL;

    li->stream1 = stream1;
    li->stream2 = stream2;
    li->level = level;
    li->elist = elist;
    li->ilist = ilist;
    li->stable = stable;
    li->dlist = dlist;
    li->mnmap = mnmap;
    li->rmap = rmap;

    li->log = lg_log;
    li->logmn = lg_log_mnemonic;
    li->logreg = lg_log_register;
    li->generate_alf = lg_generate_alf;
    li->destroy = lg_destroy_LoggerInterface;

    return li;
}