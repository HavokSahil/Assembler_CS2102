#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <stdint.h>
#include <stddef.h>


/* Boolean Data Type */
#define TRUE 1
#define FALSE 0

/* Common Data Types */	
typedef unsigned long ASize;
typedef unsigned int AAddr;
typedef unsigned char ABool;
typedef unsigned char AByte;
typedef signed long AInt;
typedef signed long AErr;
typedef unsigned char AType;
typedef char* AString;
typedef uint32_t AInt32;

/* Color Data Type for Red-Black Tree Node*/
typedef enum {
	RED,
	BLACK
} Color;


#define SEPARATOR_COMMENT ';'

/* Types and Size Definations for Tokenizer */
#define  SZ_TOK_CARGO_PKT_WIN 	1000	/* Packet Window Size for Loading the File Content into Cargo  */	
#define  SZ_TOK_LINE_BUFF		1024	/* Buffer Size for Line Reading   */
#define  SZ_TOK_IF_MAX_THREAD	50		/* The maximum number of threads for Jarification  */
#define  SZ_TOK_TOKEN_COMMENT 	256 	/* Size of Comment Token  */
#define  SZ_TOK_TOKEN_LABEL 		32		/* Size of Label Token  */
#define  SZ_TOK_TOKEN_MNEMONIC 	32		/* Size of Mnemonic Token  */
#define  SZ_TOK_TOKEN_OPERAND 	32		/* Size of Operand Token  */

#define TYPE_TOK_COMMENT 	0xF1	/* Token type Comment  */
#define TYPE_TOK_LABEL 		0xF2	/* Token type Label  */
#define TYPE_TOK_WORD	 	0xF3	/* Token type Mnemonic  */
#define TYPE_TOK_OPERATOR	0xF4	/* Token type Operator  */
#define TYPE_TOK_DIRECTIVE	0xF5	/* Token type Directive  */
#define TYPE_TOK_BLANK		0xF6	/* Token type Black  */

#define UNSET_TOK_LINE		0		/* Unset value of line number of token or packet */
#define UNSET_TOK_COL		0		/* Unset value of column number of token  */


/* Types and Size Definations for Parser */
#define  SZ_PSR_BUFF_OPRND		64		/* Buffer Size for Operand  */	
#define  SZ_PSR_BUFF_LABEL		32		/* Buffer Size for Label  */
#define  SZ_PSR_BUFF_MNEMO		32		/* Buffer Size for Mnemonic  */
#define  SZ_PSR_BUFF_DIRECT		32		/* Buffer Size for Directive  */
#define  SZ_PSR_BUFF_COMMENT		256		/* Buffer Size for Comment  */

#define TYPE_PSR_JAR_LABEL			0xC0
#define TYPE_PSR_JAR_SET_DIRECT		0xC1
#define TYPE_PSR_JAR_SEC_DIRECT		0xC2
#define TYPE_PSR_JAR_DAT_DIRECT 	0xC3
#define TYPE_PSR_JAR_TEX_DIRECT 	0xC4
#define TYPE_PSR_JAR_INSTRUCTION	0xC5
#define TYPE_PSR_JAR_DATA_DECL		0xC6
#define TYPE_PSR_JAR_ERR			0xC7
#define TYPE_PSR_JAR_LABL_INSTR		0xC8

/*  Define operand types for Mnemonic Map */
#define TYPE_MNE_OPERAND_NONE		0x00
#define TYPE_MNE_OPERAND_OFFSET		0x01
#define TYPE_MNE_OPERAND_VALUE		0x02

/* Types and Size Definations for Decoder */
#define DECODER_MODE_ALF	0x01
#define DECODER_MODE_BIN	0x02
#define DECODER_IBUF_SIZ	64
#endif
