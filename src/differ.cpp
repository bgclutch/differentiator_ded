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
            node = AddDiff();
        }
        else if (node->value.arithmop.operand == SUB){
            node = SubDiff();
        }
        else if (node->value.arithmop.operand == MUL){
            node = MulDiff();
        }
        else if (node->value.arithmop.operand == DIV){
            node = DivDiff();
        }
        else{
            assert(0);
        }
    }
    else if (node->data_type == FUNCTION){
        if (node->value.funciton.func_num == SIN_NUM){
            node = SinDiff();
        }
        else if (node->value.funciton.func_num == COS_NUM){
            node = CosDiff();
        }
        else if (node->value.funciton.func_num == COS_NUM){
            node = TanDiff();
        }
        else if (node->value.funciton.func_num == COS_NUM){
            node = LogDiff();
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


Node* AddDiff(Node* left, Node* right) {
    Value_Type value = {};
    value.arithmop.operand     = ADD;
    value.arithmop.operand_num = ADD_NUM;
    return InitNewNode(OPERAND, value, Differentiation(left), Differentiation(right)); // NOTE DSL HERE
}

Node* SubDiff(Node* left, Node* right) {
    Value_Type value = {};
    value.arithmop.operand     = SUB;
    value.arithmop.operand_num = SUB_NUM;
    return InitNewNode(OPERAND, value, Differentiation(left), Differentiation(right));
}

Node* MulDiff() {
    Value_Type value = {};
    value.arithmop.operand     = ADD;
    value.arithmop.operand_num = ADD_NUM;
    Node* copied_left  = node->left;
    Node* copied_right = node->right; // FIXME FIXPLS with DSL here-----v
    return InitNewNode(OPERAND, value, InitNewNode(OPERAND, GetValue(), Differentiation(left), copied_right), InitNewNode(OPERAND, GetValue!!!, copied_left, Differentiation(right)))
}

Node* DivDiff() {
    Value_Type value = {};
    value.arithmop.operand     = DIV;
    value.arithmop.operand_num = DIV_NUM;
    Node* copied_left_subtree  = node->left;
    Node* copied_right_subtree = node->right;
    return InitNewNode(OPERAND, , /*THIS INIT SHOULD BE WITH DOUBLE INITNEWNODE INSIDE*/ InitNewNode(), /*power 2 for right node*/InitNewNode())
}

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


Node* ChangeNode() { // for what??

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
