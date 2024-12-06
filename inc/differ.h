#ifndef DIFFER_H_
#define DIFFER_H_

#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"
#include "bintree.h"
#include "dump.h"

#define AKN_ERR(result, error) if(akinator_is_err(result, __FILE__, __LINE__) == MACRO_AK_ERR) return error

#define COPY( node) CopyNode(node)
#define LCOPY(node) CopyNode(node->left)
#define RCOPY(node) CopyNode(node->right)
#define DIFF( node) Differentiation(node)
#define LDIFF(node) Differentiation(node->left)
#define RDIFF(node) Differentiation(node->right)

#define ISVARINBRANCH(  node, result) IsSmthInBranch(node, VARIABLE, result)
#define ISCONSTINBRANCH(node, result) IsSmthInBranch(node, CONST, result)
#define ISFUNCINBRANCH( node, result) IsSmthInBranch(node, FUNCTION, result)
#define ISOPERINBRANCH( node, result) IsSmthInBranch(node, OPERAND, result)

const double EULERSNUM = 2.7183;
const double PI        = 3.1415;
const double EPSILON   = 0.0001;


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

Differ_Err differ_is_err(const Differ_Err result, const char* name, const size_t line);

Node* ChangeNode(Node* node, const Data_Type data_type, const Value_Type value, Node* left, Node* right);

void IsSmthInBranch(Node* node, Data_Type data_type, int result);

Node* Differentiation(Node* node);

double GetOperResult(Node* node);

int IsZero(const double num);

Node* AddDiff(Node* node);

Node* SubDiff(Node* node);

Node* MulDiff(Node* node);

Node* DivDiff(Node* node);

Node* PowDiff(Node* node);

Node* SinDiff(Node* node);

Node* CosDiff(Node* node) ;

Node* TanDiff(Node* node) ;

Node* LogDiff(Node* node) ;

Node* CopyNode(Node* node);

Node* DiffLeaf(Node* node);

#endif // DIFFER_H_
