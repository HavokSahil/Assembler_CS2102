#include <common_ds.h>
#include <err_codes.h>

int test_SymTable() {
    SymTable *table = ds_new_SymTable();
    if (table == NULL)
        return FAILURE;
    
    AString keys[] = {"Sahil", "Raj", "is", "Here"};
    AAddr values[] = {1, 2, 3, 4};

    int i;
    for (i = 0; i<4; i++) {
        if (table->insert(table, keys[i], values[i]) != SUCCESS) {
            table->destroy(table);
            return FAILURE;
        }
        if (table->size(table) != (i+1))
            return FAILURE;
    }

    for (i = 0; i<4; i++) {
        if (table->find(table, keys[i]) != values[i])
            return FAILURE;
    }
    
    SymItem* item = table->get(table);

    for (i = 0; i<4; i++) {
        if (item == table->end())
            return FAILURE;
        item = table->get(NULL);
    }
    table->destroy(table);
    return SUCCESS;
}

int test_DList() {
    DList* dlist = ds_new_DList();
    if (dlist == NULL)
        return FAILURE;

    AAddr addrs[] = {0, 4, 8, 12};
    AInt32 datas[] = {100, 200, 300, 400};

    int i;
    for (i = 0; i<4; i++) {
        if (dlist->insert(dlist, addrs[i], datas[i]) != SUCCESS) {
            return FAILURE;
        }
        if (dlist->size(dlist) != (i+1))
            return FAILURE;
    }

    for (i = 0; i<4; i++) {
        DItem *item = dlist->find(dlist, addrs[i]);
        if (item == NULL)
            return FAILURE;
        if (datas[i] != item->data)
            return FAILURE;
    }

    DItem* item = dlist->get(dlist);
    for (i = 0; i<4; i++) {
        if (item == NULL)
            return FAILURE;

        item = dlist->get(NULL);
    }

    dlist->destroy(dlist);
    return SUCCESS;
}

int test_IList() {
    IList* ilist = ds_new_IList();
    if (ilist == NULL)
        return FAILURE;

    int i;
    for (i = 0; i<4; i++) {
        IItem *item = ds_new_IItem(i*5);
        if (item == NULL)
            return FAILURE;
        if (ilist->insert(ilist, item) != SUCCESS)
            return FAILURE;
    }
    IItem *item = ilist->get(ilist);
    for (i = 0; i<4; i++) {
        if (item == ilist->end())
            return FAILURE;
        item = ilist->get(NULL);
    }   
    ilist->get(NULL);

    ilist->destroy(ilist);
    return SUCCESS;
}

int test_EWList() {
    EWList* elist = ds_new_EWList();
    if (elist == NULL)
        return FAILURE;

    int i;
    for (i = 0; i<4; i++) {
        EWItem* eitem = ds_new_EWItem(i, 0, FAILURE);
        if (eitem == NULL)
            return FAILURE;
        if (elist->insert(elist, eitem) != SUCCESS)
            return FAILURE;
    }
    EWItem* eitem = elist->get(elist);
    for (i = 0; i<4; i++) {
        if (eitem == elist->end())
            return FAILURE;
        eitem = elist->get(NULL);
    }
    elist->get(NULL);

    elist->destroy(elist);
    return SUCCESS;
}

int main() {
    if (test_SymTable() != SUCCESS)
        return FAILURE;
    if (test_DList() != SUCCESS)
        return FAILURE;
    if (test_IList() != SUCCESS)
        return FAILURE;
    if (test_EWList() != SUCCESS)
        return FAILURE;
    return SUCCESS;
}
