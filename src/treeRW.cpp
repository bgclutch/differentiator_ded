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

    tree->root = RecursiveDecent(tree->buffer, &position);

    return DIFFER_IS_OKAY;
}

#if 0
Differ_Err read_tree_from_file(Tree* tree, Variable_Array_St* variable_array_st)
{
    assert(tree);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, &buffer_size) != DIFFER_IS_OKAY)
    {
        free(buffer);
        return DIFFER_BUFFER_CTOR_ERR;
    }

    tree->buffer = buffer;

    const char* bufend = buffer + buffer_size;
    char* root_question = find_word_begin(buffer, bufend);

    size_t all_bytes = 0;

    size_t root_question_size = get_node_data_size(root_question);

    if(node_init(&tree->root, root_question_size, root_question, variable_array_st) != NODE_IS_OKAY)
        assert(0);

    all_bytes += root_question_size;

    Node* root_addr = tree->root;

    root_addr->left   = nullptr;
    root_addr->right  = nullptr;
    root_addr->parent = nullptr;

    if(init_tree_nodes(root_addr, buffer, &all_bytes, variable_array_st) != DIFFER_IS_OKAY)
    {
        free(buffer);
        return DIFFER_IS_DEAD;
    }

    return DIFFER_IS_OKAY;
}


void create_new_node(Node** node, char* buffer, size_t* all_bytes, Variable_Array_St* variable_array_st)
{
    assert(!*node);
    assert(buffer);
    assert(all_bytes);

    else
    {
        create_new_node(&(*node)->left, buffer, all_bytes, variable_array_st);
        (*node)->left->parent = *node;

        create_new_node(&(*node)->right, buffer, all_bytes, variable_array_st);
        (*node)->right->parent = *node;
    }
    return;
}


Differ_Err init_tree_nodes(Node* node, char* buffer, size_t* all_bytes, Variable_Array_St* variable_array_st)
{
    assert(node);
    assert(buffer);
    assert(all_bytes);

    create_new_node(&node->left, buffer, all_bytes, variable_array_st);
    node->left->parent = node;

    create_new_node(&node->right, buffer, all_bytes, variable_array_st);
    node->right->parent = node;

    return DIFFER_IS_OKAY;
}


Differ_Err write_tree_to_file(Node* root, const char* outputfilename)
{
    assert(root);
    assert(outputfilename);

    FILE* write_tree_file = nullptr;

    if(file_write_open(&write_tree_file, outputfilename) == FILE_W_OPEN_ERR)
        return DIFFER_FILE_ERROR;

    write_tree_to_file(root, write_tree_file);
    file_close(write_tree_file);

    return DIFFER_IS_OKAY;
}


Differ_Err write_tree_to_file(Node* root, FILE* file)
{
    assert(root);
    assert(file);

    fprintf(file, "{");

    for(size_t i = 0; i < root->data_size; i++)
        fprintf(file, "%c", root->data[i]);

    write_nodes_to_file(root->left,  file);
    write_nodes_to_file(root->right, file);

    fprintf(file, "}");
    return DIFFER_IS_OKAY;
}

void write_nodes_to_file(Node* node, FILE* file)
{
    assert(node);
    assert(file);

    fprintf(file, "{");

    for(size_t i = 0; i < node->data_size; i++)
        fprintf(file, "%c", node->data[i]);

    if(!node->left && !node->right)
    {
        fprintf(file, "}");

        return;
    }
    else
    {
        write_nodes_to_file(node->left,  file);
        write_nodes_to_file(node->right, file);
    }

    fprintf(file, "}");
    return;
}
#endif


Tree tree_ctor(const char* database)
{
    Tree tree = {};
    ReadTreeFromFileWithRecDescent(&tree, database);

    return tree;
}


void tree_dtor(Tree* tree)
{
    assert(tree);

    tree_branch_dtor(tree->root, tree->buffer, strlen(tree->buffer));
    free(tree->buffer);
}


Node* RecursiveDecent(const char* string, size_t* position){
    return GetG(string, position); // root
}

Node* GetG(const char* string, size_t* position) {
    Node* node = GetE(string, position);
    if (string[*position] != '\0'){
        SyntaxError(__FILE__, __LINE__);
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
            SyntaxError(__FILE__, __LINE__);
        (*position)++;
        return node;
    }
    else
        return GetN(string, position);
}

Node* GetPower(const char* string, size_t* position){
    Node* node_left = GetP(string, position);
    if (string[*position] == POW){
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
            SyntaxError(__FILE__, __LINE__);

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
        SyntaxError(__FILE__, __LINE__);
    val /= pow(10, counter);
    return CONST(val);
}


void SyntaxError(const char* file, const size_t line){
    fprintf(stderr, "%s:%lu\n", file, line);
    assert(0);
    return;
}