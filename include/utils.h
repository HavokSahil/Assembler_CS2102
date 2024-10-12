/*
Author: Sahil Raj
Course Project CS2102: Roll -> 2301CS41
*/

#ifndef UTILS_H
#define UTILS_H

/* Context Definitions */
#define _ASS_CNTXT_COMM 0x00    /* Comment Context */
#define _ASS_CNTXT_INST 0x01     /* Instruction Context */
#define _ASS_CNTXT_LABL 0x02     /* Label Context */
#define _ASS_CNTXT_ADIR 0x03     /* Assembler Directive Context */
#define _ASS_CNTXT_INVL 0x04     /* Invalid Context */

/* Line Attributes Definitions */
#define _ASS_ATTR_LN_NOCOM 0x00  /* Line has no comment */
#define _ASS_ATTR_LN_FLCOM 0x01   /* Line is all comment */
#define _ASS_ATTR_LN_ONEOP 0x02    /* Line has one operand */
#define _ASS_ATTR_LN_ZEROP 0x03    /* Line has zero operand */
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

/* Warning Code Definitions */
#define _ASS_WARN_UNSD_LABL 0x21    /* Warning: Unused Label */
#define _ASS_WARN_INFI_LOOP 0x22     /* Warning: Infinite Loop */

/* Definitions of Data Structures: */

/* Data structure for inner node of the syntax tree */
struct _ass_instruction_node {
    unsigned char op_code;    /* OP Code of the instruction */
    char *init_operand;       /* Initial Operand before label address is substituted */
    unsigned char *operand;    /* Operand value after the label address is substituted */
};

/* Data structure for individual node of syntax tree [outer node of syntax tree] */
struct _ass_syntax_tree_node {
    unsigned long address;    /* address of the node [`key` for syntax tree] */
    unsigned long line;              /* Line number of the node in code [for debugging purpose] */
    unsigned char n_operand;   /* number of operands */
    struct _ass_instruction_node *instruction; /* Instruction at that address */
    char *comment;            /* Stores in-line comment for advanced listing [default: NULL] */
    int valid;                /* No error is found during parsing [default: true, on error: false] */
    unsigned char broadcast_code;   /* Stores error/warning code if error is found [default: 0 meaning no error] */
    
    struct _ass_syntax_tree_node *left;   /* left child [default: NULL] */
    struct _ass_syntax_tree_node *right;  /* right child [default: NULL] */
};

/* Data structure for storing assembly code internally */
struct _ass_syntax_tree {
    struct _ass_syntax_tree_node *root;   /* Initial or 0 address instruction */
    unsigned long n_nodes;                        /* Number of nodes in syntax tree */
};

/* Data structure for symbol table [hashmap <label: address>] */
struct _ass_symbol_table {
    /* Implementation details go here */
};

/* Data structure for data initialization in memory [later] */
struct _ass_data_mem_init {
    /* Implementation details go here */
};

/* Aliases for Data Structures */
typedef struct _ass_instruction_node InstNode;
typedef struct _ass_syntax_tree_node SynTreeNode;
typedef struct _ass_syntax_tree SynTree;
typedef struct _ass_symbol_table SymTable;
typedef struct _ass_data_mem_init DatMem;

/* Definition of preprocessing functions */
unsigned char get_line_context(char *line);  /* Get the context [COMM, INST, LABL, ADIR, INVL] based on the contents of the line */
unsigned char get_line_attrib(char *line); /* Get bitmasked line attributes from a line */

/* Dynamically allocate data structures */
InstNode *create_inst_node(unsigned char op_code, char *init_operand); /* Dynamically create new instruction node */
SynTreeNode *create_synTree_node(unsigned long address, unsigned long line, unsigned char n_operand, InstNode *instruction, char *comment, int valid, unsigned char err_code); /* Dynamically create syntax tree node */
SynTree *create_synTree(SynTreeNode *root); /* Create syntax tree */
SymTable *create_symTable();
DatMem *create_DatMem();

/* Free memory */
void free_instNode(InstNode *node);
void free_synTreeNode(SynTreeNode *node);
void free_synTree(SynTree *tree);
void free_symTable(SymTable *table);
void free_datMem(DatMem *mem);

/* Functions to insert or append into Data Structures */
int insert_synTree_node(SynTree *tree, SynTreeNode *node); /* [returns: Success or Failure] */
int insert_synTree_node_handler(SynTreeNode *cur, SynTreeNode *node); /* Handler for Syntax Tree insertion [returns: Success or Failure] */

/* Function to find the syntax tree node through address */
SynTreeNode *find_synTreeNode(SynTree *tree, unsigned long address);    /* Returns NULL if not found */
SynTreeNode *find_synTreeNode_handler(SynTreeNode *node, unsigned long address); /* Returns NULL if not found */

/* Function to check attributes of a line by decoding the flags output of get_line_attrib() function  */
int full_comment(unsigned char flags); /* Checks if complete line is a comment [yes: true, no: false] */
int no_comment(unsigned char flags);    /* Checks if line contains no comment [no: true, yes: false] */



/* String to integer hashmap using djb2 hash function */

/* Error Code Definations for Hash Map  */
#define _HMAP_ERR_KYEXST 0x31  /* Key already exists */ 

/* DJB2 hash function for string to integer */
unsigned long djb2(const char *str);

struct _bucket_node {   /* Linked list data structure for bucket node */
    char *key;  /* Key to access an element in a bucket */
    unsigned long value;    /* The value associated with that key */
    struct _bucket_node *next;
};

/* Bucket for particular hash value */
struct _hashmap_bucket {
    unsigned long hash_value;   /* A bucket containing elements with same hash value*/
    struct _bucket_node *head;  /* The bucket elements are stores in linked list */
    unsigned long bucket_size; /* The size of the bucket */

    /* Children Node  */
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
BktNode *create_bkt_node(char *key, unsigned long value);  /* create a bucket node in heap  */ 
HMapBkt *create_hmap_bucket(unsigned long hash_value); /* create a bucket in heap */ 
S2IHMap *create_s2i_hmap();/* create a top layer of hashmap in heap  */ 

/* Insertion, deletion and accessing function for hashmap */
BktNode *hmap_get_item_by_key(S2IHMap *map,  char *key);   /* get bucket node through key element [NULL: if not found]  */ 
HMapBkt *hmap_get_bucket(HMapBkt *bucket, unsigned long hash_value); /* get correct bucket with the hash value [NULL: if not found]  */ 
BktNode *hmap_get_item_from_bucket(char *key, HMapBkt *bucket); /* get bucket node from bucket linked list [NULL: if not found]  */ 
int hmap_insert(S2IHMap *map, char *key, unsigned long value); /* top function to insert key value in hashmap  */ 
int hmap_insert_handler(HMapBkt *bucket, unsigned long hash_value, char *key, unsigned long value); /* inner function to insert key, value in hashmap  */ 
int hmap_insert_to_bucket(HMapBkt *bucket, char *key, unsigned long value); /* function to insert bucket node in bucket  */ 
int hmap_delete(char *key); /* Function to delete a bucket node from the hashmap [later] */ 

/* Functions to free hashmap structure memory */
void free_bkt_node(BktNode *node);
void free_HMapBkt(HMapBkt *bucket);
void free_s2i_hashmap(S2IHMap *hashmap);

/* Functions for file handling  */

/* Definations of file handling errors  */
#define _ASS_ERR_FL_FMT 0x40   /* Error:  File format */ 
#define _ASS_ERR_FL_DNE 0x41   /* Error:  File doesn't exist */ 

/* Definations of file handling warnings */
#define _ASS_ERR_FL_BOF 0x51   /* Warning: Buffer overflow  */ 
#define _ASS_ERR_FL_BUF 0x52   /* Warning: Black Line  */



#endif

