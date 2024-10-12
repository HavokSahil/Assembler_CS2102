/*
Author: Sahil Raj
Course Project CS2102: Roll -> 2301CS41
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

/* Context Definitions */
#define _ASS_CNTXT_COMM 0x00     /* Comment Context */
#define _ASS_CNTXT_INST 0x01     /* Instruction Context */
#define _ASS_CNTXT_LABL 0x02     /* Label Context */
#define _ASS_CNTXT_ADIR 0x03     /* Assembler Directive Context */
#define _ASS_CNTXT_INVL 0x04     /* Invalid Context */

/* Line Attributes Definitions */
#define _ASS_ATTR_LN_NOCOM 0x00     /* Line has no comment */
#define _ASS_ATTR_LN_FLCOM 0x01     /* Line is all comment */
#define _ASS_ATTR_LN_ONEOP 0x02     /* Line has one operand */
#define _ASS_ATTR_LN_ZEROP 0x03     /* Line has zero operand */
#define _ASS_ATTR_LN_LBDCL 0x04     /* Line contains label declaration */
#define _ASS_ATTR_LN_LBDAT 0x05     /* Line has data declaration */
#define _ASS_ATTR_LN_LBSET 0x06     /* Line has set pseudo instruction */

/* General Flag Definitions */
#define SUCCESS 0x00
#define FAILURE 0x01

/* General Error Code Definitions [later] */
#define _GEN_ERR_ 0x02

/* General Warning Code Definitions [later] */
#define _GEN_WARN_ 0x03

/* Ass Error Codes Definitions */
#define _ASS_ERR_INV_OPCOD 0x11    /* Error: Invalid Operation or no matched Op-Code */
#define _ASS_ERR_UDC_LABEL 0x12    /* Error: Undeclared Label */
#define _ASS_ERR_MIS_OPRND 0x13    /* Error: Missing Operand */
#define _ASS_ERR_EXT_OPRND 0x14    /* Error: Extra Operand */
#define _ASS_ERR_DUP_LABEL 0x15    /* Error: Duplicate Label */
#define _ASS_ERR_INV_OPRND 0x16    /* Error: Invalid Operand */
#define _ASS_ERR_MIS_INSTR 0x17    /* Error: Missing Instruction  */ 

/* Warning Code Definitions */
#define _ASS_WARN_UNSD_LABL 0x21    /* Warning: Unused Label */
#define _ASS_WARN_INFI_LOOP 0x22    /* Warning: Infinite Loop */

/* Definitions of Data Structures: */

/* String to integer hashmap using djb2 hash function */

/* Error Code Definations for Hash Map  */
#define _HMAP_ERR_KYEXST 0x31  /* Key already exists */ 

/* DJB2 hash function for string to integer */
unsigned long djb2(const char *str);

struct _bucket_node {       /* Linked list data structure for bucket node */
    char *key;              /* Key to access an element in a bucket */
    unsigned long value;    /* The value associated with that key */
    struct _bucket_node *next;
};

/* Bucket for particular hash value */
struct _hashmap_bucket {
    unsigned long hash_value;   /* A bucket containing elements with same hash value*/
    struct _bucket_node *head;  /* The bucket elements are stores in linked list */
    unsigned long bucket_size;  /* The size of the bucket */

    /* Children Node    */
    struct _hashmap_bucket *left;
    struct _hashmap_bucket *right;
};

/* hashmap tree root */
struct _string2int_hashmap {
  struct _hashmap_bucket *root;  /* Root of the element of the hashmap tree */
};

/* Aliases for HashMap Data Structures */
typedef struct _bucket_node BktNode;
typedef struct _hashmap_bucket HMapBkt;
typedef struct _string2int_hashmap S2IHMap;

/* dynamic memory allocation function for hashmap */
BktNode *create_bkt_node(char *key, unsigned long value);   /* create a bucket node in heap  */ 
HMapBkt *create_hmap_bucket(unsigned long hash_value);      /* create a bucket in heap */ 
S2IHMap *create_s2i_hmap();                                 /* create a top layer of hashmap in heap  */ 

/* Insertion, deletion and accessing function for hashmap */
BktNode *hmap_get_item_by_key(S2IHMap *map,  char *key);            /* get bucket node through key element [NULL: if not found]  */ 
HMapBkt *hmap_get_bucket(HMapBkt *bucket, unsigned long hash_value);/* get correct bucket with the hash value [NULL: if not found]  */ 
BktNode *hmap_get_item_from_bucket(char *key, HMapBkt *bucket);     /* get bucket node from bucket linked list [NULL: if not found]  */ 
int hmap_insert(S2IHMap *map, char *key, unsigned long value);      /* top function to insert key value in hashmap  */ 
int hmap_insert_handler(HMapBkt *bucket, unsigned long hash_value, char *key, unsigned long value); /* inner function to insert key, value in hashmap  */ 
int hmap_insert_to_bucket(HMapBkt *bucket, char *key, unsigned long value); /* function to insert bucket node in bucket  */ 
int hmap_delete(char *key); /* Function to delete a bucket node from the hashmap [later] */ 

/* Functions to free hashmap structure memory */
void free_bkt_node(BktNode *node);
void free_HMapBkt(HMapBkt *bucket);
void free_s2i_hashmap(S2IHMap *hashmap);


/* Data structure for inner node of the syntax tree */
struct _ass_instruction_node {
    char *mnemonic;             /* Initial Mnemonic of the instruction without translation */ 
    unsigned char op_code;      /* OP Code of the instruction */
    char *init_operand;         /* Initial Operand before label address is substituted */
    unsigned char operand;     /* Operand value after the label address is substituted */
};

/* Data structure for individual node of syntax tree [outer node of syntax tree] */
struct _ass_syntax_tree_node {
    unsigned long address;          /* address of the node [`key` for syntax tree] */
    unsigned long line;             /* Line number of the node in code [for debugging purpose] */
    unsigned char n_operand;        /* number of operands */
    struct _ass_instruction_node *instruction; /* Instruction at that address */
    char *comment;                  /* Stores in-line comment for advanced listing [default: NULL] */
    int valid;                      /* No error is found during parsing [default: true, on error: false] */
    unsigned char broadcast_code;   /* Stores error/warning code if error is found [default: 0 meaning no error] */
    
    struct _ass_syntax_tree_node *left;   /* left child [default: NULL] */
    struct _ass_syntax_tree_node *right;  /* right child [default: NULL] */
};

/* Data structure for storing assembly code internally */
struct _ass_syntax_tree {
    struct _ass_syntax_tree_node *root; /* Initial or 0 address instruction */
    unsigned long n_nodes;              /* Number of nodes in syntax tree */
};

/* Data structure for data initialization in memory [later] */
struct _ass_data_mem_init {
    /* Implementation details go here */
};

/* Aliases for Data Structures */
typedef struct _ass_instruction_node InstNode;
typedef struct _ass_syntax_tree_node SynTreeNode;
typedef struct _ass_syntax_tree SynTree;
typedef struct _string2int_hashmap SymTable;
typedef struct _ass_data_mem_init DatMem;

/* Definition of preprocessing functions */
unsigned char get_line_context(char *line); /* Get the context [COMM, INST, LABL, ADIR, INVL] based on the contents of the line */
unsigned char get_line_attrib(char *line);  /* Get bitmasked line attributes from a line */

/* Dynamically allocate data structures */
InstNode *create_inst_node(char *mnemonic, char *init_operand); /* Dynamically create new instruction node */
SynTreeNode *create_synTree_node(unsigned long address, unsigned long line, unsigned char n_operand, InstNode *instruction, char *comment, int valid, unsigned char err_code); /* Dynamically create syntax tree node */
SynTree *create_synTree(SynTreeNode *root);                 /* Create syntax tree */
SymTable *create_symTable();                                /* Dynamically create symbol table structure */ 
DatMem *create_datMem();                                    /* Dynamically create data memory structure  */ 
S2IHMap *get_mnemonic_map();                                /* Returns the hash map of instruction mnemonics  */ 

/* Free memory */
void free_instNode(InstNode *node);
void free_synTreeNode(SynTreeNode *node);
void free_synTree(SynTree *tree);
void free_symTable(SymTable *table);
void free_datMem(DatMem *mem);

/* Functions to insert or append into Data Structures */
int insert_synTree_node(SynTree *tree, SynTreeNode *node);              /* [returns: Success or Failure] */
int insert_synTree_node_handler(SynTreeNode *cur, SynTreeNode *node);   /* Handler for Syntax Tree insertion [returns: Success or Failure] */

/* Function to find the syntax tree node through address */
SynTreeNode *find_synTreeNode(SynTree *tree, unsigned long address);            /* Returns NULL if not found */
SynTreeNode *find_synTreeNode_handler(SynTreeNode *node, unsigned long address);/* Returns NULL if not found */

/* Function to check attributes of a line by decoding the flags output of get_line_attrib() function  */
int full_comment(unsigned char flags);  /* Checks if complete line is a comment [yes: true, no: false] */
int no_comment(unsigned char flags);    /* Checks if line contains no comment [no: true, yes: false] */
int is_instruction(unsigned char flag);
int is_label(unsigned char flag);
int is_directive(unsigned char flag);
int no_comment(unsigned char flags);
int one_operand(unsigned char flags);
int zero_operand(unsigned char flags);
int label_declaration(unsigned char flags);
int label_data_declaration(unsigned char flags);
int set_pseudo_instruction(unsigned char flags);

/* Functions for file handling  */

/* Constants Definations  */
#define _ASS_ATTR_FL_BUFFSZ 1024    /* Buffer size for asm systemizer  */ 
#define _ASS_ATTR_BF_COMLIM 256     /* Comment buffer size of syntax tree */ 
#define _ASS_ATTR_BF_MNEMON 32      /* Mnemonic buffer size  */ 
#define _ASS_ATTR_BF_OPERND 64      /* Operand buffer size  */ 
#define _ASS_ATTR_BF_HMAPKY 64      /* Operand buffer size  */

/* Definations of file handling errors  */
#define _ASS_ERR_FL_FMT 0x40   /* Error:  File format */ 
#define _ASS_ERR_FL_COF 0x41   /* Error:  Can't open file */ 

/* Definations of file handling warnings */
#define _ASS_ERR_FL_BOF 0x51   /* Warning: Buffer overflow  */ 
#define _ASS_ERR_FL_BUF 0x52   /* Warning: Black Line  */

/* Message Structures */

/* Defination of message serverity level  */
#define _ASS_MSG_LVL_INF 0x61
#define _ASS_MSG_LVL_ERR 0x62
#define _ASS_MSG_LVL_WRN 0x63

struct _ass_message {
    unsigned char code;         /* codes like error codes or warning codes */ 
    unsigned char severity;    /* severity like {ERROR, WARNING, INFO}  */ 
    int line_no;                /* line number of context [default -1]  */
    struct _ass_message *next;
};

struct _ass_message_list {
    struct _ass_message *msgs;
    unsigned long size;
};

struct _ass_machine_instruction {
    char byte1;
    char byte2;
    char byte3;
    char byte4;
    struct _ass_machine_instruction *next;
};

struct _ass_instruction_list {
    struct _ass_machine_instruction *intrs;
};

typedef struct _ass_message AMsg;
typedef struct _ass_message_list AMsgList;
typedef struct _ass_machine_instruction MacInstr;
typedef struct _ass_instruction_list InstrList;


int asm_systemizer(const char *filename, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map, AMsgList *msglist);   /* Reads the asm file and store it in syntax tree  */ 
int asm_st_constructor(char *line, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map, AMsgList *msglist, unsigned long line_no);    /* decode the asm code line into syntax tree node and insert it  */ 
void judge_instructions(SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map); /* Makes final pass through all the nodes to find errors  */ 
void judge_instructions_handler(SynTreeNode *node, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map);  /* Inner recursive function for judge_instructions()  */ 

MacInstr *create_new_machine_instruction(unsigned char op_code, unsigned long operand);
int insert_machine_instruction(InstrList *ilist, unsigned char op_code, unsigned long operand);
int insert_machine_instruction_handler(MacInstr *minstr, unsigned char op_code, unsigned long operand);
void print_machine_instruction(InstrList *ilist, FILE *stream);

AMsg *create_new_message(unsigned char code, unsigned char severity, int line_no);
AMsgList *create_new_amsg_list();
int insert_message(AMsgList *mlist, unsigned char code, unsigned char severity, int line_no);
int insert_message_handler(AMsg *msgs, unsigned char code, unsigned char severity, int line_no);
void print_message_list(AMsgList *mlist, FILE *stream);

int check_error(SynTree *tree, AMsgList *msglist);
int check_error_handler(SynTreeNode *node, AMsgList *msglist);
int generate_advanced_listing_file(const char *filename, SynTree *tree, SymTable *table, AMsgList *msglist);

/* Dump Functions for Structures [Only for debugging purposes]  */
void dump_syntax_tree_node(const SynTreeNode *node, FILE *stream);
void dump_syntax_tree(const SynTree *tree, FILE *stream); 
void dump_hash_table(const S2IHMap *map, FILE *stream);
void dump_hashmap_bucket(const HMapBkt *bucket, FILE *stream);
void dump_bucket_node(const BktNode *node, FILE *stream);
#endif

