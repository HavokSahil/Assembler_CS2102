#include <stdio.h>
#include "utils.h"

#define TEST_PASSED 0
#define TEST_FAILED 1

/* Sub Test 1 */
int test_get_line_attrib() {
    char lines[][128] = {";This is a comment\0", "ADD $0 ;Comment\0", "ADD ; Comment\0", "ADD\0", "main: ; hello world\0", "main: data 0\0"};
    unsigned char answers[] = {2, 4, 8, 9, 16, 49};

    int i = 0;
    for (i = 0; i<5; i++) {
        unsigned char res = get_line_attrib(lines[i]);
        if (res != answers[i]) {
            printf("failed. [1] -> %d for %s\n", res, lines[i]);
            return TEST_FAILED;
        }
    }
    return TEST_PASSED;
}

/* Sub Test 2 */
int test_get_line_context() {
    char lines[][128] = {" ; This is a comment\0", "ADD $0; This is instruction\0", "main: ; This is label\0" };
    unsigned char answers[] = {1, 2, 4};

    int i;
    for (i = 0; i<3; i++) {
        unsigned char res = get_line_context(lines[i]);
        if (res != answers[i]) {
            printf("failed. [2] -> %d for %s\n", res, lines[i]);
            return TEST_FAILED;
        }
    }

    return TEST_PASSED;
}

int main() {

    printf("Testing get_line_attrib()...");
    if (test_get_line_attrib() == TEST_FAILED)
        return TEST_FAILED;
    printf("passed\n");

    printf("Testing get_line_context()...");
    if (test_get_line_context() == TEST_FAILED)
        return TEST_FAILED;
    printf("passed\n");

    return TEST_PASSED;
}
