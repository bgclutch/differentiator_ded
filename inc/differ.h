#ifndef DIFFER_H_
#define DIFFER_H_

#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"
#include "bintree.h"

#define AKN_ERR(result, error) if(akinator_is_err(result, __FILE__, __LINE__) == MACRO_AK_ERR) return error


enum Differ_Err
{
    DIFFER_IS_OKAY               = 0x00,
    DIFFER_IS_DEAD               = 0x01,
    DIFFER_BUFFER_CTOR_ERR       = 0x02,
    DIFFER_BUFFER_FILE_OPEN_ERR  = 0x03,
    DIFFER_BUFFER_FILE_CLOSE_ERR = 0x04,
    DIFFER_FILE_ERROR            = 0x05,
    CHOSEN_WORD_GETLINE_ERR      = 0x06,


    MACRO_AK_GOOD = 0x20,
    MACRO_AK_ERR  = 0x30,

};

enum Text_Colors
{
    REDTEXT,
    BLUETEXT,
    GREENTEXT,
    MAGENTATEXT,
    YELLOWTEXT,
};


// TODO add macros for arithm operations

#define GETVARNODE(  variable)              InitNewNode(VARIABLE,  VARVALUE( variable), nullptr, nullptr)
#define GETCONSTNODE(value)                 InitNewNode(CONST,     CONSTVALUE(  value), nullptr, nullptr)
#define GETOPERNODE( oper_num, left, right) InitNewNode(OPERAND,   OPERVALUE(oper_num),    left,   right)
#define GETFUNCNODE( func_num, right)       InitNewNode(FUNCTION,  FUNCVALUE(func_num), nullptr,   right)


#define GETDIFFVARNODE(  node, variable)              ChangeNode(node, VARIABLE,  VARVALUE( variable), nullptr, nullptr)
#define GETDIFFCONSTNODE(node, value)                 ChangeNode(node, CONST,     CONSTVALUE(  value), nullptr, nullptr)
#define GETDIFFOPERNODE( node, oper_num, left, right) ChangeNode(node, OPERAND,   OPERVALUE(oper_num),    left,   right)
#define GETDIFFFUNCNODE( node, func_num, right)       ChangeNode(node, FUNCTION,  FUNCVALUE(func_num), nullptr,   right)


#define GETDIFFADDNODE(node)                GETDIFFOPERNODE(node, ADD_NUM, Differentiation(node->left), Differentiation(node->right))
#define GETDIFFSUBNODE(node)                GETDIFFOPERNODE(node, SUB_NUM, Differentiation(node->left), Differentiation(node->right))
#define GETDIFFMULNODE(node, cleft, cright) GETDIFFOPERNODE(node, ADD_NUM, GETOPERNODE(MUL_NUM, Differentiation(node->left), cright), GETOPERNODE(MUL_NUM, cleft, Differentiation(node->right)))
#define GETDIFFDIVNODE(node, cleft, cright) GETDIFFOPERNODE(node, DIV_NUM, GETOPERNODE(SUB_NUM, GETOPERNODE(MUL_NUM, Differentiation(node->left), cright), GETOPERNODE(MUL_NUM, cleft, Differentiation(node->right))), GETOPERNODE(POW_NUM, CopyNode(cright), GETCONSTNODE(2)))
#define GETDIFFPOWNODEVARCONST(node, cleft, cright) GETDIFFOPERNODE(node, MUL_NUM, GETOPERNODE(MUL_NUM, cright, GETOPERNODE(POW_NUM, cleft, GETOPERNODE(SUB_NUM, cright, GETCONSTNODE(1)))), Differentiation(node->left))
#define GETDIFFPOWNODECONSTVAR(node, cleft, cright) GETDIFFOPERNODE(node, MUL_NUM, GETOPERNODE(DIV_NUM, GETCONSTNODE(1), GETOPERNODE(MUL_NUM, GETFUNCNODE(LN_NUM, cleft), cright)), Differentiation(node->left))
// #define GETDIFFPOWNODEVARVAR(  node, cleft, cright) GETDIFFOPERNODE(node, POW_NUM, GETOPERNODE(), GETOPERNODE())


Differ_Err differ_is_err(const Differ_Err result, const char* name, const size_t line);

Node* ChangeNode(Node* node, const Data_Type data_type, const Value_Type value, Node* left, Node* right);

int IsVarInBranch(Node* node);

Node* Differentiation(Node* node);

Node* AddDiff(Node* node);

Node* SubDiff(Node* node);

Node* MulDiff(Node* node);

Node* DivDiff(Node* node);

Node* PowDiff(Node* node);

Node* CopyNode(Node* node);

Node* DiffLeaf(Node* node);

#endif // DIFFER_H_
