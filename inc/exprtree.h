#ifndef TREE_H_
#define TREE_H_

#include <math.h>

enum Operand_Num{
    SYNTERR_N   = -1,
    ADD_NUM     =  0,
    SUB_NUM     =  1,
    MUL_NUM     =  2,
    DIV_NUM     =  3,
    POW_NUM     =  4,
};

struct Operand{
    char            operand;
    Operand_Num operand_num;
};

enum Algebra_Func_Num{
    SYNTERR_NUM = -1,
    SIN_NUM     =  0,
    COS_NUM     =  1,
    TAN_NUM     =  2,
    LN_NUM      =  3,
};


struct Algebra_Func{
    const char*          func;
    Algebra_Func_Num func_num;
};

struct Value_Type{
    Operand      arithmop;
    char         varaible;
    Algebra_Func funciton;
    double         number;
};

enum Data_Type{
    SYNTAXERROR = -1,
    VARIABLE    =  0,
    CONST       =  1,
    OPERAND     =  2,
    FUNCTION    =  3,
};

struct Node{
    Node*        parent;
    Data_Type data_type;
    Value_Type    value;
    Node*          left;
    Node*         right;
};

struct Tree
{
    Node* root;
    char* buffer;
};

static const char ADD = '+'; // maybe struct array too?
static const char SUB = '-';
static const char MUL = '*';
static const char DIV = '/';
static const char POW = '^';

static const char* const SIN = "sin"; // maybe struct array too?
static const char* const COS = "cos";
static const char* const TAN = "tan";
static const char* const LN  = "ln";

const Algebra_Func func_array[] = {{SIN, SIN_NUM},
                                   {COS, COS_NUM},
                                   {TAN, TAN_NUM},
                                   {LN, LN_NUM}};

const Operand operand_array[]   = {{ADD, ADD_NUM},
                                   {SUB, SUB_NUM},
                                   {MUL, MUL_NUM},
                                   {DIV, DIV_NUM},
                                   {POW, POW_NUM}};

enum Tree_Err
{
    TREE_IS_OKAY_SH        = 0x00,
    DATA_FILE_OPEN_ERR     = 0x01,
    DATA_FILE_CLOSE_ERR    = 0x02,
    DATA_ACCESS_ERR        = 0x03,
    DATA_BUFFER_CREATE_ERR = 0x04,
};

int IsOperand(const char argument);

int IsConst(char argument);

int IsFunction(const char* argument, const size_t arg_size);

char GetOperand(const char argument);

Operand_Num GetOperandNum(const char argument);

const char* GetAlgFunc(const char* argument, const size_t arg_size);

Algebra_Func_Num GetAlgFuncNum(const char* argument, const size_t arg_size);

#endif // TREE_H_