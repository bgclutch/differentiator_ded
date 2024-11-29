#include "../inc/differ.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/dump.h"
#include "../inc/exprtree.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../../stack_ded/stack_headers/colorised_output.h"


Differ_Err differ_is_err(const Differ_Err result, const char* name, const size_t line)
{
    assert(name);

    if(result != DIFFER_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_AK_ERR;
    }

    return MACRO_AK_GOOD;
}



Node* Differentiation() {

}



Node* SumDiff(Node* node) {
    node->left  = Differentiation(node->left);
    node->right = Differentiation(node->right);
    return node;
}

Node* SubDiff() {

}

Node* MulDiff() {

}

Node* DivDiff() {

}

Node* PowDiff() {

}

Node* SinDiff() {

}

Node* CosDiff() {

}

Node* TanDiff() {

}

Node* LogDiff() {

}


Node* ChangeNode() {

}


Node* DiffLeaf(Node* node) {
    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    new_node->data_type = node->data_type;
    new_node->parent    = node->parent;
    new_node->left      = node->left;
    new_node->right     = node->right;
    if (node->data_type == CONST)
        new_node->value.number = 0;
    else if (node->data_type == VARIABLE)
        new_node->value.number = 1;

    free(node);
    return new_node;
}
