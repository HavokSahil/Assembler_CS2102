/*
Author: Sahil Raj
File Name: Utils.c
Course Project [CS2102]: Roll -> 2301CS41
 */

#include <string.h>
#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
        while ((*ptr == ' ') || (*ptr == '\t')) ptr++;
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
            if (strstr(token, "SET") != NULL) {
                result |= (1<<_ASS_ATTR_LN_LBSET);
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
        while (cur != NULL) {  /* Move to the end of the linked list  */ 
            /* Report if key already exists  */
            if (strcmp(cur->key, key) == 0)
                return _HMAP_ERR_KYEXST;
            cur = cur->next;
        }

        node->next = bucket->head;
        bucket->head = node;
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

int asm_systemizer(const char *filename, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map, AMsgList *msglist) {
    
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
            int exec_code = asm_st_constructor(line, tree, table, datmem, mnemonic_map, msglist, line_no);
            if (exec_code == _HMAP_ERR_KYEXST) {   /* Label repeat error  */ 
               insert_message(msglist, _ASS_ERR_DUP_LABEL, _ASS_MSG_LVL_ERR, line_no);
               continue;
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

    return SUCCESS;
}

int asm_st_constructor(char *line, SynTree *tree, SymTable *table, DatMem *datmem, S2IHMap *mnemonic_map, AMsgList *msglist, unsigned long line_no) {

    static unsigned long address = 0;

    char copy_line[_ASS_ATTR_FL_BUFFSZ];   /* Copy of the line to preserve the original line while tokenizing  */ 
    memcpy(copy_line, line, 1+(unsigned long)strlen(line));
    char *comment; /* Comment: if present assing a dynamic memory to it  */ 

    unsigned char flags = get_line_attrib(copy_line);
    
    if (label_data_declaration(flags) == 1) {  /* There is label data declaration in this line  */

        char *label;
        char delim[] = " ;:\t";

        label = strtok(line, delim);    /* This gives the label name  */ 
        char *label_t = (char *)malloc(_ASS_ATTR_BF_HMAPKY);
        if (label_t != NULL) {
            unsigned long i;
            for (i = 0; i<_ASS_ATTR_BF_HMAPKY; i++) {
                label_t[i] = label[i];
                if (label[i] == '\0')
                    break;
            }
    
            label_t[_ASS_ATTR_BF_HMAPKY - 1] = '\0';
            label = label_t;
        }

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
     if (set_pseudo_instruction(flags) == 1) {  /* There is label SET directive declaration in this line  */

        char *label;
        char delim[] = " ;:\t\n";

        label = strtok(line, delim);    /* This gives the label name  */ 
        char *label_t = (char *)malloc(_ASS_ATTR_BF_HMAPKY);
        if (label_t != NULL) {
            unsigned long i;
            for (i = 0; i<_ASS_ATTR_BF_HMAPKY; i++) {
                label_t[i] = label[i];
                if (label[i] == '\0')
                    break;
            }
    
            label_t[_ASS_ATTR_BF_HMAPKY - 1] = '\0';
            label = label_t;
        }

        strtok(NULL, delim);   /* Spits out `data` text  */ 
        char *data;
        data = strtok(NULL, delim); /* Gets the actual data value  */ 

        char *data_end = data; /* Find the end pointer of data string as required by strtol function  */ 
        while (isdigit(*data_end))
            data_end++;
        
        unsigned long nval = strtol(data, data_end, 10);
        if (data_end == data) {  /* No conversion is performed  */ 
            return _ASS_ERR_INV_OPRND; 
        }

        return hmap_insert(table, label, nval);
    }

    if (label_declaration(flags) == 1) {   /* There is a label declaration in this line  */ 

        char *token;
        char delim[] = " ;:\t\n";
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
        cur++;
        unsigned long sz = (_ASS_ATTR_BF_COMLIM > (strlen(cur) + 1))? strlen(cur) + 1: _ASS_ATTR_BF_COMLIM;
        char *comment_t = (char *)malloc(sz);
        if (comment_t != NULL) {
            unsigned long i;
            for (i = 0; i<sz; i++)
                if (cur[i] == '\n')
                    comment_t[i] = '\\';
                else
                    comment_t[i] = cur[i];
            }

            comment_t[sz - 1] = '\0';
            comment = comment_t;
    } else {
            comment = NULL;
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
        unsigned long sz = (strlen(operand) + 1 < _ASS_ATTR_BF_OPERND)? strlen(operand) + 1: _ASS_ATTR_BF_OPERND;
        char *operand_t  = (char *)malloc(sz);
        for (i = 0; i<sz; i++) {
            operand_t[i] = operand[i];
        }

        operand_t[sz-1] = '\0';
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
        address+=4;

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

        if (isdigit(*operand) || (*operand == '-')) {
            /* Operand is a value, parse it  */
            char *end_ptr = operand;
            while (!isdigit(*end_ptr)) {
                end_ptr++;
            }
            while (isdigit(*end_ptr)) {
                end_ptr++;
            }

            char *null_term = end_ptr; /* Store the position of null terminator of the init_operand  */ 
            long int value = strtol(operand, end_ptr, 10);

            if (operand == end_ptr) {     /* no conversion has been done  */ 
                node->valid = 0;  
                return;     
            }

            *null_term = '\0'; /* Recover the null terminator of the init_operand  */ 
            (node->instruction)->operand = value;
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

/* Functions for assembler messages */
AMsg *create_new_message(unsigned char code, unsigned char severity, int line_no) {
    AMsg *msg = (AMsg *)malloc(sizeof(AMsg));

    if (msg == NULL)
        return NULL;

    msg->code = code;
    msg->severity = severity;
    msg->line_no = line_no;
    msg->next = NULL;

    return msg;
}

AMsgList *create_new_amsg_list() {
    AMsgList *msglist = (AMsgList *)malloc(sizeof(AMsgList));
    
    if (msglist == NULL)
        return NULL;

    msglist->msgs = NULL;
    msglist->size = 0;

    return msglist;
}

int insert_message(AMsgList *msglist, unsigned char code, unsigned char severity, int line_no) {
    AMsg *msg = create_new_message(code, severity, line_no);
    if (msg == NULL)
        return FAILURE;

    if (msglist->msgs == NULL) {
        msglist->msgs = msg;
        return SUCCESS;
    }
    msg->next = msglist->msgs;
    msglist->msgs = msg;
    return SUCCESS;
}

void print_message_list(AMsgList *mlist, FILE *stream) {
    if (mlist == NULL)          return;
    if (mlist->msgs == NULL)    return;
    
    fprintf(stream, "\nWarnings and Errors:\n");
    fprintf(stream, "%-*s %-*s %-*s %-*s\n", 6, "Line", 10, "Severity", 6,"Code", 32, "Description");
    fprintf(stream, "----------------------------------------\n");

    AMsg *cur = mlist->msgs;
    while (cur) {

        fprintf(stream, "%-*ld ", 6, (unsigned long)cur->line_no);

        if (cur->severity == _ASS_MSG_LVL_INF)
            fprintf(stream, "%-*s ", 10,"INFO");
        else if (cur->severity == _ASS_MSG_LVL_ERR)
            fprintf(stream, "%-*s ", 10, "ERROR");
        else
            fprintf(stream, "%-*s", 10, "WARNING");

        fprintf(stream, "%04X" ,cur->code);

        switch (cur->code) {
            case _ASS_ERR_INV_OPCOD:
                fprintf(stream, "\t %-*s\n", 32, "Invalid Op-Code.");
                break;
            case _ASS_ERR_UDC_LABEL:
                fprintf(stream, "\t %-*s\n", 32, "Undeclared Label.");
                break;
            case _ASS_ERR_MIS_OPRND:
                fprintf(stream, "\t %-*s\n", 32, "Missing Operand.");
                break;
            case _ASS_ERR_EXT_OPRND:
                fprintf(stream, "\t %-*s\n", 32, "Extra Operand.");
                break;
            case _ASS_ERR_DUP_LABEL:
                fprintf(stream, "\t %-*s\n", 32, "Duplicate Label.");
                break;
            case _ASS_ERR_INV_OPRND:
                fprintf(stream, "\t %-*s\n", 32, "Invalid Operand.");
                break;
            case _ASS_ERR_MIS_INSTR:
                fprintf(stream, "\t %-*s\n", 32, "Missing Instruction.");
                break;
            default:
                break;
        }

        cur = cur->next;
    }
}

int generate_advanced_listing_file(const char *filename, SynTree *tree, SymTable *table, AMsgList *msglist) {
    FILE *output = fopen(filename, "w");

    if (output == NULL)
        return FAILURE;

    dump_syntax_tree(tree, output);
    dump_hash_table(table, output);
    print_message_list(msglist, output);

    fclose(output);
    return SUCCESS;
}

/* Check for errors and populate instructions  */
int check_error(SynTree *tree, AMsgList *msglist, InstrList *ilist) {
    if (tree == NULL)
        return SUCCESS;

    return check_error_handler(tree->root, msglist, ilist);
}

int check_error_handler(SynTreeNode *node, AMsgList *msglist, InstrList *ilist) {
    if (node == NULL)
        return SUCCESS;
    
    int res = check_error_handler(node->right, msglist, ilist);

    if (node->valid == 0) { /* There is some error in this node  */ 
        insert_message(msglist, node->broadcast_code, _ASS_MSG_LVL_ERR, node->line);
        res = FAILURE;
    } else {
        insert_machine_instruction(ilist, (node->instruction)->op_code, (node->instruction)->operand, node->n_operand);
    }
    
    res |= check_error_handler(node->left, msglist, ilist);

    return res;
}

/* The function that returns final instruction from op-code and operand  */
uint32_t get_machine_instruction(int32_t operand, uint8_t opcode, unsigned char n_operand) {
    if (n_operand == 0) {
        return (0xFF)&opcode;
    }

    if (operand<-8388608) operand = -8388608;
    if (operand>8388607) operand = 8388607;

    return ((0xFFFFFF & operand)<<8) | (0xFF & opcode);
}

/* Functions for handling machine instructions and dumping it  */
MacInstr *create_new_machine_instruction(unsigned char op_code, long int operand, unsigned char n_operand) {
    MacInstr *macinstr = (MacInstr *)malloc(sizeof(macinstr));
    
    if (macinstr == NULL)
        return NULL;

    macinstr->instruction_word = get_machine_instruction(operand, op_code, n_operand);
    macinstr->next = NULL;

    return macinstr;
}

/* Insert machine instruction MacInstr structure in instruction list structure linked list  */
int insert_machine_instruction(InstrList *ilist, unsigned char op_code, long int operand, unsigned char n_operand) {
    if (ilist == NULL)
        return FAILURE;
    
    MacInstr *macinstr = create_new_machine_instruction(op_code, operand, n_operand);

    if (macinstr == NULL)
        return FAILURE;

    if (ilist->intrs == NULL) {
        ilist->intrs = macinstr;
        return SUCCESS;
    }

    macinstr->next = ilist->intrs;
    ilist->intrs = macinstr;

    return SUCCESS;
}

/* Allocate memory to new instruction  */
InstrList *create_new_instruction_list() {
    InstrList *ilist = (InstrList *)malloc(sizeof(InstrList));
    return ilist;
}

/* Output the instruction into stream  */
void print_machine_instruction(InstrList *ilist, FILE *stream) {
    if (ilist == NULL)
        return;

    MacInstr *macinstr = ilist->intrs;
    while (macinstr) {
        uint32_t instruction_word = macinstr->instruction_word;

        unsigned char bytes[4] = {
            (instruction_word>>24)  &0xFF,
            (instruction_word>>16)  &0xFF,
            (instruction_word>>8)   &0xFF,
            (instruction_word)     &0xFF,
        };

        fwrite(bytes, sizeof(bytes), 1, stream);
        macinstr = macinstr->next;
    }
}

/* Dump functions for structures [only for debugging purposes] */

/* Dump function for SynTreeNode */
void dump_syntax_tree_node(const SynTreeNode *node, FILE *stream) {
    if (node == NULL) {
        return;
    }

    dump_syntax_tree_node(node->left, stream);

    char *comment = (node->comment == NULL)? " ": node->comment;
    fprintf(stream, "%-*ld %08X \t  %08X \t\t\t\t %-*s %-*s %-*s\n", 8, node->line, (unsigned int)node->address, get_machine_instruction((node->instruction)->operand, (node->instruction)->op_code, node->n_operand), 6, (node->instruction)->mnemonic, 10, ((node->instruction)->init_operand == NULL)? " ": (node->instruction)->init_operand, 64, comment);
    dump_syntax_tree_node(node->right, stream);
}

/* Dump function for SynTree */
void dump_syntax_tree(const SynTree *tree, FILE *stream) {
    if (tree == NULL) {
        return;
    }
    fprintf(stream, "Advanced Listing Table: \n");
    fprintf(stream, "\nInstruction Table: \n");
    fprintf(stream, "%-*s %-*s %-*s %-*s %-*s %-*s\n", 10, "Line", 10, "Memory", 16, "Machine Code", 6, "Assembly",10, "Code", 64, "Comments");
    fprintf(stream, "-----------------------------------------------------------------------\n");
    dump_syntax_tree_node(tree->root, stream);
}

/* Dump function for hash table  */
void dump_hash_table(const S2IHMap *map, FILE *stream) {
    if (map == NULL) {
        return;
    }
    
    fprintf(stream, "\nSymbol Table:\n");
    fprintf(stream, "%-*s %-*s\n", 20, "Symbols", 16, "Address");
    fprintf(stream, "------------------------------------\n");
    dump_hashmap_bucket(map->root, stream);
}

/* Dump Hash map bucket  */
void dump_hashmap_bucket(const HMapBkt *bucket, FILE *stream) {
    if (bucket == NULL) {
        return;
    }

    dump_hashmap_bucket(bucket->left, stream);
    BktNode *node = bucket->head;
    while (node) {
        dump_bucket_node(node, stream);
        node = node->next;
    }
    dump_hashmap_bucket(bucket->right, stream);
}

/* Dump hash map bucket node  */
void dump_bucket_node(const BktNode *node, FILE *stream) {
    if (node == NULL) {
        return;
    }
    fprintf(stream, "%-*s %08X\n", 20, node->key, (unsigned int)node->value);
}

