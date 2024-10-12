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
        char ndelim[] = " ";
        
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


/* functions for allocation of dynamic memory */
InstNode *create_inst_node(unsigned char op_code, char *init_operand) {
    InstNode *node = (InstNode *)malloc(sizeof(InstNode));  /* Allocate structure on heap */

    if (node == NULL)   /* Couldn't allocate to heap */
        return NULL;
    
    node->op_code = op_code;
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

DatMem *create_DatMem() {
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
        if (node->instruction != NULL)
            free_instNode(node->instruction);
        
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

    if (hash_value == bucket->hash_value) {/* If a bucket is matched  */ 
        return hmap_insert_to_bucket(bucket, key, value);
    }

    /* Bucket lies on the left or right of the current bucket */
    if (hash_value < bucket->hash_value) {
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
