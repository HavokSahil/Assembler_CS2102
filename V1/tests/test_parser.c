#include <parser/parser.h>
#include <common_ds.h>

#define SUCCESS 0
#define FAILURE 1

typedef struct {
    AString mnemonic;
    AAddr encoding;
    ASize n_operand;
} Rule;

int test_parser_interface() {
    IList* ilist = ds_new_IList();
    DList* dlist = ds_new_DList();
    SymTable* stable = ds_new_SymTable();
    MnMap* map = ds_new_MnMap();
    EWList* elist = ds_new_EWList();
    RegMap* regmap = ds_new_RegMap();
    

    /* Add Instuctions  */
    Rule rules[19] = {
        {"ldc", 0, 1},
        {"adc", 1, 1},
        {"ldl", 2, 1},
        {"stl", 3, 1},
        {"ldnl", 4, 1},
        {"stnl", 5, 1},
        {"add", 6, 0},
        {"sub", 7, 0},
        {"shl", 8, 0},
        {"shr", 9, 0},
        {"adj", 10, 1},
        {"a2sp", 11, 0},
        {"sp2a", 12, 0},
        {"call", 13, 1},
        {"return", 14, 0},
        {"brz", 15, 1},
        {"brlz", 16, 1},
        {"br", 17, 1},
        {"HALT", 18, 0}
    };

    AInt j;
    for (j = 0; j<19; j++) {
        if (map->insert(map, rules[j].mnemonic, rules[j].encoding, rules[j].n_operand))
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

    IItem* cur = ilist->get(ilist);
    
    while (cur != NULL) {
        printf("%d %d %d %s %s %s %s\n", cur->address, cur->lno, cur->n_op, cur->opcode, cur->operand_1, cur->operand_2, cur->comment);
        cur = ilist->get(NULL);
    }
    return SUCCESS;
}

int main() {
    if (test_parser_interface() == FAILURE)
        return FAILURE;
    return SUCCESS;
}
