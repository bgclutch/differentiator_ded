#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../inc/dump.h"
#include "../inc/treeRW.h"

int main(void)
{
    Tree tree = tree_ctor();
    Dump_St dump_st = {};
    create_png(&dump_st, tree.root);
    tree_dtor(&tree);

    return EXIT_SUCCESS;
}
