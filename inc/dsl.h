#ifndef DSL_H_
#define DSL_H_

#include "exprtree.h"
#include "bintree.h"
//---------------------------------GET VALUE CASES-------------------------------------------------------------------------
#define OPERVALUE(  index)                         GetValue(OPERAND,  index,        0,     0,     NAN)
#define VARVALUE(variable)                         GetValue(VARIABLE,     0, variable,     0,     NAN)
#define FUNCVALUE(  index)                         GetValue(FUNCTION,     0,        0, index,     NAN)
#define CONSTVALUE(number)                         GetValue(CONST,        0,        0,     0,  number)
//-------------------------------ARITHMETIC OPERATIONS----------------------------------------------------------------------------
#define ADD(left, right)                           OPER(ADD_NUM, left, right)
#define SUB(left, right)                           OPER(SUB_NUM, left, right)
#define MUL(left, right)                           OPER(MUL_NUM, left, right)
#define DIV(left, right)                           OPER(DIV_NUM, left, right)
#define POW(left, right)                           OPER(POW_NUM, left, right)
//---------------------------------FUNCTIONS MACROS---------------------------------------------------------------------------------------
#define SIN(right)                                 FUNC(SIN_NUM, right)
#define COS(right)                                 FUNC(COS_NUM, right)
#define TAN(right)                                 FUNC(TAN_NUM, right)
#define LN( right)                                 FUNC( LN_NUM, right)
//--------------------------------CREATE NEW NODE WITH GIVEN TYPE-----------------------------------------------------------------------
#define VAR(  variable)                            InitNewNode(VARIABLE,  VARVALUE( variable), nullptr, nullptr)
#define CONST(value)                               InitNewNode(CONST,     CONSTVALUE(  value), nullptr, nullptr)
#define OPER( oper_num, left, right)               InitNewNode(OPERAND,   OPERVALUE(oper_num),    left,   right)
#define FUNC( func_num, right)                     InitNewNode(FUNCTION,  FUNCVALUE(func_num), nullptr,   right)
//---------------------------------DIFFERENTIATE NODES BY CASES-------------------------------------------------------
#define DIFFVARNODE(  node, variable)              ChangeNode(node, VARIABLE,  VARVALUE( variable), nullptr, nullptr)
#define DIFFCONSTNODE(node, value)                 ChangeNode(node, CONST,     CONSTVALUE(  value), nullptr, nullptr)
#define DIFFFUNCNODE( node, func_num, right)       ChangeNode(node, FUNCTION,  FUNCVALUE(func_num), nullptr,   right)
#define DIFFOPERNODE( node, oper_num, left, right) ChangeNode(node, OPERAND,   OPERVALUE(oper_num),    left,   right)
//-----------------------------HELPERS FUNCTIONS FOR DIFF FUNCTIONS-------------------------------------------------------
#define DIFFADDNODE(node, left, right)             DIFFOPERNODE(node, ADD_NUM, left, right) // REVIEW излишне
#define DIFFSUBNODE(node, left, right)             DIFFOPERNODE(node, SUB_NUM, left, right)
#define DIFFMULNODE(node, left, right)             DIFFOPERNODE(node, MUL_NUM, left, right)
#define DIFFDIVNODE(node, left, right)             DIFFOPERNODE(node, DIV_NUM, left, right)
//---------------------------------------SIMPLE CASES-----------------------------------------------------------------------------
#define DIFFADD(node)                              DIFFADDNODE(node, LDIFF(node), RDIFF(node))
#define DIFFSUB(node)                              DIFFSUBNODE(node, LDIFF(node), RDIFF(node))
#define DIFFMUL(node, cleft, cright)               DIFFADDNODE(node, MUL(LDIFF(node), cright), MUL(cleft, RDIFF(node)))
#define DIFFDIV(node, cleft, cright)               DIFFDIVNODE(node, SUB(MUL(LDIFF(node), cright), MUL(cleft, RDIFF(node))), POW(COPY(cright), CONST(2)))
//------------------------------------DIFFERENTIAL MACROS--------------------------------------------------------------------------
#define DIFFPOWCONST(node, cleft, cright)       DIFFMULNODE(node, MUL(node->right, POW(cleft, SUB(cright, CONST(1)))), LDIFF(node))
#define DIFFPOWVAR(  node, cleft, cright)       DIFFMULNODE(node, POW(CONST(EULERSNUM), MUL(node->right, LN(node->left))), DIFF(MUL(cright, LN(cleft))))
//---------------------------------DIFFERENTIATE FUNCS MACROS------------------------------------------------------------------------------------------
#define DIFFSIN(node, cright)                      DIFFMULNODE(node, COS(cright), RDIFF(node))
#define DIFFCOS(node, cright)                      DIFFMULNODE(node, SUB(CONST(0), SIN(cright)), RDIFF(node))
#define DIFFTAN(node, cright)                      DIFFMULNODE(node, DIV(CONST(1), POW(COS(cright), CONST(2))), RDIFF(node))
#define DIFFLN( node, cright)                      DIFFMULNODE(node, DIV(CONST(1), cright), RDIFF(node))

#endif // DSL_H_