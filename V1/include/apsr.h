#pragma once

struct _arg_option {
	char arg_short;	/* short form of argument `a character eg: o for output`  */	
	char *arg_long;	/* word form of argument */
	int required;		/* tells if the argument is necessary or not  */		
	int n_in;				/* number of inputs the argument take seperated by comma */
	char *in_type;	/* what input does argument takes like: `filename` for output */
	char *desc;			/* help description of the argument */
};

struct _arg_choices {
	int output;
	char *output_filename;

	int input;
	char *input_filename;

	int alf;	/* advanced linking file choice */	
	char *alf_filename;

	int mnemonic;	/* Shows the mnemonic for supported operands */
	/* Other non-necessary arguments */
	int verbose;	/* increase the verbosity of output  */	
	int help;			/* show help or not  */	
};

/* Aliases for argument data structures  */
typedef struct _arg_option ArgOpt;
typedef struct _arg_choices Args;

/* Definations of function returns  */
#define _ARG_SP 0x00	/* argument succesfully parsed */	
#define _ARG_PF 0x01	/* argument parsing failed  */

#define _ARG_ATTR_RNM 0x21 	/* argument requrement not met */
#define _ARG_ATTR_INV	0x22	/* argument contained is invalid */	
#define _ARG_ATTR_INP 0x23	/* input file not provided */
#define _ARG_ATTR_ONP 0x24	/* output file not provided  */ 
#define _ARG_ATTR_ALF 0x25	/* alf file not provided */	
#define _ARG_ATTR_HD	0x26	/* help has been displaed, suspend all activity */	
#define _ARG_ATTR_MNE 0x27

#define _ARG_HELP 	0x10
#define	_ARG_INPUT 	0x11
#define _ARG_OUTPUT 0x12
#define _ARG_ALFILE	0x13
#define _ARG_VERBOSE 0x14

/* Definations for long argument flag  */
#define _ARG_FL_HELP "--help"
#define _ARG_FL_VERBOSE "--verbose"
#define _ARG_FL_MNEMONIC "--mnemonic"
#define _ARG_FL_INPUT "--input"
#define _ARG_FL_OUTPUT "--output"
#define _ARG_FL_ALF "--alf"

/* Definations for short argument flag  */
#define _ARG_FS_HELP "-h"
#define _ARG_FS_VERBOSE "-v"
#define _ARG_FS_MNEMONIC "-m"
#define _ARG_FS_INPUT "-i"
#define _ARG_FS_OUTPUT "-o"
#define _ARG_FS_ALF "-a"

/* Definations for argument character  */
#define _ARG_CHR_HELP 'h'
#define _ARG_CHR_VERBOSE 'v'
#define _ARG_CHR_MNEMONIC 'm'
#define _ARG_CHR_INPUT 'i'
#define _ARG_CHR_OUTPUT 'o'
#define _ARG_CHR_ALF 'a'

int parse_arguments(int argc, char **argv, Args *parsed_args);
void show_help_text(char *exec_name);
