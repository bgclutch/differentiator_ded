#include "../inc/akinator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/dump.h"
#include "../inc/exprtree.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../../stack_ded/stack_headers/colorised_output.h"


Akinator_Err akinator_is_err(const Akinator_Err result, const char* name, const size_t line)
{
    assert(name);

    if(result != AKINATOR_STILL_ALIVE)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_AK_ERR;
    }

    return MACRO_AK_GOOD;
}

#if 0
Tree_Errors move_old_and_add_new_answer(Node* node)
{
    assert(node);

    NODE_ERR(node_init(&node->left, node->data, node->data_size), NODE_INIT_ERR);
    node->left->parent = node;                               // move old answer

    fprintf(stderr, "\nplease, input your expected object(w/o 'you are' ):\n");

    char* new_answer_data = get_user_sentence();
    if(!new_answer_data)
        return NEWNODE_CRE_ERR;

    size_t new_answer_size = strlen(new_answer_data) - 1;

    NODE_ERR(node_init(&node->right, new_answer_data, new_answer_size), NODE_INIT_ERR);
    node->right->parent = node;

    return NODE_IS_OKAY;
}


Tree_Errors change_recieved_leaf(Node** node)
{
    assert(node);
    assert(*node);

    if((*node)->left || (*node)->right)
        return NODE_IS_BRANCH;

    move_old_and_add_new_answer(*node);

    fprintf(stderr, "please, input difference between wrong(old) and right(new) answers(w/o '?' and 'you are'):\n");

    char* new_branch_data = get_user_sentence();
    if(!new_branch_data)
        return NEWNODE_CRE_ERR;
    size_t new_branch_size = strlen(new_branch_data) - 1;
    NODE_ERR(edit_node(node, new_branch_data, new_branch_size), DENY_TO_EDIT);

    return NODE_IS_OKAY;
}


Tree_Errors edit_node(Node** node, const NodeElem_t new_data, const size_t new_data_size)
{
    assert(*node);
    assert((*node)->left);
    assert((*node)->right);

    if((*node)->data != (*node)->left->data)
        return DENY_TO_EDIT;

    if(!(*node)->left || !(*node)->right)
        return DENY_TO_EDIT;

    (*node)->data = new_data;
    (*node)->data_size = new_data_size;

    return NODE_IS_OKAY;
}
#endif
