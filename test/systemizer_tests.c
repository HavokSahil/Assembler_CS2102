#include <stdio.h>
#include "utils.h"

#define TEST_PASSED 0
#define TEST_FAILED 1

/* Sub Test 1  */
int test_asm_systemizer() {
    char test_file[] = "test.asm";
    SynTree *tree = create_synTree(NULL);
    SymTable *table = create_symTable();
    DatMem *datmem = create_datMem();
    S2IHMap *mnemonic_map = get_mnemonic_map();

    int res = asm_systemizer(test_file, tree, table, datmem, mnemonic_map);
    if (res == TEST_PASSED) {
        judge_instructions(tree, table, datmem, mnemonic_map);
        dump_syntax_tree(tree);    
        dump_hash_table(table);
    }
    return res;
}

/* Sub Test 2  */
int test_asm_st_constructor() {
    char line[][128] = {
        "addi 90 ; Hello Friends this is comment",
        "label: data 0 ; this is label data",
        "ld ; this is comment",
        "main: ; this is label",
    };

    SynTree *tree = create_synTree(NULL);
    SymTable *table = create_symTable();
    DatMem *datmem = create_datMem();
    S2IHMap *mnemonic_map = get_mnemonic_map();
    
    unsigned long i = 0;
    for (i = 0; i<4; i++) {
        int code = asm_st_constructor(line[i], tree, table, datmem, mnemonic_map, (unsigned long)i);
        if (code == TEST_FAILED)
            return TEST_FAILED;
    }

    judge_instructions(tree, table, datmem, mnemonic_map);
    dump_syntax_tree(tree);
    dump_hash_table(table);
    return TEST_PASSED;
}

int main() {
    printf("Testing test_asm_systemizer()...");
    if (test_asm_systemizer() == TEST_FAILED) {
        printf("failed.\n");
        return TEST_FAILED;
    }
    printf("passed.\n");
    return TEST_PASSED;
    printf("Testing test_asm_st_constructor()...");
    if (test_asm_st_constructor() == TEST_FAILED) {
        printf("failed.\n");
        return TEST_FAILED;
    }
    printf("passed.\n");
    
    return TEST_PASSED;
}
