#include "utils.h"
#include <stdio.h>

#define TEST_PASSED 0
#define TEST_FAILED 1

char tests[][128] = { "India\0", "Bhutan\0", "Nepal\0", "Shri Lanks\0" };
unsigned long results[] = { 1, 2, 3, 4 };

/* Sub Test 1 */
int test_insertion() {
    int i;
    S2IHMap *map = create_s2i_hmap();
    for (i = 0; i<4; i++) {
        if (hmap_insert(map, tests[i], results[i]) == 1) {
            printf("failed. [%d]\n", i);
            return TEST_FAILED;
        }
    }

    free_s2i_hashmap(map);
    return TEST_PASSED;
}

/* Sub Test 2  */
int test_accessing() {
    int i;
    S2IHMap *map = create_s2i_hmap();
    for (i = 0; i<4; i++) {
        if (hmap_insert(map, tests[i], results[i]) == 1) {
            printf("failed. [%d]\n", i);
            return TEST_FAILED;
        }
    }
    for (i = 0; i<4; i++) {
        BktNode *node = hmap_get_item_by_key(map, tests[i]);
        if (node == NULL)
            return TEST_FAILED;

        if (node->value != results[i])
            return TEST_FAILED;
    }

    free_s2i_hashmap(map);
    return TEST_PASSED;
}

int main() {
    printf("Testing for test_insertion()...");
    if (test_insertion() == TEST_FAILED)
        return TEST_FAILED;
    printf("passed\n");

    printf("Testing for test_accessing()...");
    if (test_accessing() == TEST_FAILED)
        return TEST_FAILED;
    printf("passed\n");
    
    return TEST_PASSED;
}
