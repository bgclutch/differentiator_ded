#include "../inc/differ.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/dump.h"
#include "../inc/exprtree.h"
#include "../inc/dsl.h"
#include "../inc/colorised_output.h"
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
    else if (node->data_type == FUNCTION){
        if (node->value.funciton.func_num == SIN_NUM){
            node = SinDiff(node);
        }
        else if (node->value.funciton.func_num == COS_NUM){
            node = CosDiff(node);
        }
        else if (node->value.funciton.func_num == TAN_NUM){
            node = TanDiff(node);
        }
        else if (node->value.funciton.func_num == LN_NUM){
            node = LogDiff(node);
        }
        else{
            assert(0);
        }
    }
    else {
        assert(0);
    }

    return node;
}


Node* AddDiff(Node* node) {
    return DIFFADD(node);
}

Node* SubDiff(Node* node) {
    return DIFFSUB(node);
}

Node* MulDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return DIFFMUL(node, cleft, cright);
}

Node* DivDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return DIFFDIV(node, cleft, cright);
}

Node* PowDiff(Node* node) {
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    if (ISVARINBRANCH(node->left) && !ISVARINBRANCH(node->right)){ // var^const
        return DIFFPOWVARCONST(node, cleft, cright);
    }
    else if (!ISVARINBRANCH(node->left) && ISVARINBRANCH(node->right)) { // const^var
        return DIFFPOWCONSTVAR(node, cleft, cright);
    }
    else if (ISVARINBRANCH(node->left) && ISVARINBRANCH(node->right)) { // var^var
        return DIFFPOWVARVAR(node, cleft, cright);
    }
    else if (!ISVARINBRANCH(node->left) && !ISVARINBRANCH(node->right)) { // just a number
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
// TODO codegeneration
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

Node* SinDiff(Node* node) {
    Node* cright = CopyNode(node->right);
    return DIFFSIN(node, cright);
}

Node* CosDiff(Node* node) {
    Node* cright = CopyNode(node->right);
    return DIFFCOS(node, cright);
}

Node* TanDiff(Node* node) {
    Node* cright = CopyNode(node->right);
    return DIFFTAN(node, cright);
}

Node* LogDiff(Node* node) {
    Node* cright = CopyNode(node->right);
    return DIFFLN(node, cright);
}

int IsSmthInBranch(Node* node, Data_Type data_type) { // node->left needed for base node->right needed for power
    if (node->left)
        return IsSmthInBranch(node->left, data_type);
    if (node->right)
        return IsSmthInBranch(node->right, data_type);

    int value = 0;
    if (node->data_type == data_type)
        value = 1;
    else
        value = 0;
    return value;
}


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
    if (node->left){
        new_node->left  = CopyNode(node->left);
        new_node->left->parent = new_node;
    }
    if (node->right){
        new_node->right = CopyNode(node->right);
        new_node->right->parent = new_node;
    }

    return new_node;
}
