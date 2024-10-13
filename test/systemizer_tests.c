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
    AMsgList *msglist = create_new_amsg_list();
    InstrList *ilist = create_new_instruction_list();

    int res = asm_systemizer(test_file, tree, table, datmem, mnemonic_map, msglist);
    if (res == TEST_PASSED) {
        judge_instructions(tree, table, datmem, mnemonic_map);
        check_error(tree, msglist, ilist);
        FILE *output = fopen("output.bin", "wb");
        if (output != NULL) {
            print_machine_instruction(ilist, output);
            fclose(output);
        }
        return generate_advanced_listing_file("out.txt", tree, table, msglist);
    }
    return res;
}

int main() {
    printf("Testing test_asm_systemizer()...");
    if (test_asm_systemizer() == TEST_FAILED) {
        printf("failed.\n");
        return TEST_FAILED;
    }
    printf("passed.\n");
    return TEST_PASSED;
}
