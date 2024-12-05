#include "../inc/simplification.h"
#include "../inc/dsl.h"
#include "../inc/differ.h"

void Simplification(Node* node) { // a lot copypaste but it all used once
    if (node->left){
        Simplification(node->left);
    }
    if (node->right){
        Simplification(node->right);
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
    if (node->data_type == FUNCTION){
        Simplification(node->right);
    }
    else
    if (node->left->data_type == CONST && node->left->data_type == node->right->data_type){
        if (node->value.arithmop.operand_num == DIV_NUM && IsZero(node->right->value.number))
            assert(0);
        if (IsZero(node->left->value.number) && node->value.arithmop.operand_num == SUB_NUM && !IsZero(node->right->value.number)){
            return;
        }
        node = ChangeNode(node, CONST, CONSTVALUE(GetOperResult(node)), node->left, node->right);
        free(node->left);
        free(node->right);
        node->left  = nullptr;
        node->right = nullptr;
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
        if (node->right->data_type == CONST && IsZero(node->right->value.number)){
            tree_branch_dtor(node->right);
            tree_branch_dtor(node->left);
            node = ChangeNode(node, CONST, CONSTVALUE(0), nullptr, nullptr);
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
    return;
}