#include "../inc/bintree.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/exprtree.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../inc/colorised_output.h"

Node* InitNewNode(const Data_Type data_type, const Value_Type value, Node* left, Node* right) {
    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    switch (data_type) {
    case VARIABLE:
        new_node->data_type = VARIABLE;
        break;

    case CONST:
        new_node->data_type = CONST;
        break;

    case OPERAND:
        new_node->data_type = OPERAND;
        break;

    case FUNCTION:
        new_node->data_type = FUNCTION;
        break;

    case SYNTAXERROR: // not good
        assert(0);

    default:
        assert(0);
    }

    new_node->value = value;
    new_node->left  = left;
    new_node->right = right;
    if (left)
        left->parent  = new_node;
    if (right)
        right->parent = new_node;

    return new_node;
}


Tree_Errors tree_is_err(const Tree_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != NODE_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_ERR;
    }

    return MACRO_GOOD;
}


void tree_branch_dtor(Node* node, const char* data, const size_t len)
{
    assert(node);
    assert(data);

    if(node->left != nullptr)
    {
        tree_branch_dtor(node->left, data, len);
    }
    if(node->right != nullptr)
    {
        tree_branch_dtor(node->right, data, len);
    }

    free(node);

    return;
}

Value_Type GetValue(const Data_Type data_type, const int op_num, const char variable,
                    const int func_num, const double number)
{
    Value_Type value = {};
    switch(data_type) {
        case VARIABLE:
            value.varaible = variable;
            break;

        case CONST:
            assert(!isnan(number));
            value.number = number;
            break;

        case OPERAND:
            value.arithmop = operand_array[op_num];
            break;

        case FUNCTION:
            value.funciton = func_array[func_num];
            break;

        case SYNTAXERROR:
            assert(0);

        default:
            assert(0);
    }

    return value;
}