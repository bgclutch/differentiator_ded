#include "../inc/bintree.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/exprtree.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../inc/colorised_output.h"

void elem_ctor(void** elem, const size_t size)
{
    *elem = calloc(size, sizeof(char));

    assert(*elem);
}

void elem_dtor(void* elem)
{
    free(elem);
    elem = nullptr;
}


Tree_Errors tree_is_err(const Tree_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != NODE_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_ERR;
    }

    return MACRO_GOOD;
}


void tree_branch_dtor(Node* node, const char* data, const size_t len)
{
    assert(node);
    assert(data);

    if(node->left != nullptr)
    {
        tree_branch_dtor(node->left, data, len);
    }
    if(node->right != nullptr)
    {
        tree_branch_dtor(node->right, data, len);
    }

    free(node);

    return;
}