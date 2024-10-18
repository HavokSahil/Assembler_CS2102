#include "arg_parser.h"

static ArgOpt options[6] = {
	/* short, long, required, num_inputs, input_type, description */
	{'o', "output", 0, 1, "filename", "Specify output file for machine code in binary."},
	{'h', "help", 0, 0, "", "Shows this help text."},
	{' ', "input", 1, 1, "filename", "Specify the input .asm assembly file."},
	{'v', "verbose", 0, 0, "", "Increase the verbosity of the output."},
	{'l', "alf", 0, 1, "filename", "Specify the filename for generating the ALF(Advanced Linking File)."},
	{0, NULL, 0, 0, NULL, NULL}	/* End Marker */
};

int parse_arguments(int argc, char **argv) {
	Args argument;

	if (argc<2) {
		show_help_text(argv[0]);
	}
}

void show_help_text(char *program_name) {
	printf("\nAssembler for Simple Instructions.\n Usage: %s <arguments>\n");
	printf("\nHelp:\n");
	printf("%-*s %-*s %-*s %-*s %-*s %-*s\n", 5, "Args", 10, "", 10, "Required", 8, "Inputs", 16, "Input Type", 64, "Description");

	int i;
	for (i = 0; i<5; i++) {
		ArgOpt opt = options[i];
		char arg_short = opt.arg_short;
		char *arg_long = opt.arg_long;
		char *required = (opt.required == 1)? "Yes": "No";
		int n_in = opt.n_in;
		char *in_type = opt.in_type;
		char *description = opt.description;

		printf("%-*c %-*s %-*s %-*d %-*s %-*s\n", 5, arg_short, 10, arg_long, 10, required, 8, n_in, 16, in_type, 64, description);

	}
}
