#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../inc/dump.h"
#include "../inc/treeRW.h"

int main(void)
{
    Variable_Array_St variable_array_st = {};
    CtorVariablesArray(&variable_array_st);
    Tree tree = tree_ctor(&variable_array_st);
    Dump_St dump_st = {};
    create_png(&dump_st, tree.root);
    tree_dtor(&tree, &variable_array_st);

    return EXIT_SUCCESS;
}
