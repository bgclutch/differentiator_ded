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


Node* Differentiation(Node* node) {
    if (node->data_type == CONST || node->data_type == VARIABLE){
        node = DiffLeaf(node);
    }
    else if (node->data_type == OPERAND){
        if (node->value.arithmop.operand == ADD){
            node = AddDiff(node);
        }
        else if (node->value.arithmop.operand == SUB){
            node = SubDiff(node);
        }
        else if (node->value.arithmop.operand == MUL){
            node = MulDiff(node);
        }
        else if (node->value.arithmop.operand == DIV){
            node = DivDiff(node);
        }
        else{
            assert(0);
        }
    }
    #if 0
    else if (node->data_type == FUNCTION){
        if (node->value.funciton.func_num == SIN_NUM){
            node = SinDiff();
        }
        else if (node->value.funciton.func_num == COS_NUM){
            node = CosDiff();
        }
        else if (node->value.funciton.func_num == TAN_NUM){
            node = TanDiff();
        }
        else if (node->value.funciton.func_num == LN_NUM){
            node = LogDiff();
        }
        else{
            assert(0);
        }

    }
    #endif
    else {
        assert(0);
    }

    return node;
}


Node* AddDiff(Node* node) {
    return GETDIFFADDNODE(node->left, node->right);
}

Node* SubDiff(Node* node) {
    return GETDIFFSUBNODE(node->left, node->right);
}

Node* MulDiff(Node* node) {
    Node* cleft  = node->left;
    Node* cright = node->right; // FIXME FIXPLS with DSL here-----v
    return GETDIFFMULNODE(node->left, cleft, node->right, cright);
}

Node* DivDiff(Node* node) {
    Node* cleft  = node->left;
    Node* cright = node->right;
    return GETDIFFDIVNODE(node->left, cleft, node->right, cright);
}

#if 0
Node* PowDiff() {


    return InitNewNode();
}

Node* SinDiff() { // NOTE read next line comment
    return InitNewNode(FUNCTION, /*GetValue with mul*/, Differentiation(right), InitNewNode(, /*init with cos here*/,,)); // NOTE mul with diff of inside func or divide by cases???????
}

Node* CosDiff() {
    return InitNewNode();
}

Node* TanDiff() {

}

Node* LogDiff() {

}

#endif

Node* DiffLeaf(Node* node) {
    if (node->data_type == CONST)
        node->value.number = 0;
    else if (node->data_type == VARIABLE){
        node->value = CONSTVALUE(CONST, 1);
    }

    return node;
}


Node* CopyNode(Node* node) {
    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    new_node->data_type = node->data_type;
    new_node->parent    = node->parent;
    new_node->value     = node->value;
    if (node->left)
        new_node->left  = CopyNode(node->left);
    if (node->right)
        new_node->right = CopyNode(node->right);

    return new_node;
}