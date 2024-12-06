#ifndef ALGFUNCS_H_
#define ALGFUNCS_H_

static const char* const SIN = "sin";
static const char* const COS = "cos";
static const char* const TAN = "tan";
static const char* const LN  = "ln";

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

const Algebra_Func func_array[] = {{SIN, SIN_NUM},
                                   {COS, COS_NUM},
                                   {TAN, TAN_NUM},
                                   {LN, LN_NUM}};

#endif // ALGFUNCS_H_