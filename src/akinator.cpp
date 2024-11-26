#include "../inc/akinator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/bintree.h"
#include "../inc/dump.h"
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

#if 0
void gamestart(Node* root)
{
    assert(root);

    Node* answer_node = akinator_func(root);
    assert(answer_node);

    fprintf(stderr, "\nam i right?");

    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        fprintf(stderr, "i hope you enjoy being " GREEN_TEXT("%.*s!\n"), (int)answer_node->data_size, answer_node->data);
    }
    else if(!strncmp(answer, NOANSWER, strlen(NOANSWER)))
    {
        change_recieved_leaf(&answer_node);
        fprintf(stderr, "thank you! now i know more about matvey\n");
    }
    else
    {
        assert(0);
    }

    free(answer);
    fprintf(stderr, "do you want to play again?\n");
    answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))  // copypaste(((
    {
        free(answer);
        gamestart(root);
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    return;
}

Akinator_Err comparestart(Node* root)
{
    assert(root);

    fprintf(stderr, "input your " BLUE_TEXT("first ") "word/sentence\n");
    char* first  = get_user_sentence();
    fprintf(stderr, "input your " BLUE_TEXT("second ") "word/sentence\n");
    char* second = get_user_sentence();

    if(!first || !second)
    {
        free(first);
        free(second);
        return CHOSEN_WORD_GETLINE_ERR;
    }

    Main_Stack_Struct first_stack  = {};
    Main_Stack_Struct second_stack = {};

    if(ctor_stack(&first_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

    if(ctor_stack(&second_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

    Find_Res retval_first  = NOTFOUND;
    Find_Res retval_second = NOTFOUND;

    if(find_define(root, first,  &retval_first,  &first_stack) == EXIT_FAILURE)
    {
        if(dtor_stack(&first_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

        if(dtor_stack(&second_stack) != ALL_IS_OK)
            return AKINATOR_IS_DEAD;

        free(first);
        free(second);

        return AKINATOR_IS_DEAD;
    }

    if(find_define(root, second,  &retval_second,  &second_stack) == EXIT_FAILURE)
    {
        if(dtor_stack(&first_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

        if(dtor_stack(&second_stack) != ALL_IS_OK)
            return AKINATOR_IS_DEAD;

        free(first);
        free(second);

        return AKINATOR_IS_DEAD;
    }

    if(retval_first == NOTFOUND || retval_second == NOTFOUND)
    {
        fprintf(stderr, "NOTHING TO COMPARE\n");
    }
    else
    {
        compare_definitions_print(&first_stack, &second_stack);
    }

    free(first);
    free(second);
    if(dtor_stack(&first_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

    if(dtor_stack(&second_stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

    fprintf(stderr, "do you want to play again?\n");
    char* answer = get_user_answer();


    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        if(comparestart(root) != AKINATOR_STILL_ALIVE)
            return AKINATOR_IS_DEAD;
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    return AKINATOR_STILL_ALIVE;
}



Akinator_Err getdefine(Node* root)
{
    assert(root);

    fprintf(stderr, BLUE_TEXT("input your word/sentence:\n"));
    char* chosen_sent = get_user_sentence();

    Main_Stack_Struct stack = {};
    if(ctor_stack(&stack))
        return CHOSEN_WORD_GETLINE_ERR;

    Find_Res retval = NOTFOUND;
    if(find_define(root, chosen_sent, &retval, &stack))
    {
        if(dtor_stack(&stack) != ALL_IS_OK)
            return AKINATOR_IS_DEAD;
        return CHOSEN_WORD_GETLINE_ERR;
    }

    if(retval == FOUND)
    {
        fprintf(stderr, "DEFINITION IS HERE\n");
        print_definition(&stack);
    }
    else
    {
        fprintf(stderr, "NO DEFINE TODAY\n");
    }

    fprintf(stderr, "do you want to play again?\n");
    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, sizeof(YESANSWER) - 1))
    {
        free(answer);
        if(getdefine(root))
            return AKINATOR_IS_DEAD;
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    free(chosen_sent);
    if(dtor_stack(&stack) != ALL_IS_OK)
        return AKINATOR_IS_DEAD;

    return AKINATOR_STILL_ALIVE;
}


int find_define(Node* node, const char* word, Find_Res* retval, Main_Stack_Struct* stack) // FIXME retval and push pop
{
    assert(node);
    assert(word);
    assert(retval);
    assert(stack);

    char* pop_elem = 0;

    if(stack_push(stack, node->data) != ALL_IS_OK)
        return EXIT_FAILURE;

    if(strncmp(node->data, word, node->data_size) == 0)
    {
        *retval = FOUND;

        if(stack_pop(stack, &pop_elem) != ALL_IS_OK)
            return EXIT_FAILURE;
    }

    if(!node->left && !node->right)
        return NOTFOUND;

    if(*retval == FOUND)
    {
        return FOUND;
    }
    else
    {
        if(stack_push(stack, NOTSTRING) != ALL_IS_OK)
        return EXIT_FAILURE;
        find_define(node->left, word, retval, stack);
    }
    if(*retval == NOTFOUND)
    {
        if(stack_pop(stack, &pop_elem) != ALL_IS_OK)
            return EXIT_FAILURE;

        if(stack_pop(stack, &pop_elem) != ALL_IS_OK)
            return EXIT_FAILURE;
    }

    if(*retval == FOUND)
    {
        return FOUND;
    }
    else
    {
        find_define(node->right, word, retval, stack);
    }

    if(*retval == NOTFOUND)
    {
        if(stack_pop(stack, &pop_elem) != ALL_IS_OK)
            return EXIT_FAILURE;
    }

    return NOTFOUND;
}

void print_definition(Main_Stack_Struct* stack)
{
    assert(stack);

    fprintf(stderr, "you are ");

    while(stack->size > 0)
    {
        char* elem = nullptr;
        stack_pop(stack, &elem);
        for(int i = 0; elem[i] != '}' && elem[i] != '{' && elem[i] != '\0'; i++)
            fprintf(stderr, BLUE_TEXT("%c"), elem[i]);

        if(strcmp(elem, NOTSTRING) == 0)
            fprintf(stderr, " ");
        else
            fprintf(stderr, "\nyou are ");

    }

    fprintf(stderr, "\r");
}
#endif

Node* akinator_func(Node* node)
{
    assert(node);

    fprintf(stderr, "you are %.*s", (int)node->data_size, node->data);

    if(!node->left && !node->right)
    {
        return node;
    }

    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        return akinator_func(node->right);
    }
    else if(!strncmp(answer, NOANSWER, strlen(NOANSWER)))
    {
        free(answer);
        return akinator_func(node->left);
    }

    assert(0);
    return nullptr;
}


char* get_user_answer()
{
    fprintf(stderr, "["GREEN_TEXT("yes")"/"RED_TEXT("no")"]\n");
    char* userinput = get_user_sentence();
    if(!userinput)
        return nullptr;


    for(int i = 0; strncmp(userinput, YESANSWER, strlen(YESANSWER)) &&
          strncmp(userinput, NOANSWER,  strlen(NOANSWER)) && i < 5; i++)
    {
        fprintf(stderr, "try again!\n");
        free(userinput);
        userinput = get_user_sentence();
        if(!userinput)
            return nullptr;
    }

    return userinput;
}


void clean_buffer()
{
    int bufsymb = 0;
    while((bufsymb = getchar()) != '\n');
}


char* get_user_sentence()
{
    char* sentence = nullptr;
    size_t size = 0;
    if(getline(&sentence, &size, stdin) == GETLINEERR)
    {
        free(sentence);
        return nullptr;
    }

    return sentence;
}

#if 0
void compare_definitions_print(Main_Stack_Struct* first, Main_Stack_Struct* second)
{
    assert(first);
    assert(second);

    char* first_elem  = nullptr;
    char* second_elem = nullptr;

    fprintf(stderr, BLUE_TEXT("color if similar\n") GREEN_TEXT("first")"\\"RED_TEXT("second\n"));

    while(first->size != 0 && second->size != 0)
    {
        int first_is_left  = 0;
        int second_is_left = 0;

        if(first->size)
            stack_pop(first, &first_elem);
        else
            first_elem = nullptr;

        if(second->size)
            stack_pop(second, &second_elem);
        else
            second_elem = nullptr;

        if(strcmp(first_elem, NOTSTRING) == 0 && first->size > 0)
        {
            first_is_left = 1;
            stack_pop(first, &first_elem);
        }

        if(strcmp(second_elem, NOTSTRING) == 0 && second->size > 0)
        {
            second_is_left = 1;
            stack_pop(second, &second_elem);
        }

        if(first->size != second->size)
        {
            if(first->size > second->size)
            {
                if(first_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(first_elem, GREENTEXT);
                fprintf(stderr,  "\\---");

                stack_push(second, second_elem);
            }
            else if(second->size > first->size)
            {
                fprintf(stderr,  "---\\");
                if(second_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(second_elem, REDTEXT);

                stack_push(first, first_elem);
            }
        }
        else
        {
            if(first_elem == second_elem && first_is_left == second_is_left)
            {
                if(first_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(first_elem, BLUETEXT);
                fprintf(stderr,  "\\");

                if(second_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(second_elem, BLUETEXT);
            }
            else
            {
                if(first_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(first_elem, GREENTEXT);
                fprintf(stderr,  "\\");

                if(second_is_left)
                    fprintf(stderr, MAGENTA_TEXT("%s "), NOTSTRING);

                stack_elem_outp(second_elem, REDTEXT);
            }
        }

        fprintf(stderr, "\n");
    }
}


void stack_elem_outp(const StackElem_t elem, const Text_Colors color)
{
    switch(color)
    {
        case REDTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, RED_TEXT("%c"), elem[i]);
            break;
        }
        case BLUETEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, BLUE_TEXT("%c"), elem[i]);
            break;
        }
        case GREENTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, GREEN_TEXT("%c"), elem[i]);
            break;
        }
        case YELLOWTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, YELLOW_TEXT("%c"), elem[i]);
            break;
        }
        case MAGENTATEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, MAGENTA_TEXT("%c"), elem[i]);
            break;
        }
        default:
        {
            assert(0);
            break;
        }

    }
}
#endif

int is_left_node(Node* node)
{
    return ((Node*)(node->parent))->left == node;
}
