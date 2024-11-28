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

    size_t scope_difference = scope_checker(*buffer, *buffer_size);

    if(scope_difference != 0)
    {
        fprintf(stderr, "begin and end scopes difference: %lu\n", scope_difference);
        free(buffer);
        if(file_close(buffer_file))
            return AKINATOR_BUFFER_FILE_CLOSE_ERR;
    }

    if(file_close(buffer_file))
        return AKINATOR_BUFFER_FILE_CLOSE_ERR;


    return AKINATOR_STILL_ALIVE;
}


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


size_t scope_checker(const char* buffer, const size_t buffer_size)
{
    assert(buffer);

    size_t counter_scope_begin = 0;
    size_t counter_scope_end   = 0;

    for(size_t index_buffer = 0; index_buffer < buffer_size; index_buffer++)
    {
        if(buffer[index_buffer] == '(')
            counter_scope_begin++;

        if(buffer[index_buffer] == ')')
            counter_scope_end++;
    }

    return counter_scope_begin - counter_scope_end;
}



char* find_word_begin(char* buffer, const char* bufend)
{
    assert(buffer);
    assert(bufend);
    size_t index_buffer = 0;

    for(; IsSpaceOrScope(buffer[index_buffer]) && buffer + index_buffer < bufend; index_buffer++);
    return buffer + index_buffer;
}


void create_new_node(Node** node, char* buffer, size_t* all_bytes, Variable_Array_St* variable_array_st)
{
    assert(!*node);
    assert(buffer);
    assert(all_bytes);

    const char* bufend = buffer + strlen(buffer); // not optimal
    char* arg_begin = find_word_begin(buffer + *all_bytes, bufend);

    size_t elem_size = get_node_data_size(arg_begin);

    *all_bytes += (size_t)(arg_begin - (buffer + *all_bytes));
    *all_bytes += elem_size;

    node_init(node, elem_size, arg_begin, variable_array_st);

    if(*(buffer + *all_bytes) == ')')
    {
        return;
    }
    else
    {
        create_new_node(&(*node)->left, buffer, all_bytes, variable_array_st);
        (*node)->left->parent = *node;

        create_new_node(&(*node)->right, buffer, all_bytes, variable_array_st);
        (*node)->right->parent = *node;
    }
    return;
}


size_t get_node_data_size(const char* word)
{
    assert(word);

    size_t index = 0;

    for(; word[index] !='(' && word[index] != ')' && word[index] != '\0'; index++);


    return index;
}


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


#if 0
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


Tree tree_ctor(Variable_Array_St* variable_array_st)
{
    Tree tree = {};
    read_tree_from_file(&tree, variable_array_st);

    return tree;
}


void tree_dtor(Tree* tree, Variable_Array_St* variable_array_st)
{
    assert(tree);

    tree_branch_dtor(tree->root, tree->buffer, strlen(tree->buffer));
    DtorVariablesArray(variable_array_st);
    free(tree->buffer);
}


int IsSpaceOrScope(const char symbol){
    if (symbol == '(' || symbol == ')' || symbol == ' ')
        return 1;

    return 0;
}