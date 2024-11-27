#ifndef TREE_H_
#define TREE_H_

#include <math.h>

struct Variable{
    char    var;
    int var_num;
};

struct Variable_Array_St{
    size_t size;
    size_t capacity;
    Variable* var_array;
};

enum Operand_Num{ // maybe struct array too?
    SYNTERR_N = -1,
    ADD_N     =  1,
    SUB_N     =  2,
    MUL_N     =  3,
    DIV_N     =  4,
    POW_N     =  5,
};

struct Operand{
    char            operand;
    Operand_Num operand_num;
};

enum Algebra_Func_Num{ // maybe struct array too?
    SYNTERR_ALG = -1,
    SIN_ALG     =  1,
    COS_ALG     =  2,
    TAN_ALG     =  3,
    COT_ALG     =  4,
    LN_ALG      =  5,
};

struct Algebra_Func{
    const char*          func;
    Algebra_Func_Num func_num;
};

struct Value_Type{
    Operand      arithmop;
    Variable     varaible;
    Algebra_Func funciton;
    double         number;
};

enum Data_Type{
    SYNTAXERROR = 0,
    VARIABLE    = 1,
    CONST       = 2,
    OPERAND     = 3,
    FUNCTION    = 4,
};

struct Node{
    Node*        parent;
    Data_Type data_type;
    Value_Type    value;
    size_t    data_size;
    Node*          left;
    Node*         right;
};

struct Tree
{
    Node* root; // ctor elem && dtor elem
    char* buffer;
    // tree size
};

static const char ADD = '+'; // maybe struct array too?
static const char SUB = '-';
static const char MUL = '*';
static const char DIV = '/';
static const char POW = '^';

static const char* const SIN = "sin"; // maybe struct array too?
static const char* const COS = "cos";
static const char* const TAN = "tan";
static const char* const COT = "cot";
static const char* const LN  = "ln";

static const size_t VAR_ARRAY_SCALE_COEF = 2;


enum Tree_Err
{
    TREE_IS_OKAY_SH        = 0x00,
    DATA_FILE_OPEN_ERR     = 0x01,
    DATA_FILE_CLOSE_ERR    = 0x02,
    DATA_ACCESS_ERR        = 0x03,
    DATA_BUFFER_CREATE_ERR = 0x04,
};


Data_Type GetNodeType(const char* arg_begin, const size_t arg_size);

size_t GetTokenDataSize(char* arg_begin);

int IsOperand(const char argument, const size_t arg_size);

int IsConst(const char* argument, const size_t arg_size);

int IsFunction(const char* argument, const size_t arg_size);

int IsVariableExists(const char var, const Variable_Array_St variable_array_st);

Value_Type GetNodeValue(const Data_Type data_type, const size_t data_size, const char* argument,
                        Variable_Array_St* variable_array_st);

int FindVarNum(const char var, const Variable_Array_St variable_array_st);

char GetOperand(const char argument);

Operand_Num GetOperandNum(const char argument);

const char* GetAlgFunc(const char* argument, const size_t arg_size);

Algebra_Func_Num GetAlgFuncNum(const char* argument, const size_t arg_size);


void CtorVariablesArray(Variable_Array_St* variable_array_st);

void AddVariableToArray(const char new_variable, Variable_Array_St* variable_array_st);

void DtorVariablesArray(Variable_Array_St* variable_array_st);

Variable* VarArrayReallocation(Variable_Array_St* variable_array_st);

#endif // TREE_H_