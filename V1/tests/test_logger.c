#include <logger/logger.h>
#include <parser/parser.h>
#include <common_ds.h>

#define SUCCESS 0
#define FAILURE 1

typedef struct {
    AString mnemonic;
    AAddr encoding;
    ASize n_operand;
    AType operand_type;
} Rule;

typedef struct {
    AString key;
    AAddr encoding;
} Register;

int test_logger_interface() {
    IList* ilist = ds_new_IList();
    DList* dlist = ds_new_DList();
    SymTable* stable = ds_new_SymTable();
    MnMap* map = ds_new_MnMap();
    EWList* elist = ds_new_EWList();
    RegMap* regmap = ds_new_RegMap();
    
    /* Add Instuctions  */
    Rule rules[19] = {
        {"ldc", 0, 1, TYPE_MNE_OPERAND_VALUE},
        {"adc", 1, 1, TYPE_MNE_OPERAND_VALUE},
        {"ldl", 2, 1, TYPE_MNE_OPERAND_OFFSET},
        {"stl", 3, 1, TYPE_MNE_OPERAND_OFFSET},
        {"ldnl", 4, 1, TYPE_MNE_OPERAND_OFFSET},
        {"stnl", 5, 1, TYPE_MNE_OPERAND_OFFSET},
        {"add", 6, 0, TYPE_MNE_OPERAND_NONE},
        {"sub", 7, 0, TYPE_MNE_OPERAND_NONE},
        {"shl", 8, 0, TYPE_MNE_OPERAND_NONE},
        {"shr", 9, 0, TYPE_MNE_OPERAND_NONE},
        {"adj", 10, 1, TYPE_MNE_OPERAND_VALUE},
        {"a2sp", 11, 0, TYPE_MNE_OPERAND_NONE},
        {"sp2a", 12, 0, TYPE_MNE_OPERAND_NONE},
        {"call", 13, 1, TYPE_MNE_OPERAND_OFFSET},
        {"return", 14, 0, TYPE_MNE_OPERAND_NONE},
        {"brz", 15, 1, TYPE_MNE_OPERAND_OFFSET},
        {"brlz", 16, 1, TYPE_MNE_OPERAND_OFFSET},
        {"br", 17, 1, TYPE_MNE_OPERAND_OFFSET},
        {"HALT", 18, 0}
    };

    Register registers[10] = {
        {"$s0", 0},
        {"$s1", 1},
        {"$s2", 2},
        {"$s3", 3},
        {"$s4", 4},
        {"$s5", 5},
        {"$s6", 6},
        {"$s7", 7},
        {"$s8", 8},
        {"$s9", 9},
    };

    AInt j;
    for (j = 0; j<19; j++) {
        if (map->insert(map, rules[j].mnemonic, rules[j].encoding, rules[j].n_operand, rules[j].operand_type) != SUCCESS)
            return FAILURE;
    }

    for (j = 0; j<10; j++) {
        if (regmap->insert(regmap, registers[j].key, registers[j].encoding) != SUCCESS)
        return FAILURE;
    }

    FILE* file = fopen("hello.txt", "r");

    if ((file == NULL) || (ilist == NULL) || (dlist == NULL) || (stable == NULL) || (map == NULL) || (elist == NULL) || (regmap == NULL))
        return FAILURE;

    ParserInterface* pi = psr_new_ParserInterface(ilist, elist, stable, dlist, map, regmap, file);
    if (pi == NULL)
        return FAILURE;

    if (pi->parse(pi, file) != SUCCESS)
        return FAILURE;

    LoggerInterface* li = lg_new_LoggerInterface(stdout, stdout, 0, elist, ilist, stable, dlist, map, regmap);
    
    if (li == NULL)
        return FAILURE;

    li->logmn(li);
    li->logreg(li);
    li->log(li, 0);
    
    FILE* ffile = fopen("new.txt", "w");
    li->generate_alf(li, NULL, ffile);

    return SUCCESS;
}

int main() {
    if (test_logger_interface() == FAILURE)
        return FAILURE;
    return SUCCESS;
}
