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
    if (!ISVARINBRANCH(node->right)){ // var^const const^const
        return DIFFPOWCONST(node, cleft, cright);
    }
    else if (ISVARINBRANCH(node->right)) { // var^var
        return DIFFPOWVAR(node, cleft, cright);
    }
    else {
        assert(0);
    }
}
// TODO codegeneration?
Node* ChangeNode(Node* node, const Data_Type data_type, const Value_Type value, Node* left, Node* right) { // FIXME asserts !everywhere
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
    fprintf(stderr, YELLOW_TEXT("copied node:%p\n"
                    MAGENTA_TEXT("prev node:%p\n")), new_node, node);
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

void Simplification(Node* node, Dump_St* my_dump, Node* root) { // a lot copypaste but it all used once
    fprintf(stderr, "simplif is here\n%p\n", node);

    if (node->left){
        Simplification(node->left, my_dump, root);
    }
    if (node->right){
        Simplification(node->right, my_dump, root);
    }
    if (node->data_type == CONST){
        if (node->value.number < -EPSILON){
            node = ChangeNode(node, OPERAND, OPERVALUE(SUB_NUM), ChangeNode(node->left, CONST, CONSTVALUE(0), nullptr, nullptr),
                   ChangeNode(node->right, CONST, CONSTVALUE(fabs(node->value.number)), nullptr, nullptr));
        }
        return;
    }
    else if (node->data_type == VARIABLE){
        return;
    }
    if (node->data_type != FUNCTION){
        if (node->left->data_type == CONST && node->left->data_type == node->right->data_type){
            if (node->value.arithmop.operand_num == DIV_NUM && IsZero(node->right->value.number))
                assert(0);
            if (!IsZero(node->left->value.number)){
                node = ChangeNode(node, CONST, CONSTVALUE(GetOperResult(node)), node->left, node->right);
                free(node->left);
                free(node->right);
                node->left = nullptr;
                node->right = nullptr;
            }
        }
        else if (node->data_type == OPERAND && node->value.arithmop.operand_num == MUL_NUM){
            if (node->left->data_type == CONST && IsZero(node->left->value.number)){
                tree_branch_dtor(node->right);
                tree_branch_dtor(node->left);
                node = ChangeNode(node, CONST, CONSTVALUE(0), nullptr, nullptr);
            }
            else if (node->right->data_type == CONST && IsZero(node->right->value.number)){
                tree_branch_dtor(node->left);
                node->value = node->right->value;
                node->data_type = node->right->data_type;
                tree_branch_dtor(node->right);
                node->left  = nullptr;
                node->right = nullptr;
            }
            else if (node->left->data_type == CONST &&
                     1 - EPSILON < node->left->value.number && node->left->value.number < 1 + EPSILON){
                tree_branch_dtor(node->left);
                Node* chnode = CopyNode(node->right);
                tree_branch_dtor(node->right);
                node = ChangeNode(node, chnode->data_type, chnode->value, chnode->left, chnode->right);
                free(chnode);
            }
            else if (node->right->data_type == CONST &&
                     1 - EPSILON < node->right->value.number && node->right->value.number < 1 + EPSILON){
                tree_branch_dtor(node->right);
                Node* chnode = CopyNode(node->left);
                tree_branch_dtor(node->left);
                node = ChangeNode(node, chnode->data_type, chnode->value, chnode->left, chnode->right);
                free(chnode);
            }
        }
        else if (node->data_type == OPERAND && node->value.arithmop.operand_num == DIV_NUM){
            if (node->left->data_type == CONST && IsZero(node->left->value.number)){
                tree_branch_dtor(node->right);
                node->value = node->left->value;
                node->data_type = node->left->data_type;
                tree_branch_dtor(node->left);
                node->left  = nullptr;
                node->right = nullptr;
            }
            else if (node->right->data_type == CONST && IsZero(node->right->value.number)){
                assert(0); // soft return!!!
            }
            else if (node->right->data_type == CONST &&
                     1 - EPSILON < node->right->value.number && node->right->value.number < 1 + EPSILON){
                tree_branch_dtor(node->right);
                Node* chnode = CopyNode(node->left);
                tree_branch_dtor(node->left);
                node = ChangeNode(node, chnode->data_type, chnode->value, chnode->left, chnode->right);
                free(chnode);
            }
        }
        else if (node->data_type == OPERAND && node->value.arithmop.operand_num == POW_NUM){
           if (node->left->data_type == CONST && IsZero(node->left->value.number)){
                tree_branch_dtor(node->right);
                node->value = node->left->value;
                node->data_type = node->left->data_type;
                tree_branch_dtor(node->left);
                node->left  = nullptr;
                node->right = nullptr;
            }
            else if (node->right->data_type == CONST && IsZero(node->right->value.number)){
                assert(0); // soft return!!!
            }
            else if (node->right->data_type == CONST &&
                     1 - EPSILON < node->right->value.number && node->right->value.number < 1 + EPSILON){
                tree_branch_dtor(node->right);
                Node* chnode = CopyNode(node->left);
                tree_branch_dtor(node->left);
                node = ChangeNode(node, chnode->data_type, chnode->value, chnode->left, chnode->right);
                free(chnode);
            }
        }
    }
    return;
}


double GetOperResult(Node* node) {
    double num = 0;
    switch (node->value.arithmop.operand_num){
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
        case SYNTERR_N:
            assert(0);
        default:
            assert(0);
    }
    return num;
}

int IsZero(const double num) {
    return fabs(num) < EPSILON;
}