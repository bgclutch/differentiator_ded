#ifndef DSL_H_
#define DSL_H_

#include "exprtree.h"
#include "bintree.h"
//---------------------------------GET VALUE CASES-------------------------------------------------------------------------
#define OPERVALUE(  index)                        GetValue(OPERAND,  index,        0,     0,     NAN)
#define VARVALUE(variable)                        GetValue(VARIABLE,     0, variable,     0,     NAN)
#define FUNCVALUE(  index)                        GetValue(FUNCTION,     0,        0, index,     NAN)
#define CONSTVALUE(number)                        GetValue(CONST,        0,        0,     0,  number)
//--------------------------------CREATE NEW NODE WITH GIVEN TYPE-----------------------------------------------------------------------
#define GETVAR(  variable)                        InitNewNode(VARIABLE,  VARVALUE( variable), nullptr, nullptr)
#define GETCONST(value)                           InitNewNode(CONST,     CONSTVALUE(  value), nullptr, nullptr)
#define GETOPER( oper_num, left, right)           InitNewNode(OPERAND,   OPERVALUE(oper_num),    left,   right)
#define GETFUNC( func_num, right)                 InitNewNode(FUNCTION,  FUNCVALUE(func_num), nullptr,   right)

#define DIFFOPERNODE(node, oper_num, left, right) ChangeNode(node, OPERAND,   OPERVALUE(oper_num), left, right)
//-----------------------------HELPERS FUNCTIONS FOR DIFF FUNCTIONS------------------------------------------------------------------
#define DIFFADD(node)                             DIFFOPERNODE(node, ADD_NUM, LDIFF(node), RDIFF(node))
#define DIFFSUB(node)                             DIFFOPERNODE(node, SUB_NUM, LDIFF(node), RDIFF(node))
#define DIFFMUL(node, cleft, cright)              DIFFOPERNODE(node, ADD_NUM, GETOPER(MUL_NUM, LDIFF(node), cright), GETOPER(MUL_NUM, cleft, RDIFF(node)))
#define DIFFDIV(node, cleft, cright)              DIFFOPERNODE(node, DIV_NUM, GETOPER(SUB_NUM, GETOPER(MUL_NUM, LDIFF(node), cright), GETOPER(MUL_NUM, cleft, RDIFF(node))), GETOPER(POW_NUM, COPY(cright), GETCONST(2)))
//------------------------------------DIFFERENTIAL MACROS--------------------------------------------------------------------------
#define DIFFPOWCONST(node, cleft, cright)         DIFFOPERNODE(node, MUL_NUM, GETOPER(MUL_NUM, node->right, GETOPER(POW_NUM, cleft, GETOPER(SUB_NUM, cright, GETCONST(1)))), LDIFF(node))
#define DIFFPOWVAR(  node, cleft, cright)         DIFFOPERNODE(node, MUL_NUM, GETOPER(POW_NUM, GETCONST(EULERSNUM), GETOPER(MUL_NUM, node->right, GETFUNC(LN_NUM, node->left))), DIFF(GETOPER(MUL_NUM, cright, GETFUNC(LN_NUM, cleft))))
//---------------------------------DIFFERENTIATE FUNCS MACROS------------------------------------------------------------------------------------------
#define DIFFSIN(node, cright)                     DIFFOPERNODE(node, MUL_NUM, GETFUNC(COS_NUM, cright), RDIFF(node))
#define DIFFCOS(node, cright)                     DIFFOPERNODE(node, MUL_NUM, GETOPER(SUB_NUM, GETCONST(0), GETFUNC(SIN_NUM, cright)), RDIFF(node))
#define DIFFTAN(node, cright)                     DIFFOPERNODE(node, MUL_NUM, GETOPER(DIV_NUM, GETCONST(1), GETOPER(POW_NUM, GETFUNC(COS_NUM, cright), GETCONST(2))), RDIFF(node))
#define DIFFLN( node, cright)                     DIFFOPERNODE(node, MUL_NUM, GETOPER(DIV_NUM, GETCONST(1), cright), RDIFF(node))

#endif