#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../inc/dump.h"
#include "../inc/treeRW.h"

int main(const int argc, const char* argv[])
{
    if (argc < 2){
        fprintf(stderr, "please, input your file in format:'*.txt'\n"
                        "program aborted\n");
        return EXIT_FAILURE;
    }
    Tree tree = tree_ctor(argv[1]);
    Dump_St dump_st = {};
    create_png(&dump_st, tree.root);
    Differentiation(tree.root);
    create_png(&dump_st, tree.root);
    Simplification(tree.root);
    create_png(&dump_st, tree.root);
    tree_dtor(&tree);

    return EXIT_SUCCESS;
}
