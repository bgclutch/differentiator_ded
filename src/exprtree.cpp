#include "../inc/exprtree.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../inc/colorised_output.h"
#include "../lib_buffer_proc/buffer.h"
#include "../lib_file_proc/file.h"


int IsOperator(const char argument){
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
    else if (argument == BSCOPE)
        result = 1;
    else if (argument == CSCOPE)
        result = 1;

    return result;
}


int IsConst(char argument){
    if(!isdigit(argument) && argument != '.')
        return 0;

    return 1;
}


int IsFunction(const char* argument){
    int result = 0;

    if (strncmp(argument, SIN, strlen(SIN)) == 0)
        result = 1;
    else if (strncmp(argument, COS, strlen(COS)) == 0)
        result = 1;
    else if (strncmp(argument, TAN, strlen(TAN)) == 0)
        result = 1;
    else if (strncmp(argument, LN, strlen(LN)) == 0)
        result = 1;

    return result;

}

char GetOperator(const char argument){
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


Operator_Num GetOperatorNum(const char argument){
    if (argument == ADD)
        return ADD_NUM;
    else if (argument == SUB)
        return SUB_NUM;
    else if (argument == MUL)
        return MUL_NUM;
    else if (argument == DIV)
        return DIV_NUM;
    else if (argument == POW)
        return POW_NUM;

    return SYNTERR_N; // error exit
}


const char* GetAlgFunc(const char* argument, const size_t arg_size){
    if (!strncmp(argument, SIN, arg_size))
        return SIN;
    else if (!strncmp(argument, COS, arg_size))
        return COS;
    else if (!strncmp(argument, TAN, arg_size))
        return TAN;
    else if (!strncmp(argument, LN, arg_size))
        return LN;
    else
        return nullptr; // error exit

}

Algebra_Func_Num GetAlgFuncNum(const char* argument, size_t* position){
    if (!strncmp(argument, SIN, strlen(SIN))){
        (*position) += strlen(SIN);
        return SIN_NUM;
    }
    else if (!strncmp(argument, COS, strlen(COS))){
        (*position) += strlen(COS);
        return COS_NUM;
    }
    else if (!strncmp(argument, TAN, strlen(TAN))){
        (*position) += strlen(TAN);
        return TAN_NUM;
    }
    else if (!strncmp(argument, LN, strlen(LN))){
        (*position) += strlen(LN);
        return LN_NUM;
    }
    else
        return SYNTERR_NUM; // error exit
}