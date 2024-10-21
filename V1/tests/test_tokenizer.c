#include "tokenizer.h"

#define SUCCESS 0
#define FAILURE 1

int test_cargo() {
    Cargo* cargo = tk_new_Cargo();
    if (cargo == NULL)
        return FAILURE;

    AString strings[] = {"Hello", "World", "I", "am", "Assembler"};
    int i;
    for (i = 0; i<5; i++) {
        if (cargo->load(cargo, strings[i]) != SUCCESS) {
            printf("Loading Failed.\n");
            return FAILURE;
        }
    }

    for (i = 0; i<5; i++) {
        AString string = cargo->get(cargo, i);
        printf("The %dth element is %s\n", i, string);
    }
    printf("The size of cargo after loading is %d\n", cargo->size);
    cargo->destroy(cargo, FALSE);
    cargo = NULL;

    return SUCCESS;
}

int test_Jar() {
    Jar* jar = tk_new_Jar();
    if (jar == NULL)
        return FAILURE;
    
    AString strings[] = {"Hello", "World", "I", "am", "Assembler"};
    int i;
    for (i = 0; i<5; i++) {
        Token* token = tk_new_Token(_TOK_TYPE_COMMENT, strings[i]);
        if (token == NULL)
            return FAILURE;
        if (jar->put(jar, token) != SUCCESS)
            return FAILURE;
    }

    for (i = 0; i<5; i++) {
        Token* token = jar->get(jar, i);
        if (token == NULL)
            return FAILURE;
        printf("The token is %s with type: %d\n", token->token, token->type);
    }
    printf("The size of jar after insertion is %d\n", jar->size);
    return SUCCESS;
}

int main() {
    if (test_cargo() != SUCCESS)
        return FAILURE;
    if (test_Jar() != SUCCESS)
        return FAILURE;
    return SUCCESS;
}
