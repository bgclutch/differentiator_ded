#include "../inc/exprtree.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../lib_buffer_proc/buffer.h"
#include "../lib_file_proc/file.h"

void CtorVariablesArray(Variable_Array_St* variable_array_st){
    variable_array_st->capacity = sizeof(short);
    variable_array_st->size = 0;
    variable_array_st->var_array = (Variable*)calloc(sizeof(Variable), variable_array_st->capacity);

    AddVariableToArray('x', variable_array_st);
    AddVariableToArray('y', variable_array_st);
}


void AddVariableToArray(const char new_variable, Variable_Array_St* variable_array_st){
    variable_array_st->var_array = VarArrayReallocation(variable_array_st);
    variable_array_st->var_array[variable_array_st->size].var     = new_variable;
    variable_array_st->var_array[variable_array_st->size].var_num = (int)variable_array_st->size;
    variable_array_st->size++;
}


void DtorVariablesArray(Variable_Array_St* variable_array_st){
    free(variable_array_st->var_array);
}


Variable* VarArrayReallocation(Variable_Array_St* variable_array_st){
    if (variable_array_st->capacity == variable_array_st->size){
        size_t new_capacity = variable_array_st->capacity * VAR_ARRAY_SCALE_COEF;
        Variable* new_var_array = (Variable*)realloc(variable_array_st->var_array, new_capacity);

        if (!new_var_array){
            fprintf(stderr, "nullptr was returned in VarArrayReallocation\n");
            return variable_array_st->var_array;
        }
        variable_array_st->capacity = new_capacity;
        return new_var_array;
    }

    return variable_array_st->var_array;
}

#if 0
Node* CreateNewDiffNode(char* buffer, size_t* buffer_ptr, Variable_Array_St* variable_array_st){
    Node* new_node = (Node*)calloc(sizeof(Node), sizeof(char));

    if (!new_node)
        return nullptr;

    char* arg_begin     = FindArgBegin(buffer, buffer_ptr);
    new_node->data_size = GetTokenDataSize(arg_begin);
    new_node->data_type = GetNodeType(arg_begin, new_node->data_size);
    new_node->value     = GetNodeValue(new_node->data_type, new_node->data_size, arg_begin, variable_array_st);

    *buffer_ptr += new_node->data_size;

    CreateNewNode(buffer, buffer_ptr, variable_array_st);
    CreateNewNode();
}
#endif


Data_Type GetNodeType(const char* arg_begin, const size_t arg_size){
    if (IsOperand(*arg_begin, arg_size)){
        return OPERAND;
    }
    else if (IsConst(arg_begin, arg_size)){
        return CONST;
    }
    else if (IsFunction(arg_begin, arg_size)){
        return FUNCTION;
    }
    else{
        return VARIABLE;
    }
}


size_t GetTokenDataSize(char* arg_begin){
    size_t size = 0;

    for(int i = 0; arg_begin[i] != ')'; i++)
        size++;

    return size;
}


int IsOperand(const char argument, const size_t arg_size){
    if (arg_size != 1)
        return 0;

    int result = 0;

    if (argument == ADD)
        result = 1;
    else if (argument == SUB)
        result = 1;
    else if (argument == MUL)
        result = 1;
    else if (argument == DIV)
        result = 1;
    else if (argument == POW)
        result = 1;

    return result;
}


int IsConst(const char* argument, const size_t arg_size){
    int result = 1;
    for(size_t i = 0; i < arg_size; i++)
    {
        if(!isalnum(argument[i]) && argument[i] != '.' && argument[i] != '+' && argument[i] != '-'){
            result = 0;
            break;
        }
    }

    return result;
}


int IsFunction(const char* argument, const size_t arg_size){
    int result = 0;

    if (strncmp(argument, SIN, arg_size) == 0)
        result = 1;
    else if (strncmp(argument, COS, arg_size) == 0)
        result = 1;
    else if (strncmp(argument, TAN, arg_size) == 0)
        result = 1;
    else if (strncmp(argument, COT, arg_size) == 0)
        result = 1;
    else if (strncmp(argument, LN, arg_size) == 0)
        result = 1;

    return result;

}


int IsVariableExists(const char var, const Variable_Array_St variable_array_st){
    for(size_t i = 0; i < variable_array_st.size; i++){
        if (var == variable_array_st.var_array[i].var)
            return 1;
    }

    return 0;
}


Value_Type GetNodeValue(const Data_Type data_type, const size_t data_size, const char* argument,
                        Variable_Array_St* variable_array_st){
    Value_Type value = {};
    switch (data_type){
        case VARIABLE:
            if (!IsVariableExists(*argument, *variable_array_st))
                AddVariableToArray(*argument, variable_array_st);

            memcpy(&value.varaible.var, argument, sizeof(char));

            if ((value.varaible.var_num = FindVarNum(*argument, *variable_array_st)) == -1)
                assert(0 && "InitNode variable ub\n");

            break;
        case CONST:
            memcpy(&value.number, argument, data_size);
            break;
        case OPERAND:
            value.arithmop.operand     = GetOperand(*argument);
            value.arithmop.operand_num = GetOperandNum(*argument);
            break;
        case FUNCTION:
            value.funciton.func     = GetAlgFunc(argument, data_size);
            value.funciton.func_num = GetAlgFuncNum(argument, data_size);
            break;
        case SYNTAXERROR:
            assert(0);
        default:
            assert(0);
    }

    return value;
}


int FindVarNum(const char var, const Variable_Array_St variable_array_st){
    for(size_t i = 0; i < variable_array_st.size; i++){
        if (var == variable_array_st.var_array[i].var)
            return variable_array_st.var_array[i].var_num;
    }

    return -1;
}


char GetOperand(const char argument){
    if (argument == ADD)
        return ADD;
    else if (argument == SUB)
        return SUB;
    else if (argument == MUL)
        return MUL;
    else if (argument == DIV)
        return DIV;
    else if (argument == POW)
        return POW;

    return 0; // error exit
}


Operand_Num GetOperandNum(const char argument){
    if (argument == ADD)
        return ADD_N;
    else if (argument == SUB)
        return SUB_N;
    else if (argument == MUL)
        return MUL_N;
    else if (argument == DIV)
        return DIV_N;
    else if (argument == POW)
        return POW_N;

    return SYNTERR_N; // error exit
}


const char* GetAlgFunc(const char* argument, const size_t arg_size){
    if (!strncmp(argument, SIN, arg_size)) // FIXME if there were array of structs there will be less copypaste???
        return SIN;
    else if (!strncmp(argument, COS, arg_size))
        return COS;
    else if (!strncmp(argument, TAN, arg_size))
        return TAN;
    else if (!strncmp(argument, COT, arg_size))
        return COT;
    else if (!strncmp(argument, LN, arg_size))
        return LN;
    else
        return nullptr; // error exit

}

Algebra_Func_Num GetAlgFuncNum(const char* argument, const size_t arg_size){
    if (!strncmp(argument, SIN, arg_size)) // FIXME if there were array of structs there will be less copypaste???
        return SIN_ALG;
    else if (!strncmp(argument, COS, arg_size))
        return COS_ALG;
    else if (!strncmp(argument, TAN, arg_size))
        return TAN_ALG;
    else if (!strncmp(argument, COT, arg_size))
        return COT_ALG;
    else if (!strncmp(argument, LN, arg_size))
        return LN_ALG;
    else
        return SYNTERR_ALG; // error exit
}