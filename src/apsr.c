#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <apsr.h>

static ArgOpt options[6] = {
    {'o', "output", 1, 1, "filename", "Specify the output file"},
    {'i', "input", 1, 1, "filename", "Specify the input file"},
    {'a', "alf", 0, 1, "filename", "Specify the advanced linking file"},
    {'v', "verbose", 0, 0, NULL, "Increase verbosity"},
    {'h', "help", 0, 0, NULL, "Show help text"},
    {0, NULL, 0, 0, NULL, NULL}  
};

int parse_arguments(int argc, char **argv, Args *parsed_args) {
    if (argc < 2) {
        show_help_text(argv[0]);
        return _ARG_ATTR_HD;
    }
    unsigned long i = 1;
    int stack_on = 0;  /* waiting for argument values */ 
    char last_arg = '\0';
    while (i < argc) {
        if (stack_on == 0) {  /* if an argument flag is present */ 
            char *flag = argv[i];
            if (*flag == '-') {
                if (*(flag + 1) == '-') {  /* argument is present in long form */
                    if (strcmp(flag, _ARG_FL_HELP) == 0) {
                        show_help_text(argv[0]);
                        return _ARG_ATTR_HD;
                    } else if (strcmp(flag, _ARG_FL_VERBOSE) == 0) {
                        parsed_args->verbose = 1;
                    } else if (strcmp(flag, _ARG_FL_OUTPUT) == 0) {
                        last_arg = _ARG_CHR_OUTPUT;
                        stack_on = 1;
                        parsed_args->output = 1;
                    } else if (strcmp(flag, _ARG_FL_INPUT) == 0) {
                        last_arg = _ARG_CHR_INPUT;
                        stack_on = 1;
                        parsed_args->input = 1;
                    } else if (strcmp(flag, _ARG_FL_ALF) == 0) {
                        last_arg = _ARG_CHR_ALF;
                        stack_on = 1;
                        parsed_args->alf = 1;
                    } else {
                       /* Invalid argument  */ 
                        return _ARG_ATTR_INV;
                    }
                } else {  /* argument is present in short form */ 
                    if (strcmp(flag, _ARG_FS_HELP) == 0) {
                        show_help_text(argv[0]);
                        return _ARG_ATTR_HD;
                    } else if (strcmp(flag, _ARG_FS_VERBOSE) == 0) {
                        parsed_args->verbose = 1;
                    } else if (strcmp(flag, _ARG_FS_INPUT) == 0) {
                        last_arg = _ARG_CHR_INPUT;
                        stack_on = 1;
                        parsed_args->input = 1;
                    } else if (strcmp(flag, _ARG_FS_OUTPUT) == 0) {
                        last_arg = _ARG_CHR_OUTPUT;
                        stack_on = 1;
                        parsed_args->output = 1;
                    } else if (strcmp(flag, _ARG_FS_ALF) == 0) {
                        last_arg = _ARG_CHR_ALF;
                        stack_on = 1;
                        parsed_args->alf = 1;
                    } else {
                       /* Invalid argument  */ 
                        return _ARG_ATTR_INV;
                    } 
                }
            }
        } else {/* if waiting for values of preceding argument  */ 
            char *value = argv[i];
            switch (last_arg) {
                case _ARG_CHR_OUTPUT:
                    stack_on = 0;
                    parsed_args->output_filename = value;
                    break;
                case _ARG_CHR_INPUT:
                    stack_on = 0;
                    parsed_args->input_filename = value;
                    break;
                case _ARG_CHR_ALF:
                    stack_on = 0;
                    parsed_args->alf_filename = value;
                    break;
                default:
                    break;
            }
        }
        i++;     
    }
    
    if ((parsed_args->input == 0)||(parsed_args->input_filename == NULL)) {
        /* input filename not provided  */
       return _ARG_ATTR_INP; 
    }
    if ((parsed_args->output == 1) && (parsed_args->output_filename == NULL)) {
        /* output file chosen but not specified  */
        return _ARG_ATTR_ONP;
    }
    if ((parsed_args->alf == 1) && (parsed_args->alf_filename == NULL)) {
        /* alf file chosen but not specified  */
        return _ARG_ATTR_ALF;
    }

    return _ARG_SP;
}


void show_help_text(char *exec_name) {
    printf("Assembler for Simple Instruction Sets [CS2102]\n------------------------------------------------\nAuthor: Sahil Raj\nRoll: 2301CS41\n");
    printf("\nNote: Intended only for demonstration.\n");
    printf("\nUsage: %s <arguments>\n", exec_name);

    printf("\nHelp:\n");
    printf("%-*s %-*s %-*s %-*s %-*s %-*s\n", 6, "Args", 12, " ", 10, "Required", 10, "Inputs", 16, "Type", 64, "Description");
    printf("-------------------------------------------------------------------------\n");

    int i;
    for (i = 0; i<5; i++) {
        char arg_short = options[i].arg_short;
        char *arg_long = options[i].arg_long;
        char *required = (options[i].required == 1)? "Yes": "No";
        int inputs = options[i].n_in;
        char *input_type = (options[i].in_type == NULL)? " ": options[i].in_type;
        char *description = options[i].desc;
        printf("-%-*c --%-*s %-*s %-*d %-*s %-*s\n", 6, arg_short, 12, arg_long, 10, required, 10, inputs, 16, input_type, 64, description);
    }
    printf("\n");
}

