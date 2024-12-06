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
    assert(node);
    if (node->data_type == CONST || node->data_type == VARIABLE){
        node = DiffLeaf(node);
    }
    else if (node->data_type == OPERATOR){
        if (node->value.arithmop.oper == ADD){
            node = AddDiff(node);
        }
        else if (node->value.arithmop.oper == SUB){
            node = SubDiff(node);
        }
        else if (node->value.arithmop.oper == MUL){
            node = MulDiff(node);
        }
        else if (node->value.arithmop.oper == DIV){
            node = DivDiff(node);
        }
        else if (node->value.arithmop.oper == POW){
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
    assert(node);
    return DIFFADD(node);
}

Node* SubDiff(Node* node) {
    assert(node);
    return DIFFSUB(node);
}

Node* MulDiff(Node* node) {
    assert(node);
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return DIFFMUL(node, cleft, cright);
}

Node* DivDiff(Node* node) {
    assert(node);
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    return DIFFDIV(node, cleft, cright);
}

Node* PowDiff(Node* node) {
    assert(node);
    Node* cleft  = CopyNode(node->left);
    Node* cright = CopyNode(node->right);
    int result = 0;
    ISVARINBRANCH(node->right, result);
    if (!result) { // var^var
        return DIFFPOWVAR(node, cleft, cright);
    }
    else if (result){ // var^const const^const
        return DIFFPOWCONST(node, cleft, cright);
    }
    else {
        assert(0);
    }
}
// TODO codegeneration!
Node* ChangeNode(Node* node, const Data_Type data_type, const Value_Type value, Node* left, Node* right) { // FIXME asserts !everywhere
    assert(node);
    switch (data_type) {
    case VARIABLE:
        node->data_type = VARIABLE;
        break;
    case CONST:
        node->data_type = CONST;
        break;
    case OPERATOR:
        node->data_type = OPERATOR;
        break;
    case FUNCTION:
        node->data_type = FUNCTION;
        break;
    case SYNTAXERROR: // not good
        assert(0);
    default:
        assert(0);
    }


    #if 0
    #define PUTNODETYPE(Data_Type)\
    case ##Data_Type:\
        node->data_type = ##Data_Type;\
        break;

    switch (data_type) {
        #include "../inc/getnodetype.h"
        default:
            assert(0);
    }

    #undef PUTNODETYPE
    #endif


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
    assert(node);
    Node* cright = CopyNode(node->right);
    return DIFFSIN(node, cright);
}

Node* CosDiff(Node* node) {
    assert(node);
    Node* cright = CopyNode(node->right);
    return DIFFCOS(node, cright);
}

Node* TanDiff(Node* node) {
    assert(node);
    Node* cright = CopyNode(node->right);
    return DIFFTAN(node, cright);
}

Node* LogDiff(Node* node) {
    assert(node);
    Node* cright = CopyNode(node->right);
    return DIFFLN(node, cright);
}

void IsSmthInBranch(Node* node, Data_Type data_type, int result) { // node->left needed for base node->right needed for power
    assert(node);
    if (node->data_type == data_type){
        result = 1;
    }
    else{
        if (node->left)
            return IsSmthInBranch(node->left, data_type, result);
        if (node->right)
            return IsSmthInBranch(node->right, data_type, result);
    }
}


Node* DiffLeaf(Node* node) {
    assert(node);
    if (node->data_type == CONST)
        node->value.number = 0;
    else if (node->data_type == VARIABLE){
        node->data_type = CONST;
        node->value = CONSTVALUE(1);
    }

    return node;
}


Node* CopyNode(Node* node) {
    assert(node);
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

// TODO codegeneration
double GetOperResult(Node* node) {
    assert(node);
    double num = 0;
    switch (node->value.arithmop.operator_num){
        case ADD_NUM:
            num = node->left->value.number + node->right->value.number;
            break;
        case SUB_NUM:
            num = node->left->value.number - node->right->value.number;
            break;
        case MUL_NUM:
            num = node->left->value.number * node->right->value.number;
            break;
        case DIV_NUM:
            num = node->left->value.number / node->right->value.number;
            break;
        case POW_NUM:
            num = pow(node->left->value.number, node->right->value.number);
            break;
        case BSCOPE_NUM:
        case CSCOPE_NUM:
            break;
        case SYNTERR_N:
            assert(0);
        default:
            assert(0);
    }

    return num;
    #if 0
    #define GETOPERATIONRESULT(first, second, oper)\
        return first ##oper second

    switch (node->value.arithmop.operator_num){
        #include "../inc/operres.h"
        default:
            assert(0);
    }
    #undef GETOPERATIONRESULT
    #endif
}

int IsZero(const double num) {
    return fabs(num) < EPSILON;
}