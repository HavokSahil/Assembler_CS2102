/**
 * Common Data Structures
 * --------------------------------------------------------
 *  This section contains data structures that are shared
 *  across various modules.
 *
 *
 *  Note:
 *  - Each Data Structure is modular and easily extendable.
 *  - Each Data Structure implements `insert`, `delete`,
 *  	`find`, `empty`, `size`.
 *  - `find` returns .end() if element is not found.
 *  - Each Data Structure stores `void *` as value for
 *  	usage across multiple context using typecast.
 *
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
 *********************************************************/

#ifndef _COMMON_DS_H
#define _COMMON_DS_H

#include <common_types.h>

/* Definations for End Iterators of each data structures  */
#define _END_ILIST NULL	/* The end pointer of Instruction list */
#define _END_DLIST NULL	/* The end pointer of Data List   */
#define _END_SYMTB NULL	/* The end pointer of Symbol Table   */
#define _END_EWLST NULL	/* The end pointer of Error/Warning List   */


/* --------------------------------------------------------
 * Structures for Global Data Structures
 * --------------------------------------------------------*/

/* The structure for instruction item  */
struct ds_instruction_struct {
	AAddr address;	/* The address of instruction in the memory space  */
	ASize n_op;	/* The numbers of operand */
	AString opcode;	/* The opcode in mnemonic form  */
	AString operand_1;	/* The first operand (NULL if not exist)  */
	AString operand_2;	/* The second operand (NULL if not exist)  */
	AString comment;	/* The comment in the instruction (NULL if not exist)  */
};

typedef struct ds_instruction_struct IItem;

/* The instruction for data item  */
struct ds_data_struct {
	AAddr address;	/*   */
	AInt32 data;	/*   */
};

typedef struct ds_data_struct DItem;

/* The instructions for Error/Warning Item  */
struct ds_ew_item {
	ASize line;	/* Line of the Error/Warning   */
	ASize col;	/* Column of the Error/Warning   */
	AErr code;	/* The Error/Warning Code  */
};

typedef struct ds_ew_item EWItem;

/* The Symbol Item Data Structure */
struct ds_sym_item {
	AString key;
	AAddr address;
	void (*destroy)(struct ds_sym_item*);
};

typedef struct ds_sym_item SymItem;

/* The Symbol Table Data Structure  */
struct ds_symtable_struct {
	void* hashmap;	/*   */

	AErr (*insert)(struct ds_symtable_struct*, AString, AAddr);	/*   */
	AAddr (*find)(struct ds_symtable_struct*, AString);	/*   */
	ABool (*empty)(struct ds_symtable_struct*);		/*   */
	ASize (*size)(struct ds_symtable_struct*);	/*   */
	SymItem* (*get)(struct ds_symtable_struct*);	/*   */
	SymItem* (*end)(void);	/*    */
	void (*destroy)(struct ds_symtable_struct*);	/*   */
};

typedef struct ds_symtable_struct SymTable;


/* The Instruction List Data Structure */
struct ds_ilist_struct {
	void* queue;	/*   */

	AErr (*insert)(struct ds_ilist_struct*, IItem*);	/*   */
	IItem* (*find)(struct ds_ilist_struct*, AAddr);	/*   */
	ABool (*empty)(struct ds_ilist_struct*);	/*   */
	ASize (*size)(struct ds_ilist_struct*);	/*   */
	IItem* (*get)(struct ds_ilist_struct*); 	/*   */
	IItem* (*end)(void); 	/*   */
	void (*destroy)(struct ds_ilist_struct*);	/*   */
};

typedef struct ds_ilist_struct IList;


/* The Data List Data Structure  */
struct ds_dlist_struct {
	void* map;	/*   */

	AErr (*insert)(struct ds_dlist_struct*, AAddr, AInt32);	/*   */
	DItem* (*find)(struct ds_dlist_struct*, AAddr);	/*   */
	ABool (*empty)(struct ds_dlist_struct*);	/*   */
	ASize (*size)(struct ds_dlist_struct*);	/*   */
	DItem* (*get)(struct ds_dlist_struct*); 	/*   */
	DItem* (*end)(void); 	/*   */
	void (*destroy)(struct ds_dlist_struct*);	/*   */
};

typedef struct ds_dlist_struct DList;


/* The Error/Warning List Data Structure  */
struct ds_ewlist_struct {
	void* queue;	/*   */

	AErr (*insert)(struct ds_ewlist_struct*, EWItem*);	/*   */
	EWItem* (*find)(struct ds_ewlist_struct*, AAddr);	/*   */
	ABool (*empty)(struct ds_ewlist_struct*);	/*   */
	ASize (*size)(struct ds_ewlist_struct*);	/*   */
	EWItem* (*get)(struct ds_ewlist_struct*); 	/*   */
	EWItem* (*end)(void); 	/*   */
	void (*destroy)(struct ds_ewlist_struct*);	/*   */
};

typedef struct ds_ewlist_struct EWList;


/* -----------------------------------------------
 * The Functions for Global Data Structures
 * -----------------------------------------------*/

/**
 * the functions for instruction item
 * -----------------------------------------*/
IItem *ds_new_IItem(AAddr);	/* allocate new Instruction Item  */
void ds_destroy_IItem(IItem*);	/* free the allocated Instruction Item  */

/**
 * the functions for Data Item 
 * -----------------------------------------*/
DItem *ds_new_DItem(AAddr, AInt32);	/*   */
void ds_destroy_DItem(DItem*);	/*   */

/**
 * the functions for Error/Warning Item 
 * -----------------------------------------*/
EWItem *ds_new_EWItem(ASize, ASize, AErr);	/*   */
void ds_destroy_EWItem(EWItem*);	/*   */


/**
 * The Functions for Symbol Table
 * -----------------------------------------*/
SymTable *ds_new_SymTable();	/*   */
void ds_destroy_SymTable(SymTable*);	/*   */
AErr ds_SymTable_insert(SymTable*, AString, AAddr);	/*   */
AAddr ds_SymTable_find(SymTable*, AString);	/*   */
ABool ds_SymTable_empty(SymTable*);	/*   */
ASize ds_SymTable_size(SymTable*);	/*   */

/**
 * The Functions for Instruction List
 * -----------------------------------------*/
IList *ds_new_IList();	/*   */
void ds_destroy_IList(IList*);	/*   */
AErr ds_IList_insert(IList*, IItem*);	/*   */
IItem *ds_IList_find(IList*, AAddr);	/*   */
ABool ds_IList_empty(IList*);	/*   */
ASize ds_IList_size(IList*);	/*   */


/**
 * The Functions for Data List
 * -----------------------------------------*/
DList *ds_new_DList();	/*   */
void ds_destroy_DList(DList*);	/*   */
AErr ds_DList_insert(DList*, AAddr, AInt32);	/*   */
DItem *ds_DList_find(DList*, AAddr);	/*   */
ABool ds_DList_empty(DList*);	/*   */
ASize ds_DList_size(DList*);	/*   */

/**
 * The Functions for Error/Warning List
 * -----------------------------------------*/
EWList *ds_new_EWList();	/*   */
void ds_destroy_EWList(EWList*);	/*   */
AErr ds_EWList_insert(EWList*, EWItem*);	/*   */
EWItem *ds_EWList_find(EWList*, ASize);	/*   */
ABool ds_EWList_empty(EWList*);	/*   */
ASize ds_EWList_size(EWList*);	/*   */

#endif
