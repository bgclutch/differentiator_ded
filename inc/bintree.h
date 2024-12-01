#ifndef BINTREE_H_
#define BINTREE_H_

#include <stdio.h>
#include "../inc/exprtree.h"

#define NODE_ERR(result, error) if(tree_is_err(result, __FILE__, __LINE__) == MACRO_ERR) return error

typedef char* NodeElem_t;

enum Tree_Errors
{
    NODE_IS_OKAY    = 0x00,
    NODE_CTOR_ERR   = 0x01,
    NODE_FIND_ERR   = 0x02,
    NODE_INIT_ERR   = 0x03,
    NODE_INSERT_ERR = 0x04,
    NEWNODE_CRE_ERR = 0x05,
    NODE_IS_BRANCH  = 0x06,
    DENY_TO_EDIT    = 0x07,


    MACRO_ERR  = 0x20,
    MACRO_GOOD = 0x30,

};

#define OPERVALUE(  index) GetValue(OPERAND,  index,        0,     0,     NAN)
#define VARVALUE(variable) GetValue(VARIABLE,     0, variable,     0,     NAN)
#define FUNCVALUE(  index) GetValue(FUNCTION,     0,        0, index,     NAN)
#define CONSTVALUE(number) GetValue(CONST,        0,        0,     0,  number)


void tree_branch_dtor(Node* node, const char* data, const size_t len);

Tree_Errors tree_is_err(const Tree_Errors result, const char* name, const size_t line);

Node* InitNewNode(const Data_Type data_type, const Value_Type value, Node* left, Node* right);

Value_Type GetValue(const Data_Type data_type, const int op_num, const char variable,
                    const int func_num, const double number);

#endif // BINTREE_H_