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
        else if (node->value.arithmop.operand == POW){
            node = PowDiff(node);
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
    return GETDIFFADDNODE(node);
}

Node* SubDiff(Node* node) {
    return GETDIFFSUBNODE(node);
}

Node* MulDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return GETDIFFMULNODE(node, cleft, cright);
}

Node* DivDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return GETDIFFDIVNODE(node, cleft, cright);
}

Node* PowDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    if (IsVarInBranch(node->left) && !IsVarInBranch(node->right)){ // var^const
        return GETDIFFPOWNODEVARCONST(node, cleft, cright);
    }
    else if (!IsVarInBranch(node->left) && IsVarInBranch(node->right)) { // const^var
        return GETDIFFPOWNODECONSTVAR(node, cleft, cright);
    }
    else if (IsVarInBranch(node->left) && IsVarInBranch(node->right)) { // var^var
        assert(0);
    }
    else if (!IsVarInBranch(node->left) && !IsVarInBranch(node->right)) { // just a number
        free(cleft);
        free(cright);
        free(node->left);
        free(node->right);
        return ChangeNode(node, CONST, CONSTVALUE(0), nullptr, nullptr);
    }
    else {
        assert(0);
    }
}

Node* ChangeNode(Node* node, const Data_Type data_type, const Value_Type value, Node* left, Node* right) {
    switch (data_type) {
    case VARIABLE:
        node->data_type = VARIABLE;
        break;

    case CONST:
        node->data_type = CONST;
        break;

    case OPERAND:
        node->data_type = OPERAND;
        break;

    case FUNCTION:
        node->data_type = FUNCTION;
        break;

    case SYNTAXERROR: // not good
        assert(0);

    default:
        assert(0);
    }

    node->value = value;
    node->left  = left;
    node->right = right;
    if (left)
        left->parent  = node;
    if (right)
        right->parent = node;

    return node;
}

int IsVarInBranch(Node* node) { // node->left needed for base node->right needed for power
    if (node->left)
        return IsVarInBranch(node->left);
    if (node->right)
        return IsVarInBranch(node->right);

    int value = 0;
    if (node->data_type == VARIABLE)
        value = 1;
    if (node->data_type == CONST)
        value = 0;
    return value;
}

#if 0
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
        node->data_type = CONST;
        node->value = CONSTVALUE(1);
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
