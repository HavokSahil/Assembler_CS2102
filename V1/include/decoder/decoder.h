#ifndef _DECODER_H
#define _DECODER_H

#include <common_types.h>
#include <common_ds.h>
#include <err_codes.h>
#include <stdio.h>


/**
 * Format Defination of Decoding
 * -------------------------------------------------------
 * Binary File will have two sections `data` and `code`.
 * 
 * The structure is:
 * <4 byte>:Header `DLCO`
 * <1 byte>: if `1` then next section is data section else
 *           there is no data section.
 * 
 * If there is a data section then.
 * <4 byte>: `section name` `DATA`
 * <4 byte>: Base address
 * <4 byte>: size of `data` section. Number of data entry.
 * 
 * If last <4 byte> was say n:
 * Next <4*n byte> will contain data. with each entry
 * <4 byte>
 * 
 * Code section
 * <4 byte>: `section name` `CODE`
 * <4 byte>: size of `code` section say n;
 * <4*n byte>: the instructions. with each <4 byte>
 * 
 * ------------------------------------------------------*/

/* The instruction is such that last 8 bit is for opcode and first 24 for value */


/* The Decoder Interface */
struct _dc_decoder_interface {
    IList* ilist;
    SymTable* stable;
    EWList* elist;
    DList* dlist;
    MnMap* mnmap;
    RegMap* rmap;   /* Unused for now, keeping for final extension of assembler */

    AErr (*decode_instruction)(struct _dc_decoder_interface*, IItem*, AAddr*, AType);    /* Address to dump the decoded instruction */
    AErr (*decode)(struct _dc_decoder_interface*, FILE*);
    void (*destroy)(struct _dc_decoder_interface*);
};

typedef struct _dc_decoder_interface DecoderInterface;


/* Functions for Decoder Interface */
DecoderInterface* dc_new_DecoderInterface(IList*, SymTable*, DList*, MnMap*, RegMap*, EWList*);

#endif