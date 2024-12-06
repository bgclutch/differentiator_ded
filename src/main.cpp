#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../inc/dump.h"
#include "../inc/treeRW.h"
#include "../inc/simplification.h"

int main(const int argc, const char* argv[])
{
    if (argc < 2){
        fprintf(stderr, "please, input your file in format:'*.txt'\n"
                        "program aborted\n");
        return EXIT_FAILURE;
    }
    Tree tree = tree_ctor(argv[1]);
    Dump_St dump_st = {};
    FILE* texdump = PrepareTexDumpFile(dump_st.TEX_DUMP);
    create_png(&dump_st, tree.root);
    WriteTreeToTex(tree.root, texdump, EQU);
    Simplification(tree.root);
    WriteTreeToTex(tree.root, texdump, EQU);
    Differentiation(tree.root);
    create_png(&dump_st, tree.root);
    WriteTreeToTex(tree.root, texdump, DEQU);
    Simplification(tree.root);
    create_png(&dump_st, tree.root);
    WriteTreeToTex(tree.root, texdump, DEQU);
    CloseAndCreateTeXpdf(texdump);
    tree_dtor(&tree);

    return EXIT_SUCCESS;
}
