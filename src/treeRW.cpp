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


Differ_Err ReadTreeFromFileWithRecDescent(Tree* tree, const char* database) {
    assert(tree);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, &buffer_size, database) != DIFFER_IS_OKAY)
    {
        free(buffer);
        return DIFFER_BUFFER_CTOR_ERR;
    }

    tree->buffer = buffer;
    size_t position = 0;

    tree->root = RecursiveDescent(tree->buffer, &position);

    return DIFFER_IS_OKAY;
}

Tree tree_ctor(const char* database)
{
    Tree tree = {};
    ReadTreeFromFileWithRecDescent(&tree, database);

    return tree;
}

void tree_dtor(Tree* tree)
{
    assert(tree);

    tree_branch_dtor(tree->root);
    free(tree->buffer);
}

Node* RecursiveDescent(const char* string, size_t* position){
    return GetG(string, position); // root
}

Node* GetG(const char* string, size_t* position) {
    Node* node = GetE(string, position);
    if (string[*position] != '\0'){
        SyntaxError(__FILE__, __LINE__, *position);
    }
    (*position)++;
    return node;
}

Node* GetE(const char* string, size_t* position){
    Node* node_left = GetT(string, position);
    while (string[*position] == ADD || string[*position] == SUB){
        int oper = string[*position];
        (*position)++;
        Node* node_right = GetT(string, position);
        node_left = OPER(GetOperandNum((char)oper), node_left, node_right);
    }
    return node_left;
}

Node* GetT(const char* string, size_t* position){
    Node* node_left = GetFunction(string, position);
    while (string[*position] == MUL || string[*position] == DIV){
        int oper = string[*position];
        (*position)++;
        Node* node_right = GetFunction(string, position);
        node_left = OPER(GetOperandNum((char)oper), node_left, node_right);
    }
    return node_left;
}

Node* GetP(const char* string, size_t* position){
    if (string[*position] == '('){
        (*position)++;
        Node* node = GetE(string, position);
        if (string[*position] != ')')
            SyntaxError(__FILE__, __LINE__, *position);
        (*position)++;
        return node;
    }
    else
        return GetN(string, position);
}

Node* GetPower(const char* string, size_t* position){
    Node* node_left = GetP(string, position);
    while (string[*position] == POW){
        (*position)++;
        Node* node_right = GetP(string, position);
        node_left = OPER(POW_NUM, node_left, node_right);
    }
    return node_left;
}

Node* GetFunction(const char* string, size_t* position){
    if (strncmp(string + *position, SIN, strlen(SIN)) == 0){
        (*position) += strlen(SIN);
        return FUNC(SIN_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, COS, strlen(COS)) == 0){
        (*position) += strlen(COS);
        return FUNC(COS_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, TAN, strlen(TAN)) == 0){
        (*position) += strlen(TAN);
        return FUNC(TAN_NUM, GetPower(string, position));
    }
    else if (strncmp(string + *position, LN, strlen(LN)) == 0){
        (*position) += strlen(LN);
        return FUNC(LN_NUM, GetPower(string, position));
    }
    else {
        return GetPower(string, position);
    }
}

Node* GetN(const char* string, size_t* position){
    if (isalpha(string[*position])) {
        char variable = string[*position];
        (*position)++;
        if (isalpha(string[*position]))
            SyntaxError(__FILE__, __LINE__, *position);

        return VAR(variable);
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
    return CONST(val);
}


void SyntaxError(const char* file, const size_t line, const size_t position){
    fprintf(stderr, "%s:%lu\nposition:%lu\n", file, line, position);
    assert(0);
    return;
}