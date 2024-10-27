#include <stdio.h>
#include <apsr.h>
#include <parser/parser.h>
#include <decoder/decoder.h>
#include <logger/logger.h>
#include <common_ds.h>
#include <common_types.h>
#include <err_codes.h>

/* Functions declaration  */
AErr execute_argument();
void handle_error_and_execute_argument(int error_code);

static Args parsed_args = {0};

/* Assembler entry point  */
int main(int argc, char **argv) {
    handle_error_and_execute_argument(parse_arguments(argc, argv, &parsed_args));
    return 0;
}

/*  To-do: Verbosity and better error representation  */

void handle_error_and_execute_argument(int error_code) {
    switch (error_code) {
        case _ARG_PF:
            printf("Argument parsing failed\n");
            break;
        case _ARG_SP:
            /* parsing succesful  */
            execute_argument();
            break;
        case _ARG_ATTR_HD:
            /* help is displayed in this case  */
            printf("Help is displayed.\n");
            break;
		case _ARG_ATTR_MNE:
			execute_argument();
			break;
        case _ARG_ATTR_RNM:
            printf("Argument requirement not met.\n");
            break;
        case _ARG_ATTR_INV:
            printf("Invalid argument provided.\n");
            break;
        case _ARG_ATTR_INP:
            printf("Input field not provided.\n");
            break;
        case _ARG_ATTR_ONP:
            printf("Missing file. Usage -o <filename> or --output <filename>\n");
            break;
        case _ARG_ATTR_ALF:
            printf("Missing file. Usage -a <filename> or --alf <filename>\n");
            break;
        default:
            break;
    }
}

typedef struct {
    AString mnemonic;
    AAddr encoding;
    ASize n_operand;
    AType operand_type;
} Rule;

typedef struct {
    AString key;
    AAddr encoding;
} Register;

AErr execute_argument() {
    char *input_file = parsed_args.input_filename;
    char *output_file = (parsed_args.output == 1)? parsed_args.output_filename: "machine.bin";
    char *alf_file = (parsed_args.alf == 1)? parsed_args.alf_filename: NULL;
    

    IList* ilist = ds_new_IList();
    DList* dlist = ds_new_DList();
    SymTable* stable = ds_new_SymTable();
    MnMap* map = ds_new_MnMap();
    EWList* elist = ds_new_EWList();
    RegMap* regmap = ds_new_RegMap();
    
    /* Add Instuctions  */
    Rule rules[19] = {
        {"ldc", 0, 1, TYPE_MNE_OPERAND_VALUE},
        {"adc", 1, 1, TYPE_MNE_OPERAND_VALUE},
        {"ldl", 2, 1, TYPE_MNE_OPERAND_VALUE},
        {"stl", 3, 1, TYPE_MNE_OPERAND_VALUE},
        {"ldnl", 4, 1, TYPE_MNE_OPERAND_VALUE},
        {"stnl", 5, 1, TYPE_MNE_OPERAND_VALUE},
        {"add", 6, 0, TYPE_MNE_OPERAND_NONE},
        {"sub", 7, 0, TYPE_MNE_OPERAND_NONE},
        {"shl", 8, 0, TYPE_MNE_OPERAND_NONE},
        {"shr", 9, 0, TYPE_MNE_OPERAND_NONE},
        {"adj", 10, 1, TYPE_MNE_OPERAND_VALUE},
        {"a2sp", 11, 0, TYPE_MNE_OPERAND_NONE},
        {"sp2a", 12, 0, TYPE_MNE_OPERAND_NONE},
        {"call", 13, 1, TYPE_MNE_OPERAND_OFFSET},
        {"return", 14, 0, TYPE_MNE_OPERAND_NONE},
        {"brz", 15, 1, TYPE_MNE_OPERAND_OFFSET},
        {"brlz", 16, 1, TYPE_MNE_OPERAND_OFFSET},
        {"br", 17, 1, TYPE_MNE_OPERAND_OFFSET},
        {"HALT", 18, 0}
    };

	Register registers[10] = {
        {"$s0", 0},
        {"$s1", 1},
        {"$s2", 2},
        {"$s3", 3},
        {"$s4", 4},
        {"$s5", 5},
        {"$s6", 6},
        {"$s7", 7},
        {"$s8", 8},
        {"$s9", 9},
    };


	AInt j;
    for (j = 0; j<19; j++) {
        if (map->insert(map, rules[j].mnemonic, rules[j].encoding, rules[j].n_operand, rules[j].operand_type) != SUCCESS)
            return ERR_MAIN_EXECUTION;
    }

    for (j = 0; j<10; j++) {
        if (regmap->insert(regmap, registers[j].key, registers[j].encoding) != SUCCESS)
        return ERR_MAIN_EXECUTION;
    }

	FILE* file_input = fopen(input_file, "r");
	FILE* file_output = fopen(output_file, "w");
	FILE* file_alf = fopen(alf_file, "w");
	
	LoggerInterface* li = lg_new_LoggerInterface(stdout, stdout, 0, elist, ilist, stable, dlist, map, regmap);
	if (li == NULL)
        return ERR_MAIN_EXECUTION;

	if (parsed_args.mnemonic == 1) {
		li->logmn(li);
		return SUCCESS;
	}


	if (parsed_args.input == 1 && file_input == NULL)
		return ERR_MAIN_EXECUTION;
	if (parsed_args.output == 1 && file_output == NULL)
		return ERR_MAIN_EXECUTION;
	if (parsed_args.alf == 1 && file_alf == NULL)
		return ERR_MAIN_EXECUTION;

	ParserInterface* pi = psr_new_ParserInterface(ilist, elist, stable, dlist, map, regmap, file_input);
    if (pi == NULL)
        return ERR_MAIN_EXECUTION;

	DecoderInterface* di = dc_new_DecoderInterface(ilist, stable, dlist, map, regmap, elist);
    if (di == NULL)
        return ERR_MAIN_EXECUTION;

	if (pi->parse(pi, file_input) != SUCCESS)
		return ERR_MAIN_EXECUTION;
	
	if (di->decode(di, file_output) != SUCCESS)
		return ERR_MAIN_EXECUTION;
	

	li->log(li, 0);

	if (parsed_args.alf == 1)
		li->generate_alf(li, di, file_alf);

	if (file_input)
		fclose(file_input);
	if (file_output)
		fclose(file_output);
	if (file_alf)	
		fclose(file_alf);

	pi->destroy(pi);
	di->destroy(di);
	li->destroy(li);

	return SUCCESS;
}
