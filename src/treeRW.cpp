#include "../inc/treeRW.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/akinator.h"
#include "../inc/exprtree.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"


Akinator_Err create_data_buffer(char** buffer, size_t* buffer_size)
{
    assert(!*buffer);
    assert(buffer_size);

    FILE* buffer_file = nullptr;

    if(file_read_open(&buffer_file, DATABASE) != ALL_GOOD_RET_F)
        return AKINATOR_BUFFER_FILE_OPEN_ERR;

    *buffer_size = symbols_number(buffer_file);
    *buffer = buffer_create(*buffer_size, sizeof(char), buffer_file);

    if(!*buffer)
    {
        if(file_close(buffer_file))
            return AKINATOR_BUFFER_FILE_CLOSE_ERR;

        return AKINATOR_BUFFER_CTOR_ERR;
    }

    if(file_close(buffer_file))
        return AKINATOR_BUFFER_FILE_CLOSE_ERR;


    return AKINATOR_STILL_ALIVE;
}


Akinator_Err ReadTreeFromFileWithRecDescent(Tree* tree) {
    assert(tree);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, &buffer_size) != AKINATOR_STILL_ALIVE)
    {
        free(buffer);
        return AKINATOR_BUFFER_CTOR_ERR;
    }

    tree->buffer = buffer;
    size_t position = 0;

    tree->root = RecursiveDecent(tree->buffer, &position);

    return AKINATOR_STILL_ALIVE;
}

#if 0
Akinator_Err read_tree_from_file(Tree* tree, Variable_Array_St* variable_array_st)
{
    assert(tree);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, &buffer_size) != AKINATOR_STILL_ALIVE)
    {
        free(buffer);
        return AKINATOR_BUFFER_CTOR_ERR;
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

    if(init_tree_nodes(root_addr, buffer, &all_bytes, variable_array_st) != AKINATOR_STILL_ALIVE)
    {
        free(buffer);
        return AKINATOR_IS_DEAD;
    }

    return AKINATOR_STILL_ALIVE;
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
#endif


Node* InitNewNode(const Data_Type data_type, const Value_Type value, Node* left, Node* right) { // NOTE value given as a struct!
    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    switch (data_type) {
    case VARIABLE:
        new_node->data_type = VARIABLE;
        break;

    case CONST:
        new_node->data_type = CONST;
        break;

    case OPERAND:
        new_node->data_type = OPERAND;
        break;

    case FUNCTION:
        new_node->data_type = FUNCTION;
        break;

    case SYNTAXERROR: // not good
        assert(0);

    default:
        assert(0);
    }

    new_node->value = value;
    new_node->left  = left;
    new_node->right = right;

    return new_node;
}


#if 0
Akinator_Err init_tree_nodes(Node* node, char* buffer, size_t* all_bytes, Variable_Array_St* variable_array_st)
{
    assert(node);
    assert(buffer);
    assert(all_bytes);

    create_new_node(&node->left, buffer, all_bytes, variable_array_st);
    node->left->parent = node;

    create_new_node(&node->right, buffer, all_bytes, variable_array_st);
    node->right->parent = node;

    return AKINATOR_STILL_ALIVE;
}


Akinator_Err write_tree_to_file(Node* root, const char* outputfilename)
{
    assert(root);
    assert(outputfilename);

    FILE* write_tree_file = nullptr;

    if(file_write_open(&write_tree_file, outputfilename) == FILE_W_OPEN_ERR)
        return AKINATOR_FILE_ERROR;

    write_tree_to_file(root, write_tree_file);
    file_close(write_tree_file);

    return AKINATOR_STILL_ALIVE;
}


Akinator_Err write_tree_to_file(Node* root, FILE* file)
{
    assert(root);
    assert(file);

    fprintf(file, "{");

    for(size_t i = 0; i < root->data_size; i++)
        fprintf(file, "%c", root->data[i]);

    write_nodes_to_file(root->left,  file);
    write_nodes_to_file(root->right, file);

    fprintf(file, "}");
    return AKINATOR_STILL_ALIVE;
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


Tree tree_ctor()
{
    Tree tree = {};
    ReadTreeFromFileWithRecDescent(&tree);

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
        fprintf(stderr, "GetG err: %s\n", string + (*position));
        SyntaxError(__FILE__, __LINE__);
    }
    (*position)++;
    return node;
}

Node* GetE(const char* string, size_t* position){
    Node* node_left = GetT(string, position);
    while (string[*position] == '+' || string[*position] == '-'){
        int oper = string[*position];
        (*position)++;
        fprintf(stderr, "GetE: %c\n", string[*position]);
        Node* node_right = GetT(string, position);
        Value_Type value = {};
        value.arithmop.operand     = GetOperand((char)oper);
        value.arithmop.operand_num = GetOperandNum((char)oper);
        return InitNewNode(OPERAND, value, node_left, node_right);
    }

    return node_left;
}

Node* GetT(const char* string, size_t* position){
    Node* node_left = GetFunction(string, position);
    while (string[*position] == '*' || string[*position] == '/'){
        int oper = string[*position];
        (*position)++;
        Node* node_right = GetFunction(string, position);
        Value_Type value = {};
        value.arithmop.operand     = GetOperand((char)oper);
        value.arithmop.operand_num = GetOperandNum((char)oper);
        return InitNewNode(OPERAND, value, node_left, node_right);
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
    if (string[*position] == '^'){
        (*position)++;
        Node* node_right = GetP(string, position);
        Value_Type value = {};
        value.arithmop.operand     = POW;
        value.arithmop.operand_num = POW_N;
        return InitNewNode(OPERAND, value, node_left, node_right);
    }

    return node_left;
}

Node* GetFunction(const char* string, size_t* position){
    Value_Type value = {};
    if (strncmp(string + *position, "sin", strlen("sin")) == 0){
        position += (int)(strlen("sin"));
        value.funciton.func = SIN;
        value.funciton.func_num = SIN_ALG;
        return InitNewNode(FUNCTION, value, nullptr, GetPower(string, position));
    }
    else if (strncmp(string + *position, "cos", strlen("cos")) == 0){
        position += (int)(strlen("cos"));
        value.funciton.func = COS;
        value.funciton.func_num = COS_ALG;
        return InitNewNode(FUNCTION, value, nullptr, GetPower(string, position));
    }
    else if (strncmp(string + *position, "tan", strlen("tan")) == 0){
        position += (int)(strlen("tan"));
        value.funciton.func = TAN;
        value.funciton.func_num = TAN_ALG;
        return InitNewNode(FUNCTION, value, nullptr, GetPower(string, position));
    }
    else if (strncmp(string + *position, "ln", strlen("ln")) == 0){
        position += (int)(strlen("ln"));
        value.funciton.func = LN;
        value.funciton.func_num = LN_ALG;
        return InitNewNode(FUNCTION, value, nullptr, GetPower(string, position));
    }
    else {
        return GetPower(string, position);
    }
}

Node* GetN(const char* string, size_t* position){
    Value_Type value = {};
    fprintf(stderr, "GetN input: %s\n", string + *position);
    if (isalpha(string[*position])) {
        value.varaible = string[*position];
        (*position)++;
        fprintf(stderr, "!zalupa\n");
        if (isalpha(string[*position]))
            SyntaxError(__FILE__, __LINE__);

        return InitNewNode(VARIABLE, value, nullptr, nullptr);
    }
    double val       = 0;
    int counter      = 0;
    int flag_double  = 0;
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

    value.number = val;
    return InitNewNode(CONST, value, nullptr, nullptr);
}


void SyntaxError(const char* file, const size_t line){
    fprintf(stderr, "%s:%lu\n", file, line);
    assert(0);
    return;
}