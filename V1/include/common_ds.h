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
#define _END_MNMAP 0xFFFFFFFF	/* The end pointer of Mnemonic Map   */
#define _END_RGMAP NULL	/* The end pointer of Register Map   */

/* --------------------------------------------------------
 * Structures for Global Data Structures
 * --------------------------------------------------------*/

/* The structure for instruction item  */
struct ds_instruction_struct {
	AAddr address;	/* The address of instruction in the memory space  */
	ASize lno;
	ASize n_op;	/* The numbers of operand */
	AString opcode;	/* The opcode in mnemonic form  */
	AString operand_1;	/* The first operand (NULL if not exist)  */
	AString operand_2;	/* The second operand (NULL if not exist)  */
	AString comment;	/* The comment in the instruction (NULL if not exist)  */

	void (*destroy)(struct ds_instruction_struct*);
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
	ASize base;
	ASize offset;
	AErr (*insert)(struct ds_dlist_struct*, AInt32, AAddr*);	/*   */
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

/* The Register Item Data Structure  */
struct ds_reg_item_struct {
	AString key;
	AAddr encoding;
	void (*destroy)(struct ds_reg_item_struct*);												/*   */
};

typedef struct ds_reg_item_struct RegItem;

/* The Register Map Data Structure  */
struct ds_reg_map_struct {
	void* hashmap;

	AErr (*insert)(struct ds_reg_map_struct*, AString, AAddr);					/*   */
	AAddr (*find)(struct ds_reg_map_struct*, AString);									/*   */
	ABool (*empty)(struct ds_reg_map_struct*);													/*   */
	ASize (*size)(struct ds_reg_map_struct*);														/*   */
	RegItem* (*get)(struct ds_reg_map_struct*);													/*   */
	RegItem* (*end)(void);																							/*   */
	void (*destroy)(struct ds_reg_map_struct*);													/*   */
};

typedef struct ds_reg_map_struct RegMap;

/* The Mnemonic Item Data Structure  */
struct ds_mnemo_item_struct {
	AString key;
	AAddr encoding;		/* The machine code value for the mnemonic   */
	ASize n_operand;		/* The number of operands that mnemonic expects */
	AType operand_type;	/* The type of operand that mnemonic expects offset or value */

	void (*destroy)(struct ds_mnemo_item_struct*);
};

typedef struct ds_mnemo_item_struct MnItem;

/* The Mnemonic Map Data Structure  */
struct ds_mnemo_map_struct {
	void* hashmap;	/*   */

	AErr (*insert)(struct ds_mnemo_map_struct*, AString, AAddr, ASize, AType);	/*   */
	AAddr (*find)(struct ds_mnemo_map_struct*, AString);								/*   */
	ABool (*empty)(struct ds_mnemo_map_struct*);												/*   */
	ASize (*size)(struct ds_mnemo_map_struct*);													/*   */
	MnItem* (*get)(struct ds_mnemo_map_struct*);												/*   */
	MnItem* (*end)(void);																								/*   */
	void (*destroy)(struct ds_mnemo_map_struct*);												/*   */
};

typedef struct ds_mnemo_map_struct MnMap;

/* -----------------------------------------------
 * The Functions for Global Data Structures
 * -----------------------------------------------*/

/**
 * the functions for instruction item
 * -----------------------------------------*/
IItem *ds_new_IItem(AAddr);	/* allocate new Instruction Item  */

/**
 * the functions for Data Item 
 * -----------------------------------------*/
DItem *ds_new_DItem(AAddr, AInt32);	/*   */

/**
 * the functions for Error/Warning Item 
 * -----------------------------------------*/
EWItem *ds_new_EWItem(ASize, ASize, AErr);	/*   */

/**
 * The Function for Mnemonic Item 
 * -----------------------------------------*/
MnItem *ds_new_MnItem(AString, AAddr, ASize, AType);

/**
 * The Function for Register Item */
RegItem* ds_new_RegItem(AString, AAddr);

/**
 * The Functions for Symbol Table
 * -----------------------------------------*/
SymTable *ds_new_SymTable();	/*   */

/**
 * The Functions for Instruction List
 * -----------------------------------------*/
IList *ds_new_IList();	/*   */

/**
 * The Functions for Data List
 * -----------------------------------------*/
DList *ds_new_DList();	/*   */

/**
 * The Functions for Error/Warning List
 * -----------------------------------------*/
EWList *ds_new_EWList();	/*   */

/**
 * The Function for MnemonicHashMap
 * ----------------------------------------*/
MnMap *ds_new_MnMap();	/*   */

/**
 * The Function for Register Map
 * ---------------------------------------*/
RegMap *ds_new_RegMap();	/*   */



#endif
