#include <stdio.h>
#include <apsr.h>
#include <utils.h>

/* Functions declaration  */
int execute_argument();
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

int execute_argument() {
    char *input_file = parsed_args.input_filename;
    char *output_file = (parsed_args.output == 1)? parsed_args.output_filename: "machine.bin";
    char *alf_file = (parsed_args.alf == 1)? parsed_args.alf_filename: NULL;
    
    /* Allocate memories  */ 
    SynTree *tree = create_synTree(NULL);
    SymTable *table = create_symTable();
    DatMem *datmem = create_datMem();
    S2IHMap *mnemonic_map = get_mnemonic_map();
    AMsgList *msglist = create_new_amsg_list();
    InstrList *ilist = create_new_instruction_list();
    
    int result = asm_systemizer(input_file, tree, table, datmem, mnemonic_map, msglist);
    if (result == SUCCESS) {
        judge_instructions(tree, table, datmem, mnemonic_map);
        check_error(tree, msglist, ilist);

        FILE *output = fopen(output_file, "w");
        if (output != NULL) {
            print_machine_instruction(ilist, output);
            fclose(output);
        }
        if (parsed_args.alf) {  
            result = generate_advanced_listing_file(alf_file, tree, table, msglist);
            return result;
        }
    } else {
        return FAILURE;
    }
}
