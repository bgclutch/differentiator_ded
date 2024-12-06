#include "../inc/treeRW.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/differ.h"
#include "../inc/exprtree.h"
#include "../inc/dsl.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"

Differ_Err create_data_buffer(char** buffer, size_t* buffer_size, const char* database)
{
    assert(!*buffer);
    assert(buffer_size);

    FILE* buffer_file = nullptr;

    if(file_read_open(&buffer_file, database) != ALL_GOOD_RET_F)
        return DIFFER_BUFFER_FILE_OPEN_ERR;

    *buffer_size = symbols_number(buffer_file);
    *buffer = buffer_create(*buffer_size, sizeof(char), buffer_file);

    if(!*buffer)
    {
        if(file_close(buffer_file))
            return DIFFER_BUFFER_FILE_CLOSE_ERR;

        return DIFFER_BUFFER_CTOR_ERR;
    }

    if(file_close(buffer_file))
        return DIFFER_BUFFER_FILE_CLOSE_ERR;


    return DIFFER_IS_OKAY;
}
#if 1
Node* GetLexicalAnalyzedBuf(const char* buffer, const size_t bufsize, size_t* analyzedbufsize) {
    assert(buffer);
    assert(analyzedbufsize);

    Node* analyzedbuffer = (Node*)calloc(sizeof(Node), bufsize);
    size_t position = 0;
    for(size_t i = 0; buffer[position] != '\0'; i++, analyzedbufsize++){
        analyzedbuffer[i] = GetAnalyzedNode(buffer, &position);
    }

    return analyzedbuffer;
}

Data_Type AnalyzeDataType(const char* symbol) {
    assert(symbol);

    Data_Type data_type = SYNTAXERROR;

    if (IsConst(*symbol)){
        data_type = CONST;
    }
    else if (IsFunction(symbol)){
        data_type = FUNCTION;
    }
    else if (IsOperator(*symbol)){
        data_type = OPERATOR;
    }
    else if (*symbol == 'x'){
        data_type = VARIABLE;
    }

    return data_type;
}

Value_Type AnalyzeValue(const Data_Type data_type, const char* symbol, size_t* position) {
    assert(symbol);
    assert(position);

    Value_Type value = {};
    switch (data_type)
    {
    case CONST:
        value = CONSTVALUE(GetNum(symbol, position));
        break;
    case VARIABLE:
        value = VARVALUE(*symbol);
        (*position)++;
        break;
    case FUNCTION:
        value = FUNCVALUE(GetAlgFuncNum(symbol, position));
        break;
    case OPERATOR:
        value = OPERVALUE(GetOperatorNum(*symbol));
        (*position)++;
        break;
    case SYNTAXERROR:
        assert(0); // FIXME ITS AWFUL
        break;
    default:
        break;
    }

    return value;
}

Node GetAnalyzedNode(const char* buffer, size_t* position) {
    assert(buffer);
    assert(position);
    Node node = {};
    node.data_type = AnalyzeDataType(buffer + *position);
    node.value     = AnalyzeValue(node.data_type, buffer, position);
    node.left      = nullptr;
    node.right     = nullptr;
    node.parent    = nullptr;

    return node;
}

double GetNum(const char* string, size_t* position) {
    assert(string);
    assert(position);

    double val   = 0;
    int counter     = 0;
    int flag_double = 0;
    size_t old_position = *position;
    while (IsConst(string[*position])){
        if (flag_double)
            counter++;
        if (string[*position] == '.')
            flag_double = 1;
        else
            val = val * 10 + (string[*position] - '0');

        (*position)++;
    }
    if (old_position == *position)
        SyntaxError(__FILE__, __LINE__, *position);
    val /= pow(10, counter);

    return val;
}
#endif
Differ_Err ReadTreeFromFileWithRecDescent(Tree* tree, const char* database) {
    assert(tree);
    assert(database);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, &buffer_size, database) != DIFFER_IS_OKAY)
    {
        free(buffer);
        return DIFFER_BUFFER_CTOR_ERR;
    }

    tree->buffer = buffer;
    size_t position = 0;

    // tree->analyzedbuf = GetLexicalAnalyzedBuf(tree->buffer, buffer_size, &tree->analyzedbufsize);

    tree->root = RecursiveDescent(tree->buffer, &position);

    return DIFFER_IS_OKAY;
}

Tree tree_ctor(const char* database)
{
    assert(database);

    Tree tree = {};
    tree.analyzedbuf = nullptr;
    ReadTreeFromFileWithRecDescent(&tree, database);

    return tree;
}

void tree_dtor(Tree* tree)
{
    assert(tree);

    tree_branch_dtor(tree->root);
    free(tree->buffer);
    free(tree->analyzedbuf);
}

Node* RecursiveDescent(const char* string, size_t* position){
    assert(string);
    assert(position);

    return GetRoot(string, position); // root
}

Node* GetRoot(const char* string, size_t* position) {
    assert(string);
    assert(position);

    Node* node = GetSumOrSub(string, position);
    if (string[*position] != '\0'){
        SyntaxError(__FILE__, __LINE__, *position);
    }
    (*position)++;
    return node;
}

Node* GetSumOrSub(const char* string, size_t* position){
    assert(string);
    assert(position);

    Node* node_left = GetMulOrDiv(string, position);
    while (string[*position] == ADD || string[*position] == SUB){
        int oper = string[*position];
        (*position)++;
        Node* node_right = GetMulOrDiv(string, position);
        node_left = GETOPER(GetOperatorNum((char)oper), node_left, node_right);
    }
    return node_left;
}

Node* GetMulOrDiv(const char* string, size_t* position){
    assert(string);
    assert(position);

    Node* node_left = GetFunction(string, position);
    while (string[*position] == MUL || string[*position] == DIV){
        int oper = string[*position];
        (*position)++;
        Node* node_right = GetFunction(string, position);
        node_left = GETOPER(GetOperatorNum((char)oper), node_left, node_right);
    }
    return node_left;
}

Node* GetScopeExpr(const char* string, size_t* position){
    assert(string);
    assert(position);

    if (string[*position] == '('){
        (*position)++;
        Node* node = GetSumOrSub(string, position);
        if (string[*position] != ')')
            SyntaxError(__FILE__, __LINE__, *position);
        (*position)++;
        return node;
    }
    else
        return GetConstOrVar(string, position);
}

Node* GetPower(const char* string, size_t* position){
    assert(string);
    assert(position);

    Node* node_left = GetScopeExpr(string, position);
    while (string[*position] == POW){
        (*position)++;
        Node* node_right = GetScopeExpr(string, position);
        node_left = GETOPER(POW_NUM, node_left, node_right);
    }
    return node_left;
}

Node* GetFunction(const char* string, size_t* position){
    assert(string);
    assert(position);

    if (strncmp(string + *position, SIN, strlen(SIN)) == 0){
        (*position) += strlen(SIN);
        return GETFUNC(SIN_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, COS, strlen(COS)) == 0){
        (*position) += strlen(COS);
        return GETFUNC(COS_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, TAN, strlen(TAN)) == 0){
        (*position) += strlen(TAN);
        return GETFUNC(TAN_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, LN, strlen(LN)) == 0){
        (*position) += strlen(LN);
        return GETFUNC(LN_NUM, GetPower(string, position));
    }
    else {
        return GetPower(string, position);
    }
}

Node* GetConstOrVar(const char* string, size_t* position){
    assert(string);
    assert(position);

    if (isalpha(string[*position])) {
        char variable = string[*position];
        (*position)++;
        if (isalpha(string[*position]))
            SyntaxError(__FILE__, __LINE__, *position);

        return GETVAR(variable);
    }
    double val          = 0;
    int counter         = 0;
    int flag_double     = 0;
    size_t old_position = *position;
    while (IsConst(string[*position])){
        if (flag_double)
            counter++;
        if (string[*position] == '.')
            flag_double = 1;
        else
            val = val * 10 + (string[*position] - '0');

        (*position)++;
    }
    if (old_position == *position)
        SyntaxError(__FILE__, __LINE__, *position);
    val /= pow(10, counter);
    return GETCONST(val);
}


void SyntaxError(const char* file, const size_t line, const size_t position){
    assert(file);

    fprintf(stderr, "%s:%lu\nincorrect symbol's position:%lu\n", file, line, position);
    assert(0);
    return;
}