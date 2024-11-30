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

#define GETVARNODE(variable)               InitNewNode(VARIABLE,  VARVALUE(VARIABLE, variable),  nullptr, nullptr)
#define GETCONSTNODE(value)                InitNewNode(CONST,     CONSTVALUE(CONST, value),      nullptr, nullptr)
#define GETOPERNODE(oper_num, left, right) InitNewNode(OPERAND,   OPERVALUE(OPERAND, oper_num),     left,   right)
#define GETFUNCNODE(func_num, right)       InitNewNode(FUNCTION,  FUNCVALUE(FUNCTION, func_num), nullptr,   right)


#define GETDIFFADDNODE(left, right)                GETOPERNODE(ADD_NUM, Differentiation(left), Differentiation(right))
#define GETDIFFSUBNODE(left, right)                GETOPERNODE(SUB_NUM, Differentiation(left), Differentiation(right))
#define GETDIFFMULNODE(left, cleft, right, cright) GETOPERNODE(ADD_NUM, GETOPERNODE(MUL_NUM, Differentiation(left), cright), GETOPERNODE(MUL_NUM, cleft, Differentiation(right)))
#define GETDIFFDIVNODE(left, cleft, right, cright) GETOPERNODE(DIV, GETOPERNODE(SUB, GETOPERNODE(MUL_NUM, Differentiation(left), cright), GETOPERNODE(MUL_NUM, cleft, Differentiation(right))), GETOPERNODE(POW_NUM, right, GETCONSTNODE(2)))



Differ_Err differ_is_err(const Differ_Err result, const char* name, const size_t line);

Node* Differentiation(Node* node);

Node* AddDiff(Node* node);

Node* SubDiff(Node* node);

Node* MulDiff(Node* node);

Node* DivDiff(Node* node);

Node* CopyNode(Node* node);

Node* DiffLeaf(Node* node);

#endif // DIFFER_H_
