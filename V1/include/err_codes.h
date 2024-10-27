#ifndef _ERR_CODES_H
#define _ERR_CODES_H


/* Usage of Code Ranges */
/* 0x00: Success Codes (1)*/
/* 0x01 - 0x3F: Error Codes (64)*/
/* 0x40 - 0x7F: Warning Codes (64)*/
/* 0x80 - 0xBF: Information Codes (64)*/
/* 0xC0 - 0xFF: Debug Codes (64)*/

/* Success Codes */
#define SUCCESS 0x00

/* Definations for Threshold */
#define THRESHOLD_ERR 0x01
#define THRESHOLD_WARN 0x40
#define THRESHOLD_INFO 0x80
#define THRESHOLD_DEBUG 0xC0

/* Error Codes for Memory Management */
#define ERR_MEM_ALLOC_FAIL 0x01
#define ERR_MEM_FREE_FAIL 0x02
#define ERR_MEM_REALLOC_FAIL 0x03
#define ERR_MEM_INVALID_ADDR 0x04
#define ERR_MEM_INVALID_SIZE 0x05
#define ERR_MEM_INVALID_BLOCK 0x06

/* Error Codes for File Management */
#define ERR_FILE_OPEN_FAIL 0x07
#define ERR_FILE_CLOSE_FAIL 0x08
#define ERR_FILE_READ_FAIL 0x09
#define ERR_FILE_WRITE_FAIL 0x0A
#define ERR_FILE_INVALID_PATH 0x0B
#define ERR_FILE_INVALID_MODE 0x0C
#define ERR_FILE_INVALID_FILE 0x0D
#define ERR_FILE_INVALID_DIR 0x0E

/* Error Codes for String Management */
#define ERR_STR_INVALID_STRING 0x0F
#define ERR_STR_INVALID_LENGTH 0x10
#define ERR_STR_INVALID_INDEX 0x11
#define ERR_STR_INVALID_CHAR 0x12
#define ERR_STR_INVALID_FORMAT 0x13


/* Error Codes for Tokenizer */
#define ERR_INVALID_INTERFACE 0x14
#define ERR_INTERFACE_GEN_FAIL 0x15

/* Error Codes for Data Structures */
#define ERR_DS_INVALID_STRUCT 0x16
#define ERR_DS_STRUCT_GEN_FAIL 0x17
#define ERR_DS_INSERT_FAIL 0x18

/* Error Codes for Tokenizer */
#define ERR_TOK_INVALID_JAR 0x19
#define ERR_TOK_INVALID_TOKEN 0x1A
#define ERR_TOK_INVALID_CARGO 0x1B
#define ERR_TOK_INVALID_PACKET 0x1C
#define ERR_TOK_CARGO_LOAD_FAIL 0x1D
#define ERR_TOK_JARIFICATION_FAIL 0x1E

/* Error Codes for Parser */
#define  ERR_PSR_NULL_ARG 0x1F  /* When one of the provided argument is null */
#define  ERR_PSR_INVALID_JAR 0x20  /* When the jar is invalid */
#define  ERR_PSR_INVALID_TOKEN 0x21  /* When the token is invalid */
#define  ERR_PSR_TOK_STRING_TEMPERED 0x22  /* When the token string is tempered */

/* Error Codes for Map Data Structure */
#define ERR_MAP_DUP_KEY 0x23
#define ERR_MAP_INVALID_STRUCT 0x24
#define ERR_MAP_FIND_ADDRESS 0xFFFFFFFF

/* Error Codes for Logger */
#define ERR_LOG_FAIL 0x26
#define ERR_LOG_INVALID_STREAM 0x27

/* Error Codes for Execution */
#define ERR_MAIN_EXECUTION 0x28


/* Warnings for Parser*/
#define WARN_PSR_INVALID_JAR_TYPE 0x40

/* Assembler Erros */
#define ERR_ASM_INVALID_MNEMONIC 0x41
#define ERR_ASM_INVALID_OPCODE 0x42
#define ERR_ASM_INVALID_OPERAND 0x43
#define ERR_ASM_INVALID_LABEL 0x44
#define ERR_ASM_INVALID_DIRECTIVE 0x45
#define ERR_ASM_INVALID_SET_DIRECTIVE 0x46
#define ERR_ASM_INVALID_DATA_DIRECTIVE 0x47
#define ERR_ASM_INVALID_DATA 0x48
#define ERR_ASM_INVALID_INSTRUCTION 0x49
#define ERR_ASM_INVALID_LABEL_INSTRUCTION 0x4A
#define ERR_ASM_INVALID_LABEL_DIRECTIVE 0x4B

/* Error Codes for Parser */
#define PSR_ERR_INV_MNEMO				0xE0
#define PSR_ERR_INV_OPRND				0xE1	
#define PSR_ERR_FMT_OPRND				0xE2
#define PSR_ERR_FMT_DDATA				0xE3
#define PSR_ERR_MIS_SETDA				0xE4
#define PSR_ERR_MIS_OPRND				0xE5
#define PSR_ERR_MIS_DDATA				0xE6
#define PSR_ERR_MMT_OPRND				0xE7
#define PSR_ERR_INV_LABEL				0xE8
#define PSR_ERR_DUP_LABEL				0xE9
#define PSR_ERR_INV_JRTYP 				0xEA

/* Error Codes for Decoder */
#define DEC_ERR_INV_OPRND				0xEB
#define DEC_ERR_LBL_UNDEF				0xEC
#define DEC_ERR_INV_OFFST				0xED
#define DEC_ERR_ERR_CAPTD               0xEE

/* Assembler Warnings */
#define WARN_ASM_DUPLICATE_LABEL 0x60
#define WARN_ASM_INFINITE_LOOP 0x61

#endif
