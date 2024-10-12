/*
Author: Sahil Raj
File Name: Utils.c
Course Project [CS2102]: Roll -> 2301CS41
 */

#include <string.h>
#include <utils.h>
#include <stdio.h>
#include <stdlib.h>

/* Returns the attributes of lines in masked 8-bit number */
unsigned char get_line_attrib(char *line) {

    unsigned char result = 0;

    /* Check for no comments */
    int cur = 0;
    int i;  /* For loop iterator */
    for (i = 0; i<(int)strlen(line); i++) {
        if (line[i] == ';') cur = 1;
    }
    if (!cur) {
        result |= (1<<_ASS_ATTR_LN_NOCOM);
    }
    
    /* If not no-comment */
    if ((result & 1) == 0) {
        /* Check for full comment */ 
        char *ptr = line;
        while (*ptr == ' ') ptr++;
        if (*ptr == ';')
            result |= (1<<_ASS_ATTR_LN_FLCOM);
    }
    

    /* If line is not just all comments */
    if ((result & (1<<_ASS_ATTR_LN_FLCOM)) == 0) {
        char *token = line;
        char delim[] = ";";
        char ndelim[] = "  \t";
        
        /* If line has comment */
        if ((result & (1<<_ASS_ATTR_LN_NOCOM)) == 0) {
            token = strtok(line, delim);
        }

        /* Check for label declaration */
        for (i = 0; i<(int)strlen(token); i++) {
            if (token[i] == ':')
                result |= (1<<_ASS_ATTR_LN_LBDCL);
        }

        /* If there is label declaration*/
        if (result & (1 << _ASS_ATTR_LN_LBDCL)) {
            /* Checking for data declaration */
            if (strstr(token, "data") != NULL) {
                result |= (1<<_ASS_ATTR_LN_LBDAT);
            }

            return result;  /* Return early without checking for operands */
        }

        /* Check for number of operands */
        strtok(token, ndelim);
        if (strtok(NULL, ndelim) == NULL)
            result |= (1<<_ASS_ATTR_LN_ZEROP);
        else
            result |= (1<<_ASS_ATTR_LN_ONEOP);

    }
    return result;
}


/* Get the context of line in masked 8-bit format [orthogonal output] */
unsigned char get_line_context(char *line) {

   unsigned char result = 0;
   unsigned char flags = get_line_attrib(line);

   /* Check for comment context */
   if ((flags & (1 << _ASS_ATTR_LN_FLCOM)) != 0) {
       result |= (1 << _ASS_CNTXT_COMM);
       return result;
   }
    
   if ((flags & (1 << _ASS_ATTR_LN_LBDCL)) != 0) {
       result |= (1 << _ASS_CNTXT_LABL);
       return result;
   }

   if ((flags & (1 << _ASS_ATTR_LN_ONEOP)) | (flags & (1 << _ASS_ATTR_LN_ZEROP))) {
       result |= (1 << _ASS_CNTXT_INST);
       return result;
   }

   return 0;
}

/* Function for line context flags interpretation  */
int full_comment(unsigned char flags) {
    return ((flags & (1 << _ASS_CNTXT_COMM)) != 0)? 1 : 0;
}

int is_instruction(unsigned char flags) {
    return ((flags & (1 << _ASS_CNTXT_INST)) != 0)? 1 : 0;
}

int is_label(unsigned char flags) {
    return ((flags & (1 << _ASS_CNTXT_LABL)) != 0)? 1 : 0;
}

int is_directive(unsigned char flags) {
    return ((flags & (1 << _ASS_CNTXT_ADIR)) != 0)? 1 : 0;
}


/* Functions for line attributes flags interpretation */

int no_comment(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_NOCOM)) != 0)? 1 : 0;
}
int one_operand(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_ONEOP)) != 0)? 1 : 0;
}

int zero_operand(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_ZEROP)) != 0)? 1 : 0;
}

int label_declaration(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_LBDCL)) != 0)? 1 : 0;
}

int label_data_declaration(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_LBDAT)) != 0)? 1 : 0;
}

int set_pseudo_instruction(unsigned char flags) {
    return ((flags & (1 << _ASS_ATTR_LN_LBSET)) != 0)? 1 : 0;
}

/* functions for allocation of dynamic memory */
InstNode *create_inst_node(char *mnemonic, char *init_operand) {
    InstNode *node = (InstNode *)malloc(sizeof(InstNode));  /* Allocate structure on heap */

    if (node == NULL)   /* Couldn't allocate to heap */
        return NULL;
    
    node->mnemonic = mnemonic;
    node->init_operand = init_operand;

    return node;
}

SynTreeNode *create_synTree_node(unsigned long address, unsigned long line, unsigned char n_operand, InstNode *instruction, char *comment, int valid, unsigned char broadcast_code) {
    SynTreeNode *node = (SynTreeNode *)malloc(sizeof(SynTreeNode));

    if (node == NULL)   /* Couldn't allocate to heap */
        return NULL;
    
    node->address = address;
    node->line = line;
    node->n_operand = n_operand;
    node->instruction = instruction;
    node->comment = comment;
    node->valid = valid;
    node->broadcast_code = broadcast_code;

    return node;
}

SynTree *create_synTree(SynTreeNode *root) {
    SynTree *tree = (SynTree *)malloc(sizeof(SynTree));
    
    if (tree == NULL) /* Couldn't allocate to heap */
        return NULL;

    tree->root = root;
    tree->n_nodes = 0;

    return tree;
}

SymTable *create_symTable() {
    SymTable *table = (SymTable *)malloc(sizeof(SymTable));

    if (table == NULL)   /* Couldn't allocate to heap */
        return NULL;

    return table;
}

DatMem *create_datMem() {
    DatMem *datmem = (DatMem *)malloc(sizeof(DatMem));

    if (datmem == NULL) /* Couldn't allocate to heap */
        return NULL;

    return datmem;
}

/* Functions to insert or append into data structure */

/* Function to insert node into binary search tree */
int insert_synTree_node(SynTree *tree, SynTreeNode *node) {

    if (tree->root == NULL) {
        tree->root = node;
        tree->n_nodes = 1;

        return SUCCESS;
    }
    if (insert_synTree_node_handler(tree->root, node) == FAILURE)
        return FAILURE;

    tree->n_nodes++;

    return SUCCESS;
}

/* Inner function to insert in binary search tree */
int insert_synTree_node_handler(SynTreeNode *cur, SynTreeNode *node) {
    if (node->address < cur->address) {
        if (cur->left == NULL) {
            cur->left = node;
            return SUCCESS;
        } else {
            return insert_synTree_node_handler(cur->left, node);
        }
    } else {
        if (cur->right == NULL) {
            cur->right = node;
            return SUCCESS;
        } else {
            return insert_synTree_node_handler(cur->right, node);
        }
    }
}

/* Function to  find the SynTreeNode with address as key */
SynTreeNode *find_synTreeNode(SynTree *tree, unsigned long address) {
    if (tree->root == NULL)
        return NULL;

    return find_synTreeNode_handler(tree->root, address);
}

/* Inner binary search function for find_synTree_Node */
SynTreeNode *find_synTreeNode_handler(SynTreeNode *node, unsigned long address) {
    if (node == NULL)
        return NULL;

    if (node->address > address)
        return find_synTreeNode_handler(node->left, address);
    
    if (node->address < address)
        return find_synTreeNode_handler(node->right, address);

    return node;
}


/* Functions to free the heap memory to avoid memory leak */
void free_instNode(InstNode *node) {
    if (node != NULL)
        free(node);
    node = NULL;
}

void free_synTreeNode(SynTreeNode *node) {
    if (node != NULL) {
        /* Free nested structures and comments buffer  */
        if (node->instruction != NULL)
            free_instNode(node->instruction);
        if (node->comment != NULL)
            free(node->comment);
        /* Recursively free the binary search tree data structure. */
        free_synTreeNode(node->left);
        free_synTreeNode(node->right);

        free(node);
    }
    node=NULL;
}

void free_synTree(SynTree *tree) {
    if (tree->root != NULL) {
        free_synTreeNode(tree->root);
    }
    tree = NULL;
}

void free_symTable(SymTable *table) {
    if (table != NULL)
        free(table);
    table = NULL;
}

void free_datMem(DatMem *mem) {
    if (mem != NULL)
        free(mem);
    mem = NULL;
}

/* Functions for String to Integer Hashmap */

/* DJB2 hash function for string to integer */
unsigned long djb2(const char *str) {
    unsigned long _hash = 5381;
    int c;
    while ((c = *str++)) {
        _hash = ((_hash << 5) + _hash) + c;
    }
    return _hash;
}

BktNode *create_bkt_node(char *key, unsigned long value) {
    BktNode *node = (BktNode *)malloc(sizeof(BktNode));

    if (node == NULL)  /* Couldn't allocate to heap  */  
        return NULL;

    node->key = key;
    node->value = value;
    node->next = NULL;

    return node;
}

HMapBkt *create_hmap_bucket(unsigned long hash_value) {
    HMapBkt *bucket = (HMapBkt *)malloc(sizeof(HMapBkt));

    if (bucket == NULL) /* Coulnd'nt allocate to heap  */ 
        return NULL;

    bucket->hash_value = hash_value;
    bucket->head = NULL;
    bucket->bucket_size = 0;
    bucket->left = NULL;
    bucket->right = NULL;
    return bucket;
}

S2IHMap *create_s2i_hmap() {
    S2IHMap *map = (S2IHMap *)malloc(sizeof(S2IHMap));

    if (map == NULL)   /* Couldn't allocate to heap  */ 
        return NULL;

    map->root = NULL;
    
    return map;
}

/* Outer function to insert to hashmap  */
int hmap_insert(S2IHMap *map, char *key, unsigned long value) {
    if (map == NULL)
        return FAILURE;

    unsigned long _hash = djb2(key);

    if (map->root == NULL) {
        HMapBkt *bucket = create_hmap_bucket(_hash);
        if (bucket == NULL)
            return FAILURE;
        map->root = bucket;
        return hmap_insert_to_bucket(map->root, key, value);
    }
    
    return hmap_insert_handler(map->root, _hash, key, value);
}

/* Inner function to insert entries to hashmap  */
int hmap_insert_handler(HMapBkt *bucket, unsigned long hash_value, char *key, unsigned long value) {

    if (bucket == NULL)
        return FAILURE;

    if (hash_value == (bucket->hash_value)) {/* If a bucket is matched  */ 
        return hmap_insert_to_bucket(bucket, key, value);
    }

    /* Bucket lies on the left or right of the current bucket */
    if (hash_value < (bucket->hash_value)) {
        if (bucket->left == NULL) {
            bucket->left = create_hmap_bucket(hash_value);
            if (bucket->left == NULL)
                return FAILURE;
        }
        return hmap_insert_handler(bucket->left, hash_value, key, value);
    }
    
    if (bucket->right == NULL) {
        bucket->right = create_hmap_bucket(hash_value);
        if (bucket->right == NULL)
            return FAILURE;
    }
    return hmap_insert_handler(bucket->right, hash_value, key, value);
}

/* Function to insert into bucket with elements having same hash value  */
int hmap_insert_to_bucket(HMapBkt *bucket, char *key, unsigned long value) {
    if (bucket == NULL)
        return FAILURE;

    BktNode *node = create_bkt_node(key, value);
    
    if (node == NULL)
        return FAILURE;

    BktNode *cur = bucket->head;   /* If head is empty directly assign head  */ 
    if (cur == NULL)
        bucket->head = node;
    else {
        while (cur->next != NULL) {  /* Move to the end of the linked list  */ 
            /* Report if key already exists  */
            if (strcmp(cur->key, key) == 0)
                return _HMAP_ERR_KYEXST;
            cur = cur->next;
        }

        if (strcmp(cur->key, key) == 0)
            return _HMAP_ERR_KYEXST;

        cur->next = node;
    }
    
    bucket->bucket_size+=1;
    return SUCCESS;
}

BktNode *hmap_get_item_by_key(S2IHMap *map, char *key) {
    /* Returns NULL if not such key is found */
    unsigned long _hash = djb2(key);

    if (map == NULL)
        return NULL;
    if (map->root == NULL)
        return NULL;

    /* Get the correct bucket by hash_value  */
    HMapBkt *bucket = hmap_get_bucket(map->root, _hash);
    if (bucket == NULL) /* if not such bucket exist  */ 
        return NULL;

    return hmap_get_item_from_bucket(key, bucket);
}

/* Get appropriate bucket based on the hash value  */
HMapBkt *hmap_get_bucket(HMapBkt *bucket, unsigned long hash_value) {
    if (bucket == NULL)
        return NULL;

    if (bucket->hash_value == hash_value)  /* If a bucket match is found  */ 
        return bucket;
    
    /* Bucket lies in left or right child  */ 
    if (bucket->hash_value > hash_value)
        return hmap_get_bucket(bucket->left, hash_value);
    
    return hmap_get_bucket(bucket->right, hash_value);
}

/* Get the item from the bucket  */
BktNode *hmap_get_item_from_bucket(char *key, HMapBkt *bucket) {
   if (bucket == NULL)
       return NULL;
   if (bucket->head == NULL)
       return NULL;

   BktNode *cur = bucket->head;
   while (cur) {
       if (strcmp(cur->key, key) == 0)
           break;
        cur = cur->next;
   }

   return cur;
}

/* Functions to free the hashmap allocations  */
/* Free all the linked linked list nodes  */
void free_bkt_node(BktNode *node) {
    if (node == NULL)
        return;

    free_bkt_node(node->next);
    free(node);
    node = NULL;
}

/* Free the entire subtree from the bucket root  */
void free_HMapBkt(HMapBkt *bucket) {
    if (bucket == NULL)
        return;
    
    /* Before freeing the root make sure childrens are freed  */
    free_HMapBkt(bucket->left);
    free_HMapBkt(bucket->right);

    if (bucket->head != NULL)
        free_bkt_node(bucket->head);
    free(bucket);
    bucket = NULL;
}

/* Free the entire hashmap  */
void free_s2i_hashmap(S2IHMap *map) {
    if (map == NULL)
        return;
    if (map->root != NULL) {
        free_HMapBkt(map->root);
    }

    free(map);
    map = NULL;
}

int asm_systemizer(const char *filename, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map) {
    
    /* Check for file format [.asm] */
    char *cur = strstr(filename, ".asm");
    if (cur == NULL)
        return _ASS_ERR_FL_FMT;
    if (*(cur + 4) != '\0')
        return _ASS_ERR_FL_FMT;

    FILE *file = fopen(filename, "r");  /* Open the file as read mode  */ 
    if (file == NULL)
        return _ASS_ERR_FL_COF; 
    
    char line[_ASS_ATTR_FL_BUFFSZ]; /* Buffer for line  */
    unsigned long line_no = 0;

    while (fgets(line, _ASS_ATTR_FL_BUFFSZ, file) != NULL) {
        
        if (line[0] == '\n') continue;  /* Ignore white spaces  */ 

        /* Copy the line into new buffer  */
        char *copy_line = (char *)malloc(_ASS_ATTR_FL_BUFFSZ);
        memcpy(copy_line, line, _ASS_ATTR_FL_BUFFSZ);
        copy_line[_ASS_ATTR_FL_BUFFSZ - 1] = '\0';

        unsigned char context = get_line_context(copy_line);     /* Get the context of the line  */ 
        if (full_comment(context) == 0)  {                  /* if line is not all comment  */ 
            int exec_code = asm_st_constructor(line, tree, table, datmem, mnemonic_map, line_no);
            if (exec_code != SUCCESS) {
                fclose(file);
                return exec_code;
            }
        }       
        /* If line buffer gets fully filled  */
        if ((int)strlen(line) == (_ASS_ATTR_FL_BUFFSZ - 1))  {
            /* If the newline is not yet met  */
            if (line[_ASS_ATTR_FL_BUFFSZ - 2] != '\n') {
                while (fgets(line, _ASS_ATTR_FL_BUFFSZ, file)) {
                    /* Stopping condition: when new line is met [conclusion: overlong text are discarded] */
                    if ((int)strlen(line) < (_ASS_ATTR_FL_BUFFSZ - 1))  /* new line is meet before filling buffer  */ 
                        break;
                    if (line[_ASS_ATTR_FL_BUFFSZ - 2] == '\n')          /* new line is meet at buffer fill  */ 
                        break;
                }
            }
        }
        
        line_no++;
    }

    fclose(file);

}

int asm_st_constructor(char *line, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map, unsigned long line_no) {

    static unsigned long address = 0;

    char copy_line[_ASS_ATTR_FL_BUFFSZ];   /* Copy of the line to preserve the original line while tokenizing  */ 
    memcpy(copy_line, line, (unsigned long)strlen(line));
    char *comment; /* Comment: if present assing a dynamic memory to it  */ 

    unsigned char flags = get_line_attrib(copy_line);
    
    if (label_data_declaration(flags) == 1) {  /* There is label data declaration in this line  */

        char *label;
        char delim[] = " ;:\t";

        label = strtok(line, delim);    /* This gives the label name  */ 
        strtok(NULL, delim);   /* Spits out `data` text  */ 

        char *data;
        data = strtok(NULL, delim); /* Gets the actual data value  */ 

        char *data_end = data; /* Find the end pointer of data string as required by strtol function  */ 
        while (*data_end != '\0')
            data_end++;
        
        unsigned long nval = strtol(data, data_end, 10);
        if (data_end == data) {  /* No conversion is performed  */ 
            return _ASS_ERR_INV_OPRND; 
        }

        return hmap_insert(table, label, nval);
    }

    if (label_declaration(flags) == 1) {   /* There is a label declaration in this line  */ 

        char *token;
        char delim[] = " ;:\t";
        token = strtok(line, delim);

        if (token == NULL)
            return _ASS_ERR_INV_OPCOD;
        
        char *token_t = (char *)malloc(_ASS_ATTR_BF_HMAPKY);
        if (token_t != NULL) {
            unsigned long i;
            for (i = 0; i<_ASS_ATTR_BF_HMAPKY; i++) {
                token_t[i] = token[i];
                if (token[i] == '\0')
                    break;
            }
    
            token_t[_ASS_ATTR_BF_HMAPKY - 1] = '\0';
            token = token_t;
        }
        return  hmap_insert(table, token, (unsigned long)address);
    }

    if (no_comment(flags) == 0) { /* If there is comment present  */ 

        char *cur = line;
        while (*cur != ';')
            cur++;

        char *comment_t = (char *)malloc(_ASS_ATTR_BF_COMLIM);
        if (comment_t != NULL) {
            unsigned long i;
            for (i = 0; i<_ASS_ATTR_BF_COMLIM; i++) {
                if (cur[i] == '\0')
                    break;
                comment_t[i] = cur[i];
            }

            comment_t[_ASS_ATTR_BF_COMLIM - 1] = '\0';
            comment = comment_t;
        }
    }

    /* There is instruction in the line  */
    
    unsigned long i;
    char *mnemonic; 
    char *operand;

    char delim[] = " ;\t";
    mnemonic = strtok(line, delim);
    
    while (mnemonic == ' ') mnemonic++;

    if (one_operand(flags) == 1) { /* There is one operand instruction in the line  */ 

        operand = strtok(NULL, delim);
        char *operand_t  = (char *)malloc(_ASS_ATTR_BF_OPERND);

        for (i = 0; i<_ASS_ATTR_BF_OPERND; i++) {
            operand_t[i] = operand[i];
            if (operand[i] == '\0')
                break;
        }
        operand_t[_ASS_ATTR_BF_OPERND - 1] = '\0';
        operand = operand_t;

    } else {
        operand = NULL;
    }

    char *mnemonic_t = (char *)malloc(_ASS_ATTR_BF_MNEMON);
  
    for (i = 0; i<_ASS_ATTR_BF_MNEMON; i++) {
        mnemonic_t[i] = mnemonic[i];
        if (mnemonic[i] == '\0')
            break;
    }

    mnemonic_t[_ASS_ATTR_BF_MNEMON - 1] = '\0';
    mnemonic = mnemonic_t;

    InstNode *instruction = create_inst_node(mnemonic, operand);
    SynTreeNode *node = create_synTree_node(address, line_no, one_operand(flags), instruction, comment, 1, SUCCESS);

    int code = insert_synTree_node(tree, node);
    if (code == SUCCESS)
        address+=1;

    return code;
}


/* Function to get the mnemonic hashmap  */
S2IHMap *get_mnemonic_map() {
    S2IHMap *map = create_s2i_hmap();

    if (map == NULL)
        return NULL;

    hmap_insert(map, "ldc",     0);
    hmap_insert(map, "adc",     1);
    hmap_insert(map, "ldl",     2);
    hmap_insert(map, "stl",     3);
    hmap_insert(map, "ldnl",    4);
    hmap_insert(map, "stnl",    5);
    hmap_insert(map, "add",     6);
    hmap_insert(map, "sub",     7);
    hmap_insert(map, "shl",     8);
    hmap_insert(map, "shr",     9);
    hmap_insert(map, "adj",     10);
    hmap_insert(map, "a2sp",    11);
    hmap_insert(map, "sp2a",    12);
    hmap_insert(map, "call",    13);
    hmap_insert(map, "return",  14);
    hmap_insert(map, "brz",     15);
    hmap_insert(map, "brlz",    16);
    hmap_insert(map, "br",      17);
    hmap_insert(map, "HALT",    18);

    return map;
}


void judge_instructions(SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map) { /* Makes final pass through all the nodes to find errors  */ 
   if (tree == NULL)
       return;

   return judge_instructions_handler(tree->root, table, datmem, mnemonic_map);
}


void judge_instructions_handler(SynTreeNode *node, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map) {  /* Inner recursive function for judge_instructions()  */ 
    if (node == NULL)
        return;
    
    judge_instructions_handler(node->left, table, datmem, mnemonic_map);
    judge_instructions_handler(node->right, table, datmem, mnemonic_map);

    if (node->instruction == NULL) {
        node->valid = 0;
        node->broadcast_code = _ASS_ERR_MIS_INSTR;
        return;
    }
    
    char *mnemonic = (node->instruction)->mnemonic;
    BktNode *bkt_node = hmap_get_item_by_key(mnemonic_map, mnemonic);
    if (bkt_node == NULL) {
        node->valid = 0;
        node->broadcast_code = _ASS_ERR_INV_OPCOD;
        return;
    }
    (node->instruction)->op_code = bkt_node->value;
    
    if (node->n_operand == 1) {
        char *operand = (node->instruction)->init_operand;

        if ((*operand <= '9') && (*operand >= '0')) {
            /* Operand is a value, parse it  */ 
        } else {
            /* Operand is a symbol, fetch it */
            BktNode *symbol = hmap_get_item_by_key(table, operand);
            if (symbol == NULL) {
                node->valid = 0;
                node->broadcast_code = _ASS_ERR_UDC_LABEL; 
                return;
            }
            (node->instruction)->operand = symbol->value;
        }
    } else {
        unsigned char opcode = (node->instruction)->op_code;
        if (!(
                (opcode>=6) ||
                (opcode<=9) ||
                (opcode>=11) ||
                (opcode<=12) ||
                (opcode==14) ||
                (opcode==18)
           )) {
            node->valid = 0;
            node->broadcast_code = _ASS_ERR_MIS_OPRND; 
        }
    }
}

/* Dump functions for structures [only for debugging purposes] */

/* Dump function for InstNode */
void dump_instruction_node(const InstNode *node) {
    if (node == NULL) {
        printf("Instruction Node: NULL\n");
        return;
    }
    printf("Instruction Node:\n");
    printf("  Mnemonic: %s\n", node->mnemonic ? node->mnemonic : "NULL");
    printf("  OP Code: %u\n", node->op_code);
    printf("  Initial Operand: %s\n", node->init_operand ? node->init_operand : "NULL");
    printf("  Operand: %d\n", node->operand);
}

/* Dump function for SynTreeNode */
void dump_syntax_tree_node(const SynTreeNode *node) {
    if (node == NULL) {
        printf("Syntax Tree Node: NULL\n");
        return;
    }
    printf("Syntax Tree Node:\n");
    printf("  Address: %lu\n", node->address);
    printf("  Line: %lu\n", node->line);
    printf("  Number of Operands: %u\n", node->n_operand);
    printf("  Valid: %s\n", node->valid ? "true" : "false");
    printf("  Broadcast Code: %u\n", node->broadcast_code);
    printf("  Comment: %s\n", (node->comment != NULL) ? node->comment : "NULL");

    if (node->instruction != NULL) {
        dump_instruction_node(node->instruction);
    }

    printf("  Left Child:\n");
    dump_syntax_tree_node(node->left);
    printf("  Right Child:\n");
    dump_syntax_tree_node(node->right);
}

/* Dump function for SynTree */
void dump_syntax_tree(const SynTree *tree) {
    if (tree == NULL) {
        printf("Syntax Tree: NULL\n");
        return;
    }
    printf("Syntax Tree:\n");
    printf("  Number of Nodes: %lu\n", tree->n_nodes);
    printf("  Root Node:\n");
    dump_syntax_tree_node(tree->root);
}

/* Dump function for hash table  */
void dump_hash_table(const S2IHMap *map) {
    if (map == NULL) {
        printf("Hash Map: NULL \n");
        return;
    }

    printf("\nHash Map: \n");
    printf("Root: ");
    dump_hashmap_bucket(map->root);
}

/* Dump Hash map bucket  */
void dump_hashmap_bucket(const HMapBkt *bucket) {
    if (bucket == NULL) {
        printf("Bucket: NULL\n");
        return;
    }

    printf("Left: \n");
    dump_hashmap_bucket(bucket->left);

    printf("Right: \n");
    dump_hashmap_bucket(bucket->right);

    printf("Bucket: %lu\n", bucket->hash_value);
    BktNode *node = bucket->head;
    while (node) {
        dump_bucket_node(node);
        node = node->next;
    }
}

/* Dump hash map bucket node  */
void dump_bucket_node(const BktNode *node) {
    if (node == NULL) {
        printf("BktNode: NULL\n");
        return;
    }

    printf("key: %s, value: %lu\n", node->key, node->value);
}

