#include <tokenizer/tokenizer.h>
#include <time.h>

#define SUCCESS 0
#define FAILURE 1

int test_cargo() {
    Cargo* cargo = tk_new_Cargo();
    if (cargo == NULL)
        return FAILURE;

    AString strings[] = {"Hello", "World", "I", "am", "Assembler"};
    int i;
    for (i = 0; i<5; i++) {
        if (cargo->load(cargo, i, strings[i]) != SUCCESS) {
            return FAILURE;
        }
    }

    for (i = 0; i<5; i++) {
        AString string = cargo->get(cargo, i);
    }
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
        Token* token = tk_new_Token(TYPE_TOK_COMMENT, strings[i]);
        if (token == NULL)
            return FAILURE;
        if (jar->put(jar, token) != SUCCESS)
            return FAILURE;
    }

    for (i = 0; i<5; i++) {
        Token* token = jar->get(jar, i);
        if (token == NULL)
            return FAILURE;
    }

    return SUCCESS;
}

int test_tokenizer_interface() {
    FILE* file = fopen("hello.txt", "r");
    if (file == NULL)
        return FAILURE;
    TokInterface* ti = tk_new_TokInterface(file, 20);
    if (ti == NULL)
        return FAILURE;

    Cargo* cargo = tk_new_Cargo();
    if (cargo == NULL)
        return FAILURE;

    double start = clock();
    while (ti->status == 0) {
        if (ti->fillCargo(ti, cargo) != SUCCESS)
            return FAILURE;

        printf("The size of cargo is %d\n", ti->cargo->size);
        printf("The status is %s.\n", (ti->status == 1)? "Completed": "Remaining");
        printf("The percentage of work done is %d\n", ti->percentageWorkDone(ti));
    }
    double end = clock();

    ASize i;
    for (i = 0; i<cargo->size; i++) {
        Jar* jar = cargo->get(cargo, i);
        if (jar == NULL)
            return FAILURE;
        ASize jarSize = jar->size;
        ASize j;
        printf("Jar(%d) [Size: %d]\n", jar->lno, jar->size);
        for (j = 0; j<jarSize; j++) {
            Token* token = jar->get(jar, j);
            if (token == NULL)
                return FAILURE;

            if (token->cno != UNSET_TOK_COL)
                printf("%s (col:%d, type: %d), ", token->token, token->cno, token->type);
        }
        printf("\n\n");
    }

    fclose(file);
    ti->destroy(ti);
    cargo->destroy(cargo, TRUE);

    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The time taken to complete code is %f\n", cpu_time_used);
    printf("Memory Freed.\n");
    return SUCCESS;
}

int main() {
    if (test_cargo() != SUCCESS)
        return FAILURE;
    printf("DEBUG: 1\n");
    if (test_Jar() != SUCCESS)
        return FAILURE;
    printf("DEBUG: 2\n");
    if (test_tokenizer_interface() != SUCCESS)
        return FAILURE;
    printf("DEBUG: 3\n");
    return SUCCESS;
}
