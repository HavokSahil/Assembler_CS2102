/**
 * Common Data Structures
 * --------------------------------------------------------
 *  This section contains data structures that are shared
 *  across various modules.
 *
 *  Note:
 *  - Each Data Structure is modular and easily extendable.
 *  - Each Data Structure implements `insert`, `delete`,
 *  	`find`, `empty`, `size`.
 *  - `find` returns .end() if element is not found.
 *  - Each Data Structure stores `void *` as value for
 *  	usage across multiple context using typecast.
 *
 *  Data Structures List (Globally Available):
 * --------------------------------------------------------
 *	- `SymTable`: (Symbol Table) Stores the `label` with
 *		its values.
 *
 *	- `IList`: (Instrcution List) Stores the `IItem`.
 *
 *	- `IItem`: (Instruction Item) Stores the instructions.
 *
 *	- `DList`: (Data List) Stores the all `DItem` initialised.
 *
 *	- `DItem`: (Data Item) Stores the data with address and
 *		value.
 *
 *	- `EWList`: (Error/Warning List) Stores the Errors and
 *		Warnings as `EWItem`.
 *
 *	- `EWItem`: (Error/Warning Item) Stores the Error and
 *		Warning Item.
 * 
 *	Data Structures List (Locally Available):
 * --------------------------------------------------------
 *	- `_ds_queue`: The usual Queue data structure.
 *
 *	- `_ds_smap`: The String hashmap; a map with String hash
 *		function.
 *
 *	- `_ds_map`: The usual ordered map.
 *
 *	Nodes for Data Local Data Structures.
 * --------------------------------------------------------
 *	- `_ds_queue_node`: The nodes for queue data structure.
 *	- `_ds_smap_bucket`: The bucker node for string hashmap.
 *	- `_ds_smap_node`: The node for string hash map.
 *	- `_ds_map_node`: The node for map.
 *
 *	Local Functions for Local Data Structures
 *	- `_ds_get_queue_node`: Allocate queue node in heap.
 *	- `_ds_get_queue`: Allocate queue in heap.
 *	- `_ds_free_queue_node`: Free the queue node from heap.
 *	- `_ds_free_queue`: Free the queue from heap.
 *	- `_ds_queue_insert`: Insert into queue.
 *	- `_ds_queue_empty`: Checks if queue is empty or not.
 *	- `_ds_queue_size`: Returns the size of queue.
 *	- `djb2`: String to integer hash function.
 *	- `_ds_get_smap_node`: Allocate the smap node in heap.
 *	- `_ds_get_smap_bucket`: Allocate the smap bucket in
 *		heap.
 *	- `_ds_get_smap`: Allocate the smap in heap.
 *	- `_ds_free_smap_node`: Deallocate the smap node in heap.
 *	- `_ds_free_smap_bucket`: Deallocate the smap bucket in
 *		heap.
 *	- `_ds_free_smap`: Deallocate the smap in heap.
 *	- `_ds_smap_bucket_insert`: Insert into the bucket with
 *		matched hash.
 *	- `_ds_smap_node_rot_right`: Right rotate the node.
 *	- `_ds_smap_node_rot_left`: Left rotate the node.
 *	- `_ds_smap_fixup`: Fix the violation in the red-black
 *		tree.
 *	- `_ds_smap_insert_handler`: Recursive insertion handler
 *		for insert `_ds_smap_insert_handler`
 *	- `_ds_smap_insert`: Function to insert into Hasp map.
 *	- `_ds_smap_find_handler`: Recursive find handler for
 *		`_ds_smap_find`.
 *	- `_ds_smap_find`: Function to find for a key in Hashmap.
 *	- `_ds_smap_empty`: Function to check if the Hashmap is
 *		empty.
 *	- `_ds_smap_size`: Function to get the number of elements
 *		present in the Hash map.
 *
 *********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <common_types.h>
#include "common_ds.h"
#include <err_codes.h>

/* --------------------------------------------------------
 * Structures for Local Data Structures
 * --------------------------------------------------------*/
	
/*  */
struct _ds_queue_node_struct {
	void *data;
	struct _ds_queue_node *next;
	struct _ds_queue_node *prev;
};

typedef struct _ds_queue_node_struct _ds_queue_node;

/*  */
struct _ds_queue_struct {
	_ds_queue_node *front;
	_ds_queue_node *back;
	ASize size;
};

typedef struct _ds_queue_struct _ds_queue;

/*  */
struct _ds_smap_node_struct {
	AString key;
	void *data;
	struct _ds_smap_node_struct *next;
	struct _ds_smap_node_struct *prev;
};

typedef struct _ds_smap_node_struct _ds_smap_node;

/*  */
struct _ds_smap_bucket_struct {
	AAddr key;
	_ds_smap_node *head;
	ASize size;

	struct _ds_smap_bucket_struct *parent;
	struct _ds_smap_bucket_struct *left;
	struct _ds_smap_bucket_struct *right;
	Color color;
};

typedef struct _ds_smap_bucket_struct _ds_smap_bucket;

/*  */
struct _ds_smap_struct {
	_ds_smap_bucket *root;
	ASize size;
};

typedef struct _ds_smap_struct _ds_smap;


/*  */
struct _ds_map_node_struct {
	AAddr key;
	void *data;

	struct _ds_map_node_struct *parent;
	struct _ds_map_node_struct *left;
	struct _ds_map_node_struct *right;
	Color color;
};

typedef struct _ds_map_node_struct _ds_map_node;

/*  */
struct _ds_map_struct {
	_ds_map_node *root;
	ASize size;
};

typedef struct _ds_map_struct _ds_map;

/**
 * The Functions for Queue Data Structure  
 * -----------------------------------------*/

_ds_queue_node* _ds_get_queue_node(void* data) {	/* Function to allocate queue node */
	_ds_queue_node *node = (_ds_queue_node*)malloc(sizeof(_ds_queue_node));

	if (node == NULL)
		return NULL;

	node->data = data;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

_ds_queue* _ds_get_queue() {	/* Function to allocate queue   */
	_ds_queue *queue = (_ds_queue*)malloc(sizeof(_ds_queue));

	if (queue == NULL)
		return NULL;

	queue->front = NULL;
	queue->back  = NULL;
	queue->size  = 0;

	return queue;
}

static void _ds_free_queue_node(_ds_queue_node* node, ABool wipedata) {	/* Function to free queue node from heap */
	if (node == NULL)
		return;

	if ((node->data != NULL) && (wipedata = TRUE))	/* Free the inner data */
		free(node->data);

	free(node);							/* Free the queue node structure */
	node = NULL;
}

static void _ds_free_queue(_ds_queue* queue, ABool wipedata) {
	if (queue == NULL)
		return;

	if (queue->front != NULL) {
		_ds_queue_node *prev = NULL;
		_ds_queue_node *cur  = queue->front;
		
		while (cur != NULL) {
			prev = cur;
			cur = cur->next;
			_ds_free_queue_node(prev, wipedata);
		}
	}

	free(queue);
	queue = NULL;
}

AErr _ds_queue_insert(_ds_queue* queue, void* data) {	/* Function to insert into queue  */
	if (queue == NULL)
		return ERR_DS_INVALID_STRUCT;

	_ds_queue_node* node = _ds_get_queue_node(data);
	if (node == NULL)
		return ERR_DS_STRUCT_GEN_FAIL;

	if (queue->size == 0) {	/* Queue is empty  */
		queue->front = node;
		queue->back  = node;
		queue->size = 1;

		return SUCCESS;
	}
	
	queue->back->next = node;
	node->prev = queue->back;
	queue->back = node;
	queue->size += 1;

	return SUCCESS;
}

ABool _ds_queue_empty(_ds_queue* queue) {
	if (queue == NULL)
		return TRUE;

	return (queue->size == 0);
}

ASize _ds_queue_size(_ds_queue* queue) {
	if (queue == NULL)
		return 0;

	return (queue->size);
}

void _ds_queue_print(_ds_queue* queue) {
	if (queue == NULL)
		return;

	int i;
	_ds_queue_node* node = queue->front;
	for (i = 0; i<_ds_queue_size(queue); i++) {
		if (node == NULL)
			break;
		printf("(%d/%d)%d ", i, _ds_queue_size(queue),  node);
	}
	printf("\n");
}

/**
 * The Functions for String Hashmap Data Structure  
 * -----------------------------------------------*/

/* DJB2 hash function for string to integer */
AAddr djb2(const AString str) {
	AString cur = str;
	AAddr _hash = 5381;
  AInt c;
  while ((c = *cur++)) {
  	_hash = ((_hash << 5) + _hash) + c;
  }
  return _hash;
}

_ds_smap_node* _ds_get_smap_node(AString key, void* data) {	/* Function to allocate SMap node into heap */
	_ds_smap_node* node = (_ds_smap_node*)malloc(sizeof(_ds_smap_node));

	if (node == NULL)
		return NULL;
	
	node->key = key;
	node->data = data;
	node->next = NULL;	
	node->prev = NULL;
	return node;
}

_ds_smap_bucket* _ds_get_smap_bucket(AAddr key) {	/* Function to allocate SMap Bucket structure in heap   */
	_ds_smap_bucket* bucket = (_ds_smap_bucket*)malloc(sizeof(_ds_smap_bucket));

	if (bucket == NULL)
		return NULL;

	bucket->key = key;
	bucket->head = NULL;
	bucket->size = 0;
	bucket->parent = NULL;
	bucket->left = NULL;
	bucket->right = NULL;
	bucket->color = RED;

	return bucket;
}

_ds_smap* _ds_get_smap() {	/* Function to allocate SMap structure in heap  */
	_ds_smap* smap = (_ds_smap*)malloc(sizeof(_ds_smap));

	if (smap == NULL)
		return NULL;

	smap->root = NULL;
	smap->size = 0;

	return smap;
}

static void _ds_free_smap_node(_ds_smap_node* node) {		/* Function to deallocate the smap node   */
	if (node == NULL) 
		return;

	if (node->data != NULL)
		free(node->data);

	free(node);
	node = NULL;
}

static void _ds_free_smap_bucket(_ds_smap_bucket* bucket) {	/* Function to deallocate the smap bucket   */
	if (bucket == NULL)
		return;

	if (bucket->head != NULL) {
		_ds_smap_node* prev = NULL;
		_ds_smap_node* cur = bucket->head;

		int i;
		for (i = 0; i<bucket->size; i++) {
			prev = cur;
			cur = cur->next;
			_ds_free_smap_node(prev);
		}
	}

	free(bucket);
	bucket = NULL;
}

static void _ds_free_smap_handler(_ds_smap_bucket* bucket) {	/* Function to deallocate the smap recursively  */
	if (bucket == NULL)
		return;

	_ds_free_smap_handler(bucket->left);
	_ds_free_smap_handler(bucket->right);
	_ds_free_smap_bucket(bucket);
	bucket = NULL;
}

static void _ds_free_smap(_ds_smap* smap) {		/* Function to deallocate the smap   */
	if (smap == NULL)
		return;

	_ds_free_smap_handler(smap->root);
}

static void _ds_smap_bucket_insert(_ds_smap_bucket* bucket, _ds_smap_node* node) {
	if (bucket->head == NULL)
		bucket->head = node;

	node->next = bucket->head;
	bucket->head->prev = node;
	bucket->head = node;
	bucket->size += 1;
}

static void _ds_smap_node_rot_right(_ds_smap* smap, _ds_smap_bucket* y) {
	_ds_smap_bucket* x = y->left;
	y->left = x->right;

	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == NULL)
		smap->root = x;
	else if (y == y->parent->right)
		y->parent->right = x;
	else
		y->parent->left = x;

	x->right = y;
	y->parent = x;
}

static void _ds_smap_node_rot_left(_ds_smap* smap, _ds_smap_bucket *x) {

	_ds_smap_bucket* y = x->right;
	x->right = y->left;

	if (y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		smap->root  = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

static void _ds_smap_fixup(_ds_smap* smap, _ds_smap_bucket* bucket) {

	if (bucket == NULL)
		return;

	_ds_smap_bucket* z = bucket;

	while (z != smap->root) {

		if (z->parent->color != RED)
			break;
		
		if (z->parent == z->parent->parent->left) {	/* Parent is left child   */
			_ds_smap_bucket* y = z->parent->parent->right; 		/* The uncle   */

			if ((y != NULL) && (y->color == RED)) {	/* Case I: Uncle is RED */
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
						/* Case II: z is right child  */
						z = z->parent;
						_ds_smap_node_rot_left(smap, z);
				}
				/* Case III: z is left child  */
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_ds_smap_node_rot_right(smap, z->parent->parent);
			}
		} else {
		/* Mirror Case  */
			_ds_smap_bucket* y = z->parent->parent->left;

			if ((y != NULL) && (y->color == RED)) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					_ds_smap_node_rot_right(smap, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_ds_smap_node_rot_left(smap, z->parent->parent);
			}
		}
	}
	smap->root->color = BLACK;
}

static void _ds_smap_print(_ds_smap_bucket* bucket) {
	if (bucket->left != NULL)
		_ds_smap_print(bucket->left);
	printf("DEBUG: key->%d, string->%s\n", bucket->key, bucket->head->key);
	if (bucket->right != NULL)
		_ds_smap_print(bucket->right);
}

static _ds_smap_bucket* _ds_smap_insert_handler(const _ds_smap* map, _ds_smap_bucket* bucket, AAddr key, _ds_smap_node* node) {
	if (bucket == NULL)	/* Hack Fix, neex to revisit the issue of NULL bucket when the bucket reaches root after fixup.   */
		bucket = map->root;

	if (bucket->key == key) {

		 _ds_smap_bucket_insert(bucket, node);		/* Insert the node into the bucket */
		 return bucket;

	} else if (bucket->key < key) {	/* Key bucket lies in right */
		if (bucket->right == NULL) {		/* If bucket non-existent, create new bucket */
			_ds_smap_bucket *rbucket = _ds_get_smap_bucket(key);

			if (rbucket == NULL)
				return NULL;
			
			rbucket->parent = bucket;
			bucket->right = rbucket;
			_ds_smap_fixup(map, rbucket);
		}

		return _ds_smap_insert_handler(map, bucket->right, key, node);		/* Recursively insert on right subtree */
	} else {
		if (bucket->left == NULL) {
			_ds_smap_bucket *lbucket = _ds_get_smap_bucket(key);

			if (lbucket == NULL)
				return NULL;

			lbucket->parent = bucket;
			bucket->left = lbucket;
			_ds_smap_fixup(map, lbucket);
		}

		return _ds_smap_insert_handler(map, bucket->left, key, node);	/* Recursively insert on left subtree */
	}

	return NULL;
}

static AErr _ds_smap_insert(_ds_smap* smap, const AString key, void* data) {	/* Funcion to insert into smap  */
	if (smap == NULL) 
		return ERR_DS_INVALID_STRUCT;

	AAddr hash = djb2(key);
	
	_ds_smap_node *node = _ds_get_smap_node(key, data);
	if (node == NULL)
		return ERR_DS_STRUCT_GEN_FAIL;

	if (smap->size == 0) {
	/* if hashmap is empty */
		_ds_smap_bucket *bucket = _ds_get_smap_bucket(hash);
		if (bucket == NULL)
			return ERR_DS_STRUCT_GEN_FAIL;
		
		bucket->color = BLACK;
		smap->root = bucket;
		_ds_smap_bucket_insert(smap->root, node);
		smap->size += 1;
		
		return SUCCESS;
	}

	_ds_smap_bucket* bucket = _ds_smap_insert_handler(smap, smap->root, hash, node);
	if (bucket == NULL)
		return ERR_DS_INSERT_FAIL;

	smap->size += 1;
	return SUCCESS;
}

static _ds_smap_node* _ds_smap_find_handler(_ds_smap_bucket* bucket, AString key) {
	AAddr hash = djb2(key);
	if (hash == bucket->key) {
	/* Correct bucket found */
		_ds_smap_node *cur = bucket->head;

		while (cur != NULL) {
			if (strcmp(key, cur->key) == 0)
				return cur;
			cur = cur->next;
		}
		return NULL;

	} else if (hash < bucket->key) {
	/* Value potentially lies in left subtree */
		if (bucket->left == NULL)
			return NULL;
		return _ds_smap_find_handler(bucket->left, key);

	} else {
	/* Value potentially lies in right subtree */
		if (bucket->right == NULL)
			return NULL;
		return _ds_smap_find_handler(bucket->right, key);
	}
}

static _ds_smap_node* _ds_smap_find(_ds_smap* smap, AString key) {	/* Function to find the location of value in SMap   */	
	if (smap == NULL)
		return NULL;
	if (smap->root == NULL)
		return NULL;

	return _ds_smap_find_handler(smap->root, key);
}
ABool _ds_smap_empty(_ds_smap* smap) {	/* Function to check if the SMap is empty   */
	if (smap == NULL)
		return TRUE;
	return (smap->size == 0)? TRUE: FALSE;
}

ASize _ds_smap_size(_ds_smap* smap) {
	if (smap == NULL)
		return 0;

	return smap->size;
}

static _ds_smap_bucket* _ds_smap_get_bucket_iterator(_ds_smap* smap) {
	if (smap == NULL)
		return NULL;

	if (smap->root == NULL)
		return NULL;

	_ds_smap_bucket* cur = smap->root;

	if (cur->size == 0)
		return NULL;

	while (cur->left != NULL)
		cur = cur->left;

	return cur;
}

static void _ds_smap_next_iterator(_ds_smap_bucket* bucket, _ds_queue* queue) {
	if ((bucket == NULL) || (queue == NULL))
		return;

	if (bucket->left)
		_ds_smap_next_iterator(bucket->left, queue);

	_ds_queue_insert(queue, (void*)bucket);

	if (bucket->right)
		_ds_smap_next_iterator(bucket->right, queue);
}

/**
 * The Functions for HashMap Data Structure
 * -----------------------------------------*/

_ds_map_node* _ds_get_map_node(AAddr key, void* data) {	/* Function to allocate the map node in memory   */
	_ds_map_node* node = (_ds_map_node*)malloc(sizeof(_ds_map_node));
	if (node == NULL)
		return NULL;

	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

_ds_map* _ds_get_map() { 	/* Function to allocate the map into memory  */
	_ds_map* map = (_ds_map*)malloc(sizeof(_ds_map));
	if (map == NULL)
		return NULL;

	map->root = NULL;
	map->size = 0;

	return map;
}

static void _ds_free_map_node(_ds_map_node* node) {	/* Function to deallocate the map node memory   */
	if (node == NULL)
		return;

	if (node->data != NULL)
		free(node->data);

	free(node);
	node = NULL;
}

static void _ds_free_map_handler(_ds_map_node* node) {	/* Function the deallocate the whole map recursively */
	if (node == NULL)
		return;

	_ds_free_map_handler(node->left);
	_ds_free_map_handler(node->right);
	_ds_free_map_node(node);
}

static void _ds_free_map(_ds_map* map) {	/* Function to deallocate the map memory */
	if (map == NULL)
		return;

	_ds_free_map_handler(map->root);
}

static void _ds_map_node_rot_right(_ds_map* map, _ds_map_node* y) {
	_ds_map_node* x = y->left;
	y->left = x->right;

	if (x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == NULL)
		map->root = x;
	else if (y == y->parent->right)
		y->parent->right = x;
	else
		y->parent->left = x;

	x->right = y;
	y->parent = x;

}

static void _ds_map_node_rot_left(_ds_map* map, _ds_map_node* x) {

	_ds_map_node* y = x->right;
	x->right = y->left;

	if (y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == NULL)
		map->root  = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;

}

static void _ds_map_fixup(_ds_map* map, _ds_map_node* node) {	/* Function to fix the map to adhere to Red-Black Tree rules  */
	if (node == NULL)
		return;

	_ds_map_node* z = node;

	while (z != map->root) {

		if (z->parent->color != RED)
			break;
		
		if (z->parent == z->parent->parent->left) {	/* Parent is left child   */
			_ds_map_node* y = z->parent->parent->right; 		/* The uncle   */

			if ((y != NULL) && (y->color == RED)) {	/* Case I: Uncle is RED */
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
						/* Case II: z is right child  */
						z = z->parent;
						_ds_map_node_rot_left(map, z);
				}
				/* Case III: z is left child  */
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_ds_map_node_rot_right(map, z->parent->parent);
			}
		} else {
		/* Mirror Case  */
			_ds_map_node* y = z->parent->parent->left;

			if ((y != NULL) && (y->color == RED)) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					_ds_map_node_rot_right(map, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_ds_map_node_rot_left(map, z->parent->parent);
			}
		}
	}
	map->root->color = BLACK;

}

static void _ds_map_print(_ds_map_node* node) {
	if (node == NULL)
		return;
	_ds_map_print(node->left);
	printf("DEBUG: address-> %d, data->%d\n", node->key, node->data);
	_ds_map_print(node->right);
}

AErr _ds_map_insert_handler(_ds_map_node* node, AAddr key, void* data) {
	if (node->key == key) {
		return ERR_MAP_DUP_KEY;
	}
	if (node->key > key) {
		if (node->left == NULL) {
			_ds_map_node* new_node = _ds_get_map_node(key, data);
			if (new_node == NULL)
				return ERR_DS_STRUCT_GEN_FAIL;
			node->left = new_node;
			return SUCCESS;
		}
		return _ds_map_insert_handler(node->left, key, data);
	} else {
		if (node->right == NULL) {
			_ds_map_node* new_node = _ds_get_map_node(key, data);
			if (new_node == NULL)
				return ERR_DS_STRUCT_GEN_FAIL;
			node->right = new_node;
			return SUCCESS;
		}
		return _ds_map_insert_handler(node->right, key, data);
	}
}

AErr _ds_map_insert(_ds_map* map, AAddr key, void* data) {
	if (map == NULL)
		return ERR_DS_INVALID_STRUCT;
	
	if (map->size == 0) {
		/* If size map is empty   */
		_ds_map_node* node = _ds_get_map_node(key, data);
		if (node == NULL)
			return ERR_DS_STRUCT_GEN_FAIL;
		map->root = node;
		map->size = 1;
		
		return SUCCESS;
	}

	AErr err = _ds_map_insert_handler(map->root, key, data);

	if (err == SUCCESS) {
		map->size += 1;
	}
	
	return err;
}

_ds_map_node* _ds_map_find_handler(_ds_map_node* node, AAddr key) {	/* The function to find key in HMap recursively */
	if (node == NULL)	/* Return NULL if the key is not found   */
		return NULL;

	if (node->key == key)		/* Return node is correct key is matched   */
		return node;

	else if (node->key > key)
		return _ds_map_find_handler(node->left, key);

	else
		return _ds_map_find_handler(node->right, key);
}

_ds_map_node* _ds_map_find(_ds_map* map, AAddr key) {	
	if (map == NULL)
		return NULL;

	return _ds_map_find_handler(map->root, key);
}

ABool _ds_map_empty(_ds_map* map) {
	if (map == NULL)
		return TRUE;

	return (map->size == 0)? TRUE: FALSE;
}

ASize _ds_map_size(_ds_map* map) {
	if (map == NULL)
		return NULL;

	return map->size;
}

static void _ds_map_next_iterator(_ds_map_node* node, _ds_queue* queue) {
	if (node == NULL)
		return;
	if (node->left != NULL)
		_ds_map_next_iterator(node->left, queue);		
	
	_ds_queue_insert(queue, node->data);

	if (node->right != NULL)
		_ds_map_next_iterator(node->right, queue);
}

/* --------------------------------------------------------
 * Function for Global Data Structures
 * --------------------------------------------------------*/


/**
 * The Functions for Instruction Item
 * -----------------------------------------*/
void ds_destroy_IItem(IItem* item) {	/* Free the Instruction structure from memory   */
	if (item == NULL)
		return;

	if (item->operand_1 != NULL)
		free(item->operand_1);
	if (item->operand_2 != NULL)
		free(item->operand_2);

	free(item);
	item = NULL;
}

IItem *ds_new_IItem(AAddr address) {	/* Allocate new Instruction structure in memory   */
	IItem* item = (IItem*)malloc(sizeof(IItem));
	if (item == NULL)
		return NULL;

	item->address = address;
	item->n_op = 0;	/* Set default num operators to 0  */
	item->opcode = NULL;
	item->operand_1 = NULL;
	item->operand_2 = NULL;
	item->comment = NULL;
	item->destroy = ds_destroy_IItem;
	return item;
}

/**
 * The Functions for Instruction List
 * -----------------------------------------*/

AErr ds_IList_insert(IList* ilist, IItem* item) {	/* Function to insert item to Instruction List   */
	if (ilist == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (ilist->queue == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (item == NULL)
		return ERR_DS_INVALID_STRUCT;
	
	_ds_queue* queue = (_ds_queue*)(ilist->queue);
	return _ds_queue_insert(queue, (void*)item);
}

IItem *ds_IList_find(IList* ilist, AAddr address) {
	return NULL;
}

ABool ds_IList_empty(IList* ilist) {
	if (ilist == NULL)
		return TRUE;

	if (ilist->queue == NULL)
		return TRUE;

	return _ds_queue_empty((_ds_queue*)(ilist->queue));
}

ASize ds_IList_size(IList* ilist) {
	if (ilist == NULL)
		return ilist;

	return ilist->size;
}

void ds_destroy_IList(IList* ilist) {
	if (ilist == NULL)
		return;

	if (ilist->queue != NULL) {
		_ds_queue* queue = (_ds_queue*)(ilist->queue);
		_ds_free_queue(queue, TRUE);
	}
	
	free(ilist);
	ilist = NULL;
}

IItem *ds_IList_get(IList* ilist) {
	static IList* iptr = NULL;
	static _ds_queue_node* node = NULL;

	if ((ilist == NULL) && (iptr == NULL)) {
		/* If Nothing is provided and there is no previous memory   */
		iptr = NULL;
		node = NULL;
		return _END_ILIST;
	} else if (ilist == NULL) {
		/* If nothing is provided and there is previous memory   */
		if (node == NULL)
			return _END_ILIST;
		node = node->next;
		if (node == NULL)
			return _END_ILIST;
		IItem *item = (IItem*)(node->data);
		if (item == NULL)
			return _END_ILIST;

		return item;
	} else {
		/* Start with refreshed memory   */
		iptr = ilist;
		node = NULL;
		_ds_queue* queue = ilist->queue;
		if (queue == NULL)
			return _END_ILIST;

		node = queue->front;
		if (node == NULL)
			return _END_ILIST;

		IItem *item = (IItem*)(node->data);
		if (item == NULL)
			return _END_ILIST;
		
		return item;		
	}
}

IItem *ds_IList_end() {
	return _END_ILIST;
}

IList *ds_new_IList() {
	IList *ilist = (IList*)malloc(sizeof(IList));
	if (ilist == NULL)
		return NULL;

	_ds_queue* queue = _ds_get_queue();
	if (queue == NULL) {
		ds_destroy_IList(ilist);
		return NULL;
	}

	ilist->queue = queue;
	ilist->insert = ds_IList_insert;
	ilist->find = ds_IList_find;
	ilist->empty = ds_IList_empty;
	ilist->size = ds_IList_size;
	ilist->get = ds_IList_get;
	ilist->end = ds_IList_end;
	ilist->destroy = ds_destroy_IList;

	return ilist;
}

/**
 * The Functions for Symbol Item
 * -----------------------------------------*/

void ds_destroy_SymItem(SymItem *sitem) {
	if (sitem == NULL)
		return;

	free(sitem);
	sitem = NULL;
}

SymItem* ds_new_SymItem(AString key, AAddr address) {
	SymItem* sitem = (SymItem*)malloc(sizeof(SymItem));
	if (sitem == NULL)
		return NULL;
	
	sitem->key = key;
	sitem->address = address;
	sitem->destroy = ds_destroy_SymItem;
	return sitem;
}

/**
 * The Functions for Symbol Table 
 * -----------------------------------------*/

AErr ds_SymTable_insert(SymTable* table, AString key, AAddr address) {
	if (table == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (table->hashmap == NULL)
		return ERR_DS_INVALID_STRUCT;

	_ds_smap* smap = (_ds_smap*)(table->hashmap);
	AAddr* data = (AAddr*)malloc(sizeof(AAddr));
	if (data == NULL)
		return ERR_DS_STRUCT_GEN_FAIL;
	
	*data = address;
	return _ds_smap_insert(smap, key, (void*)data);
}

AAddr ds_SymTable_find(SymTable* table, AString key) {
	if (table == NULL)
		return ERR_MAP_FIND_ADDRESS;

	if (table->hashmap == NULL)
		return ERR_MAP_FIND_ADDRESS;

	_ds_smap *smap = (_ds_smap*)(table->hashmap);
	_ds_smap_node* node = _ds_smap_find(smap, key);

	if (node == NULL)
		return ERR_MAP_FIND_ADDRESS;
	
	AAddr* data = (AAddr*)(node->data);
	return *data;
}

ABool ds_SymTable_empty(SymTable *table) {
	if (table == NULL)
		return TRUE;

	if (table->hashmap == NULL)
		return TRUE;

	return _ds_smap_empty((_ds_smap*)(table->hashmap));
}

ASize ds_SymTable_size(SymTable *table) {
	if (table == NULL)
		return 0;
	
	if (table->hashmap == NULL)
		return 0;

	return _ds_smap_size((_ds_smap*)(table->hashmap));
}

void ds_destroy_SymTable(SymTable* table) {
	if (table == NULL)
		return;

	if (table->hashmap != NULL) {
		_ds_smap* smap = (_ds_smap*)(table->hashmap);
		_ds_free_smap(smap);
	}
	
	free(table);
	table = NULL;
}

SymItem* ds_SymTable_get(SymTable* table) {
	static SymTable* tptr = NULL;
	static _ds_queue* queue = NULL;
	static _ds_queue_node* bktnode = NULL;
	static _ds_smap_node* node = NULL;
	static SymItem* sitem = NULL;
	
	ds_destroy_SymItem(sitem);

	if ((table == NULL) && (tptr == NULL)) {
	/* If the Symbol Table is not provided and there is no back record   */
		_ds_free_queue(queue, FALSE);
		bktnode = NULL;
		node = NULL;
		return _END_SYMTB;
	}
	else if (table == NULL) {
	/* If the Symbol Table is not provided and there is back record  */
		if ((node != NULL) && (bktnode != NULL) && (queue != NULL)) {
			if (node == ((_ds_smap_bucket*)(bktnode->data))->head) {
					/* Bucket is finished */
				bktnode = bktnode->next;
				if (bktnode == NULL)
					return _END_SYMTB;

				_ds_smap_bucket* bucket = bktnode->data;
				if (bucket == NULL)
					return _END_SYMTB;

				if (bucket->size == 0) {
					bucket = NULL;
					return _END_SYMTB;
				}

				node = bucket->head;
				int i;
				for (i = 0; i<bucket->size-1; i++) {
					node = node->next;
				}
				
			} else {
				node = node->prev;
				if (node == NULL)
					return _END_SYMTB;
			} 
			sitem = ds_new_SymItem(node->key, *(AAddr*)node->data);
			if (sitem == NULL) {
				_ds_free_queue(queue, FALSE);
				queue = NULL;
				return _END_SYMTB;
			}
			return sitem;
		} else {
			/*  Clean up all the mess  */
			_ds_free_queue(queue, FALSE);
			queue = NULL;
			tptr = NULL;
			bktnode = NULL;
			node = NULL;
			return _END_SYMTB;
		} 
	}
	else if ((tptr == NULL) || (tptr != table)) {
	/* If the Symbol Table is first introduced   */
		tptr = table;
		queue = _ds_get_queue();
		if (queue == NULL)
			return _END_SYMTB;
	
		if (table->hashmap == NULL)
			return _END_SYMTB;

		_ds_smap* smap = table->hashmap;
		_ds_smap_bucket* root = smap->root;

		if (root == NULL)
			return _END_SYMTB;

		_ds_smap_next_iterator(root, queue);
		if (_ds_queue_size(queue) == 0) {
			return _END_SYMTB;
		}

		bktnode = queue->front;

		if (bktnode == NULL) 
			return _END_SYMTB;
	
		_ds_smap_bucket* bucket = bktnode->data;
		if (bucket == NULL) 
			return _END_SYMTB;

		
		if (bucket->size == 0) 
			return _END_SYMTB;
		
		/* If valid case reached finally filterer from above present error handlers  */
		node = bucket->head;
		int i;
		for (i = 0; i<bucket->size-1; i++) {
			node = node->next;
		}

		sitem = ds_new_SymItem(node->key, *(AAddr*)node->data);
		if (sitem == NULL)
			return _END_SYMTB;

		return sitem;
	}
}

SymItem*  ds_SymTable_end() {
	return _END_SYMTB;
}

SymTable *ds_new_SymTable() {
	SymTable *table = (SymTable*)malloc(sizeof(SymTable));

	if (table == NULL)
		return NULL;

	_ds_smap* smap = _ds_get_smap();
	if (smap == NULL) {
		ds_destroy_SymTable(table);
		return NULL;
	}

	table->hashmap = (void*)smap;
	table->insert = ds_SymTable_insert;
	table->find = ds_SymTable_find;
	table->empty = ds_SymTable_empty;
	table->size = ds_SymTable_size;
	table->get = ds_SymTable_get;
	table->end = ds_SymTable_end;
	table->destroy = ds_destroy_SymTable;

	return table;
}

/**
 * The Functions for Data Item
 * -----------------------------------------*/

DItem *ds_new_DItem(AAddr address, AInt32 data) {
	DItem* ditem = (DItem*)malloc(sizeof(DItem));
	if (ditem == NULL)
		return NULL;

	ditem->address = address;
	ditem->data = data;

	return ditem;
}

void ds_destroy_DItem(DItem *ditem) {
	if (ditem == NULL)
		return;

	free(ditem);
	ditem = NULL;
}

/**
 * The Functions for Data List
 * -----------------------------------------*/

AErr ds_DList_insert(DList* dlist, AInt32 data, AAddr *return_addr) {
	if (dlist == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (dlist->map == NULL)
		return ERR_MAP_INVALID_STRUCT;

	AInt address = dlist->offset + dlist->base;
	/* Size of incoming all data will be 4 byte for now.   */

	DItem* ditem = ds_new_DItem(address, data);
	if (ditem == NULL)
		return ERR_DS_STRUCT_GEN_FAIL;

	_ds_map* map = (_ds_map*)(dlist->map);

	dlist->offset+=1;
	*return_addr = address;

	return _ds_map_insert(map, address, (void*)ditem);
}

DItem *ds_DList_find(DList *dlist, AAddr address) {
	if (dlist == NULL)
		return _END_DLIST;

	if (dlist->map == NULL)
		return _END_DLIST;

	_ds_map* map = dlist->map;
	_ds_map_node* node = _ds_map_find(map, address);
	if (node == NULL)
		return _END_DLIST;
	
	return (DItem*)(node->data);
}

ABool ds_DList_empty(DList *dlist) {
	if (dlist == NULL)
		return TRUE;

	if (dlist->map == NULL)
		return TRUE;

	_ds_map* map = (_ds_map*)(dlist->map);

	return _ds_map_empty(map);
}

ASize ds_DList_size(DList *dlist) {
	if (dlist == NULL)
		return 0;

	if (dlist->map == NULL)
		return 0;

	_ds_map* map = (_ds_map*)(dlist->map);

	return _ds_map_size(map);
}

void ds_destroy_DList(DList* dlist) {
	if (dlist == NULL) 
		return;
	
	if (dlist->map != NULL) {
		_ds_map* map = dlist->map;
		_ds_free_map(map);
	}

	free(dlist);
	dlist = NULL;
}

DItem *ds_DList_get(DList* dlist) {
	static DList* dptr = NULL;
	static _ds_queue_node* node = NULL;
	static _ds_queue* queue = NULL;

	if ((dlist == NULL) && (dptr == NULL)) {
	/* If the data list is not provided and there is no back record */
		_ds_free_queue(queue, FALSE);
		node = NULL;
		return _END_DLIST;
	}
	else if (dlist == NULL) {
	/* If the data list is not provided and there is back record   */
		if (node == NULL) {
	/* Wipe memory on one additional call */
			_ds_free_queue(queue, FALSE);
			return _END_DLIST;
		}

		node = node->next;
		if (node == NULL)
			return _END_DLIST;
	
		return (DItem*)(node->data);
	} else {
	/* The previous context to be lost and new instance is started  */
		_ds_free_queue(node, FALSE);
		dptr = dlist;
		queue = _ds_get_queue();
		if (queue == NULL)
			return _END_DLIST;
		
		if (dlist->map == NULL)
			return _END_DLIST;

		_ds_map* map = dlist->map;

		_ds_map_next_iterator(map->root, queue);

		if (_ds_queue_size(queue) == 0) {
			_ds_free_queue(queue, FALSE);
			return _END_DLIST;
		}

		/* If first iterator is found   */
		node = queue->front;
		return (DItem*)(node->data);
	}
}

DList* ds_DList_end() {
	return _END_DLIST;
}

DList *ds_new_DList() {
	DList* dlist = (DList*)malloc(sizeof(DList));
	if (dlist == NULL)
		return NULL;

	_ds_map* map = _ds_get_map();
	if (map == NULL) {
		ds_destroy_DList(dlist);
		return NULL;
	}

	dlist->map = (void*)map;
	dlist->base = 0;
	dlist->offset = 0;
	dlist->insert = ds_DList_insert;
	dlist->find = ds_DList_find;
	dlist->empty = ds_DList_empty;
	dlist->size = ds_DList_size;
	dlist->get = ds_DList_get;
	dlist->end = ds_DList_end;
	dlist->destroy = ds_destroy_DList;

	return dlist;
}

/**
 * The Functions for Error/Warning Item
 * -----------------------------------------*/

EWItem *ds_new_EWItem(ASize line, ASize col, AErr code) {
	EWItem* eitem = (EWItem*)malloc(sizeof(EWItem));
	if (eitem == NULL)
		return NULL;

	eitem->line = line;
	eitem->col = col;
	eitem->code = code;

	return eitem;
}

void ds_destroy_EWItem(EWItem* item) {
	if (item == NULL)
		return;

	free(item);
	item = NULL;
}

/**
 * The Functions for Error/Warning Item
 * -----------------------------------------*/

AErr ds_EWList_insert(EWList* elist, EWItem* eitem) {
	if (elist == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (elist->queue == NULL)
		return NULL;

	_ds_queue* queue = (_ds_queue*)(elist->queue);
	return _ds_queue_insert(queue, (void*)eitem);
}

EWItem *ds_EWList_find(EWList* elist, ASize line) {
	/* Not implemented for now: not needed   */
	return NULL;
}

ABool ds_EWList_empty(EWList* elist) {
	if (elist == NULL)
		return TRUE;
	
	if (elist->queue == NULL)
		return TRUE;

	_ds_queue* queue = (_ds_queue*)(elist->queue);
	return _ds_queue_empty(queue);
}

ASize ds_EWList_size(EWList *elist) {
	if (elist == NULL)
		return 0;

	if (elist->queue == NULL)
		return 0;

	_ds_queue* queue = (_ds_queue*)(elist->queue);
	return _ds_queue_size(queue);
}

void ds_destroy_EWList(EWList *elist) {
	if (elist == NULL)
		return;

	if (elist->queue != NULL) {
		_ds_queue* queue = (_ds_queue*)(elist->queue);
		_ds_free_queue(queue, TRUE);
	}

	free(elist);
	elist = NULL;
}

EWItem *ds_EWList_get(EWList* elist) {
	static EWList* eptr = NULL;
	static _ds_queue_node* node = NULL;

	if ((elist == NULL) && (eptr == NULL)) {
		/* If Nothing is provided and there is no previous memory   */
		eptr = NULL;
		node = NULL;
		return _END_EWLST;
	} else if (elist == NULL) {
		/* If nothing is provided and there is previous memory   */
		if (node == NULL)
			return _END_EWLST;
		node = node->next;
		if (node == NULL)
			return _END_EWLST;
		EWItem *eitem = (EWItem*)(node->data);
		if (eitem == NULL)
			return _END_EWLST;

		return eitem;
	} else {
		/* Start with refreshed memory   */
		eptr = elist;
		node = NULL;
		_ds_queue* queue = elist->queue;
		if (queue == NULL)
			return _END_EWLST;

		node = queue->front;
		if (node == NULL)
			return _END_EWLST;

		EWItem *eitem = (EWItem*)(node->data);
		if (eitem == NULL)
			return _END_EWLST;
		
		return eitem;		
	}
}

IItem *ds_EWList_end() {
	return _END_EWLST;
}
EWList *ds_new_EWList() {
	EWList* elist = (EWList*)malloc(sizeof(EWList));
	if (elist == NULL)
		return NULL;

	_ds_queue* queue = _ds_get_queue();
	if (queue == NULL) {
		ds_destroy_EWList(elist);
		return NULL;
	}

	elist->queue = (void*)queue;
	elist->insert = ds_EWList_insert;
	elist->find = ds_EWList_find;
	elist->empty = ds_EWList_empty;
	elist->size = ds_EWList_size;
	elist->get = ds_EWList_get;
	elist->end = ds_EWList_end;
	elist->destroy = ds_destroy_EWList;

	return elist;
}

/**
 * The Functions for Mnemonic Item
 * -----------------------------------------*/

void ds_destroy_MnItem(MnItem *mitem) {
	if (mitem == NULL)
		return;

	free(mitem);
	mitem = NULL;
}

MnItem* ds_new_MnItem(AString key, AAddr encoding, ASize n_operand, AType operand_type) {
	MnItem* mitem = (MnItem*)malloc(sizeof(MnItem));
	if (mitem == NULL)
		return NULL;
	
	mitem->key = key;
	mitem->encoding = encoding;
	mitem->n_operand = n_operand;
	mitem->operand_type = operand_type;
	mitem->destroy = ds_destroy_MnItem;
	return mitem;
}

/**
 * The Functions for Mnemonic Map
 * -----------------------------------------*/

AErr ds_MnMap_insert(MnMap* map, AString key, AAddr encoding, ASize n_operand, AType operand_type) {
	if (map == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (map->hashmap == NULL)
		return ERR_MAP_INVALID_STRUCT;

	_ds_smap* smap = (_ds_smap*)(map->hashmap);
	MnItem* mitem = ds_new_MnItem(key, encoding, n_operand, operand_type);
	if (mitem == NULL)
		return ERR_DS_STRUCT_GEN_FAIL;
	
	return _ds_smap_insert(smap, key, (void*)mitem);
}

MnItem* ds_MnMap_find(MnMap* map, AString key) {
	if (map == NULL)
		return NULL;

	if (map->hashmap == NULL)
		return NULL;

	_ds_smap *smap = (_ds_smap*)(map->hashmap);
	_ds_smap_node* node = _ds_smap_find(smap, key);

	if (node == NULL)
		return NULL;
	
	MnItem* mitem = (MnItem*)(node->data);
	return mitem;
}

ABool ds_MnMap_empty(MnMap *map) {
	if (map == NULL)
		return TRUE;

	if (map->hashmap == NULL)
		return TRUE;

	return _ds_smap_empty((_ds_smap*)(map->hashmap));
}

ASize ds_MnMap_size(MnMap *map) {
	if (map == NULL)
		return 0;
	
	if (map->hashmap == NULL)
		return 0;

	return _ds_smap_size((_ds_smap*)(map->hashmap));
}

void ds_destroy_MnMap(MnMap* map) {
	if (map == NULL)
		return;

	if (map->hashmap != NULL) {
		_ds_smap* smap = (_ds_smap*)(map->hashmap);
		_ds_free_smap(smap);
	}
	
	free(map);
	map = NULL;
}

MnItem* ds_MnMap_get(MnMap* map) {
	static MnMap* tptr = NULL;
	static _ds_queue* queue = NULL;
	static _ds_queue_node* bktnode = NULL;
	static _ds_smap_node* node = NULL;
	

	if ((map == NULL) && (tptr == NULL)) {
	/* If the Mnemonic Map is not provided and there is no back record   */
		_ds_free_queue(queue, FALSE);
		bktnode = NULL;
		node = NULL;
		return _END_MNMAP;
	}
	else if (map == NULL) {
	/* If the Mnemonic Map is not provided and there is back record  */
		if ((node != NULL) && (bktnode != NULL) && (queue != NULL)) {
			if (node == ((_ds_smap_bucket*)(bktnode->data))->head) {
					/* Bucket is finished */
				bktnode = bktnode->next;
				if (bktnode == NULL)
					return _END_MNMAP;

				_ds_smap_bucket* bucket = bktnode->data;
				if (bucket == NULL)
					return _END_MNMAP;

				if (bucket->size == 0) {
					bucket = NULL;
					return _END_MNMAP;
				}

				node = bucket->head;
				int i;
				for (i = 0; i<bucket->size-1; i++) {
					node = node->next;
				}
				
			} else {
				node = node->prev;
				if (node == NULL)
					return _END_MNMAP;
			} 
			
			return (MnItem*)(node->data);
		} else {
			/*  Clean up all the mess  */
			_ds_free_queue(queue, FALSE);
			queue = NULL;
			tptr = NULL;
			bktnode = NULL;
			node = NULL;
			return _END_MNMAP;
		} 
	}
	else if ((tptr == NULL) || (tptr != map)) {
	/* If the Mnemonic Map is first introduced   */
		tptr = map;
		queue = _ds_get_queue();
		if (queue == NULL)
			return _END_MNMAP;
	
		if (map->hashmap == NULL)
			return _END_MNMAP;

		_ds_smap* smap = map->hashmap;
		_ds_smap_bucket* root = smap->root;

		if (root == NULL)
			return _END_MNMAP;

		_ds_smap_next_iterator(root, queue);
		if (_ds_queue_size(queue) == 0) {
			return _END_MNMAP;
		}

		bktnode = queue->front;

		if (bktnode == NULL) 
			return _END_MNMAP;
	
		_ds_smap_bucket* bucket = bktnode->data;
		if (bucket == NULL) 
			return _END_MNMAP;

		
		if (bucket->size == 0) 
			return _END_MNMAP;
		
		/* If valid case reached finally filterer from above present error handlers  */
		node = bucket->head;
		int i;
		for (i = 0; i<bucket->size-1; i++) {
			node = node->next;
		}

		return (MnItem*)(node->data);
	}
}

MnItem*  ds_MnMap_end() {
	return _END_MNMAP;
}

MnMap *ds_new_MnMap() {
	MnMap *map = (MnMap*)malloc(sizeof(MnMap));

	if (map == NULL)
		return NULL;

	_ds_smap* smap = _ds_get_smap();
	if (smap == NULL) {
		ds_destroy_MnMap(map);
		return NULL;
	}

	map->hashmap = (void*)smap;
	map->insert = ds_MnMap_insert;
	map->find = ds_MnMap_find;
	map->empty = ds_MnMap_empty;
	map->size = ds_MnMap_size;
	map->get = ds_MnMap_get;
	map->end = ds_MnMap_end;
	map->destroy = ds_destroy_MnMap;

	return map;
}


/**
 * Functions for Register Item Data Structure  
 * ----------------------------------------------*/
void ds_destroy_RegItem(RegItem *ritem) {
	if (ritem == NULL)
		return;

	free(ritem);
	ritem = NULL;
}

RegItem* ds_new_RegItem(AString key, AAddr encoding) {
	RegItem* ritem = (RegItem*)malloc(sizeof(RegItem));
	if (ritem == NULL)
		return NULL;
	
	ritem->key = key;
	ritem->encoding = encoding;
	ritem->destroy = ds_destroy_RegItem;
	return ritem;
}

/**
 * The Functions for Sym 
 * -----------------------------------------*/

AErr ds_RegMap_insert(RegMap* map, AString key, AAddr encoding) {
	if (map == NULL)
		return ERR_DS_INVALID_STRUCT;

	if (map->hashmap == NULL)
		return ERR_MAP_INVALID_STRUCT;

	_ds_smap* smap = (_ds_smap*)(map->hashmap);
	AAddr* data = (AAddr*)malloc(sizeof(AAddr));
	if (data == NULL)
		return ERR_MEM_ALLOC_FAIL;
	
	*data = encoding;
	return _ds_smap_insert(smap, key, (void*)data);
}

AAddr ds_RegMap_find(RegMap* map, AString key) {
	if (map == NULL)
		return _END_RGMAP;

	if (map->hashmap == NULL)
		return _END_RGMAP;

	_ds_smap *smap = (_ds_smap*)(map->hashmap);
	_ds_smap_node* node = _ds_smap_find(smap, key);

	if (node == NULL)
		return _END_RGMAP;
	
	AAddr* data = (AAddr*)(node->data);
	return *data;
}


ABool ds_RegMap_empty(RegMap *map) {
	if (map == NULL)
		return TRUE;

	if (map->hashmap == NULL)
		return TRUE;

	return _ds_smap_empty((_ds_smap*)(map->hashmap));
}

ASize ds_RegMap_size(RegMap *map) {
	if (map == NULL)
		return 0;
	
	if (map->hashmap == NULL)
		return 0;

	return _ds_smap_size((_ds_smap*)(map->hashmap));
}

void ds_destroy_RegMap(RegMap* map) {
	if (map == NULL)
		return;

	if (map->hashmap != NULL) {
		_ds_smap* smap = (_ds_smap*)(map->hashmap);
		_ds_free_smap(smap);
	}
	
	free(map);
	map = NULL;
}

RegItem* ds_RegMap_get(RegMap* map) {
	static RegMap* tptr = NULL;
	static _ds_queue* queue = NULL;
	static _ds_queue_node* bktnode = NULL;
	static _ds_smap_node* node = NULL;
	static RegItem* ritem = NULL;
	
	ds_destroy_RegItem(ritem);

	if ((map == NULL) && (tptr == NULL)) {
	/* If the Symbol Table is not provided and there is no back record   */
		_ds_free_queue(queue, FALSE);
		bktnode = NULL;
		node = NULL;
		return _END_SYMTB;
	}
	else if (map == NULL) {
	/* If the Symbol Table is not provided and there is back record  */
		if ((node != NULL) && (bktnode != NULL) && (queue != NULL)) {
			if (node == ((_ds_smap_bucket*)(bktnode->data))->head) {
					/* Bucket is finished */
				bktnode = bktnode->next;
				if (bktnode == NULL)
					return _END_SYMTB;

				_ds_smap_bucket* bucket = bktnode->data;
				if (bucket == NULL)
					return _END_SYMTB;

				if (bucket->size == 0) {
					bucket = NULL;
					return _END_SYMTB;
				}

				node = bucket->head;
				int i;
				for (i = 0; i<bucket->size-1; i++) {
					node = node->next;
				}
				
			} else {
				node = node->prev;
				if (node == NULL)
					return _END_SYMTB;
			} 
			ritem = ds_new_RegItem(node->key, *(AAddr*)node->data);
			if (ritem == NULL) {
				_ds_free_queue(queue, FALSE);
				queue = NULL;
				return _END_SYMTB;
			}
			return ritem;
		} else {
			/*  Clean up all the mess  */
			_ds_free_queue(queue, FALSE);
			queue = NULL;
			tptr = NULL;
			bktnode = NULL;
			node = NULL;
			return _END_SYMTB;
		} 
	}
	else if ((tptr == NULL) || (tptr != map)) {
	/* If the Symbol Table is first introduced   */
		tptr = map;
		queue = _ds_get_queue();
		if (queue == NULL)
			return _END_SYMTB;
	
		if (map->hashmap == NULL)
			return _END_SYMTB;

		_ds_smap* smap = map->hashmap;
		_ds_smap_bucket* root = smap->root;

		if (root == NULL)
			return _END_SYMTB;

		_ds_smap_next_iterator(root, queue);
		if (_ds_queue_size(queue) == 0) {
			return _END_SYMTB;
		}

		bktnode = queue->front;

		if (bktnode == NULL) 
			return _END_SYMTB;
	
		_ds_smap_bucket* bucket = bktnode->data;
		if (bucket == NULL) 
			return _END_SYMTB;

		
		if (bucket->size == 0) 
			return _END_SYMTB;
		
		/* If valid case reached finally filterer from above present error handlers  */
		node = bucket->head;
		int i;
		for (i = 0; i<bucket->size-1; i++) {
			node = node->next;
		}

		ritem = ds_new_RegItem(node->key, *(AAddr*)node->data);
		if (ritem == NULL)
			return _END_SYMTB;

		return ritem;
	}
}

RegItem*  ds_RegMap_end() {
	return _END_RGMAP;
}

RegMap *ds_new_RegMap() {
	RegMap *map = (RegMap*)malloc(sizeof(RegMap));

	if (map == NULL)
		return NULL;

	_ds_smap* smap = _ds_get_smap();
	if (smap == NULL) {
		ds_destroy_RegMap(map);
		return NULL;
	}

	map->hashmap = (void*)smap;
	map->insert = ds_RegMap_insert;
	map->find = ds_RegMap_find;
	map->empty = ds_RegMap_empty;
	map->size = ds_RegMap_size;
	map->get = ds_RegMap_get;
	map->end = ds_RegMap_end;
	map->destroy = ds_destroy_RegMap;

	return map;
}
