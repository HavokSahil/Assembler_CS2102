
#include <decoder/decoder.h>
#include <common_types.h>
#include <err_codes.h>
#include <common_ds.h>
#include <string.h>
#include <stdio.h>


static AErr _dc_insert_error(EWList* elist, ASize line, ASize col, AErr err) {
    if (elist == NULL) {
        return ERR_DS_INVALID_STRUCT;
    }

    EWItem* item = ds_new_EWItem(line, col, err);
    if (item == NULL) {
        return ERR_DS_INVALID_STRUCT;
    }

    return elist->insert(elist, item);
}

static AErr _dc_decode_instruction_handler(IItem* item, AAddr* addr, EWList* elist, MnMap* mnmap, SymTable* stable, AType mode) {
    if (item == NULL || addr == NULL) {
        return ERR_DS_INVALID_STRUCT;
    }

    AErr eno = SUCCESS;

    if (item->opcode == NULL) {
        if (mode == DECODER_MODE_BIN)
            return _dc_insert_error(elist, item->lno, 1, ERR_ASM_INVALID_MNEMONIC);  
        return ERR_STR_INVALID_STRING;
    }

    MnItem* mitem = mnmap->find(mnmap, item->opcode);
    if (mitem == _END_MNMAP) {
        if (mode == DECODER_MODE_BIN)
            return _dc_insert_error(elist, item->lno, 1, ERR_ASM_INVALID_MNEMONIC);
        return ERR_ASM_INVALID_MNEMONIC;
    }
    *addr = 0;    

    /* In the current format used there can be only 0 or 1 operand */
    if (mitem->n_operand == 1) {
        AString operand = item->operand_1;
        if (operand == NULL)
            return ERR_STR_INVALID_STRING;
        
        AInt32 value = 0;
        if (isalpha(*operand)) {
            /* Check if the operand is a label */
            AAddr address = stable->find(stable, operand);
            if (address == ERR_MAP_FIND_ADDRESS) {
                if (mode == DECODER_MODE_BIN)
                    _dc_insert_error(elist, item->lno, 1, DEC_ERR_LBL_UNDEF);
                return DEC_ERR_LBL_UNDEF;
            }

            value = address;
            if (mitem->operand_type == TYPE_MNE_OPERAND_OFFSET) {
                /* The operand is label to jump */
                value =  value - (AInt32)(1+item->address);
                if (value == -1) {
                    /* Case of Infinite Loop */
                    if (mode == DECODER_MODE_BIN) {
                        _dc_insert_error(elist, item->lno, 1, WARN_ASM_INFINITE_LOOP);
                    }
                    eno = WARN_ASM_INFINITE_LOOP;
                }
            }
        } else {
            /*Value is in number form*/
            value = strtol(operand, NULL, 0);
            if (value == 0 && (strcmp(operand, "0")!=0)) {
                if (mode == DECODER_MODE_BIN)
                    _dc_insert_error(elist, item->lno, 1, PSR_ERR_FMT_OPRND);
                return PSR_ERR_FMT_OPRND;
            }
        }

        *addr = value<<8;
    }

    *addr = (*addr) | mitem->encoding;   /* Push mnemonic opcode into the instruction */
    
    return eno;
}

static void word2bytes(AInt32 word, AByte byte_array[]) {
    int i;
    for (i = 0; i<4; i++) {
        AByte byte = word & 0xFF;
        byte_array[3-i] = byte;
        word >>= 8;
    }
}

static void dump_data_section(DList* dlist, FILE* stream) {
    AByte section_header[4] = "DATA";
    fwrite(section_header, sizeof(AByte), sizeof(section_header), stream);
    ASize size = 4 * dlist->size(dlist);    

    /* Write the size of the file in a word */
    AByte size_byte[4];
    word2bytes(size, size_byte);
    fwrite(size_byte, sizeof(AByte), sizeof(size_byte), stream);

    DItem* ditem = dlist->get(dlist);
    while (ditem != NULL) {
        AInt32 data = ditem->data;
        AByte word[4];
        word2bytes(data, word);
        fwrite(word, sizeof(AByte), sizeof(word), stream);
        ditem = dlist->get(NULL);
    }
}

static void dump_text_section(AByte *buffer, ASize nbytes, FILE* stream) {
    AByte section_header[4] = "TEXT";   
    fwrite(section_header, sizeof(AByte), sizeof(section_header), stream);

    AByte size_byte[4];
    word2bytes(nbytes, size_byte);
    fwrite(size_byte, sizeof(AByte), sizeof(size_byte), stream);
    fwrite(buffer, sizeof(AByte), nbytes, stream);
}

static void write_header(FILE* stream, ABool contains_data) {
    AByte bytes[3] = {'L', 'S', 'D'};
    AByte flags = 0;
    flags = (1<<7) | flags; 
    fwrite(bytes, sizeof(AByte), sizeof(bytes), stream);
    fwrite(&flags, sizeof(AByte), 1, stream);
}

static ABool is_error(AErr code) {
    return (code>=0xE0)? TRUE: FALSE;
}

AErr dc_decode(DecoderInterface* di, FILE* stream) {
    if (di == NULL)
        return ERR_DS_INVALID_STRUCT;
    
    if ((di->dlist == NULL) && (di->ilist == NULL) && (di->mnmap) && (di->stable))
        return ERR_DS_INVALID_STRUCT;

    /* Allocate buffer for instructions */    
    AByte* buffer = (AByte*)malloc(DECODER_IBUF_SIZ);
    if (buffer == NULL)
        return ERR_MEM_ALLOC_FAIL;

    ASize cursize = DECODER_IBUF_SIZ;
    ASize bytepos =  0;

    IList* ilist = di->ilist;
    IItem* item = ilist->get(ilist);
    AErr err = SUCCESS;
    while (item != _END_ILIST) {
        AAddr addr;
        err = _dc_decode_instruction_handler(item, &addr, di->elist, di->mnmap, di->stable, DECODER_MODE_BIN);
        
        if (is_error(err))
            break;

        word2bytes(addr, buffer+bytepos);
        bytepos+=4;
        if (bytepos == cursize) {
            /* Reallocate the memory */
            buffer = (AInt32*)realloc(buffer, cursize + DECODER_IBUF_SIZ);
            if (buffer == NULL) {
                free(buffer);
                return ERR_MEM_REALLOC_FAIL;
            }
            cursize+=DECODER_IBUF_SIZ;
        }

        item = ilist->get(NULL);
    }
    
    if (is_error(err)) {
        free(buffer);
        printf("ERROR captured\n");
        return DEC_ERR_ERR_CAPTD;
    } else {
        if (di->dlist->size(di->dlist) != 0) {
            write_header(stream, TRUE);
            dump_data_section(di->dlist, stream);
        } else
            write_header(stream, FALSE);
        dump_text_section(buffer, bytepos, stream);
    }

    return SUCCESS;
}


AErr dc_decode_instruction(DecoderInterface* di, IItem* item, AAddr* addr, AType mode) {
    if (item == NULL || addr == NULL || di == NULL)
        return ERR_DS_INVALID_STRUCT;
    
    return _dc_decode_instruction_handler(item, addr, di->elist, di->mnmap, di->stable, mode);
}

void dc_destroy(DecoderInterface* di) {
    if (di == NULL)
        return;
    
    free(di);
}


DecoderInterface* dc_new_DecoderInterface(IList* ilist, SymTable* stable, DList* dlist, MnMap* mnmap, RegMap* rmap, EWList* elist) {
    DecoderInterface* di = (DecoderInterface*)malloc(sizeof(DecoderInterface));
    if (di == NULL) {
        return NULL;
    }
    di->ilist = ilist;
    di->stable = stable;
    di->elist = elist;
    di->dlist = dlist;
    di->mnmap = mnmap;
    di->rmap = rmap;
    di->decode_instruction = dc_decode_instruction;
    di->decode = dc_decode;
    di->destroy = dc_destroy;
    return di;
}