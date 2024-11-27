#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/dump.h"
#include "../inc/bintree.h"

Dump_Errors prepare_graphic_file(const Dump_St General_Dump)
{
    FILE* graph_dump_file = fopen(General_Dump.GRAPHIC_DUMP, "w");

    if(graph_dump_file == nullptr)
        return DUMP_FILE_OPEN_ERR;

    fprintf(graph_dump_file,
            "digraph LIST{\n"
            "harset = \"UTF-8\";\n"
            "node [shape=plaintext];\n"
            "bgcolor = \"#0000aa\";\n"
            "fontcolor = black;\n"
            "fontsize = 18;\n"
            "style = \"italic\";\n"
            "margin = 1;\n"
            "ranksep = 1;\n"
            "nodesep = 3;\n"
            "bgcolor = \"#aedeb0\";\n");

    if(fclose(graph_dump_file))
        return DUMP_FILE_CLOSE_ERR;

    return DUMP_IS_OKAY;
}


void graphic_dump(Node* root, Dump_St* General_Dump)
{
    assert(root);
    assert(General_Dump);

    FILE* graph_dump_file = fopen(General_Dump->GRAPHIC_DUMP, "a");
    assert(graph_dump_file);

    fill_dot_file(root, graph_dump_file);

    assert(!fclose(graph_dump_file));
}


void fill_dot_file(Node* root, FILE* dotfile)
{

    put_node_to_dot_file(dotfile, root->left);
    put_node_to_dot_file(dotfile, root->right);

    fill_file_with_data(dotfile, root);

}


void put_node_to_dot_file(FILE* dotfile, Node* node)
{
    fill_file_with_data(dotfile, node);

    fprintf(dotfile, "\"%p\" -> \"%p\";\n", node->parent, node);

    if(!node->left && !node->right)
    {
        fprintf(dotfile, "\"%p\" -> \"%p\";\n", node, &node->left);
        fill_file_with_null(dotfile, &node->left);

        fprintf(dotfile, "\"%p\" -> \"%p\";\n", node, &node->right);
        fill_file_with_null(dotfile, &node->right);

        return;
    }

    put_node_to_dot_file(dotfile, node->left);
    put_node_to_dot_file(dotfile, node->right);

    return;
}


void close_graphic_dump(const Dump_St General_Dump)
{
    FILE* graph_dump_file = fopen(General_Dump.GRAPHIC_DUMP, "a");

    if(graph_dump_file == nullptr)
        assert(0);

    fprintf(graph_dump_file, "}");

    if(fclose(graph_dump_file))
        assert(0);
}


Dump_Errors dump_is_err(const Dump_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != DUMP_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_DUMP_ERR;
    }

    return MACRO_DUMP_GOOD;
}


void fill_file_with_data(FILE* graph_dump_file, Node* node)
{
    assert(graph_dump_file);
    assert(node);

    if(node->left == node->right)
    {
        print_to_dump_file(node, graph_dump_file, LEAF_COLOR);
    }
    else if(node->parent != nullptr && node->left != node->right)
    {
        print_to_dump_file(node, graph_dump_file, BRANCH_COLOR);
    }
    else if(node->parent == nullptr)
    {
        print_to_dump_file(node, graph_dump_file, ROOT_COLOR);
    }
}


void fill_file_with_null(FILE* graph_dump_file, void* node)
{
    assert(node);
    assert(graph_dump_file);

    fprintf(graph_dump_file, "\"%p\" [style = \"filled\", fillcolor = \"#ff9191\", label=<\n"
                             "<table border = \"0\" cellspacing=\"2\" cellpadding=\"4\">\n"
                             "<tr><td>NULL</td></tr></table>>];\n\n",
                             node);
}


void dot_to_png(const char* name, Dump_St* General_Dump)
{
    assert(name);
    assert(General_Dump);

    size_t pngname_size = strlen(PATH) + strlen(GRAPH) + sizeof(General_Dump->filenum) + strlen(PNGXT);

    char* pngname = (char*)calloc(pngname_size, sizeof(char));

    snprintf(pngname, pngname_size, "%s%s%d%s", PATH, GRAPH, General_Dump->filenum, PNGXT);

    size_t dtpng_size = strlen(DOT) + strlen(SPACE) * 4 + strlen(name) + strlen(TPNG) + pngname_size + sizeof(OBJXT);

    char* dot_to_png_command = (char*)calloc(dtpng_size, sizeof(char));

    snprintf(dot_to_png_command, dtpng_size, "%s%s%s%s%s%s%s%s%s", DOT, SPACE, name, SPACE, TPNG, SPACE, OBJXT,
             SPACE, pngname);

    system(dot_to_png_command);
    General_Dump->filenum++;

    free(pngname);
    free(dot_to_png_command);
}

void make_html_file(const char* filename)
{
    assert(filename);

    FILE* dump_file = fopen(filename, "w");

    fprintf(dump_file,
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n\n"
            "<head>\n"
            "\t<meta charset=\"utf-8\">\n"
            "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=\">\n"
            "\t<title>list dump</title>\n"
            "\t<link rel=\"stylesheet\" href=\"styles.css\">\n"
            "</head>\n\n"
            "<body>\n\n");

    fclose(dump_file);
}


void fill_file_html(const char* filename, const char* pngname)
{
    assert(filename);
    assert(pngname);

    FILE* dump_file = fopen(filename, "a+");
    fprintf(dump_file,
            "<img src=\"%s\"/>\n",
            pngname);

    fclose(dump_file);
}


void close_file_html(const char* filename)
{
    assert(filename);

    FILE* dump_file = fopen(filename, "a+");
    fprintf(dump_file,
            "</body>\n\n"
            "</html>\n");

    fclose(dump_file);
}


void print_data_string(const Value_Type* value, const Data_Type data_type, const size_t data_size, FILE* dump_file)
{
    assert(dump_file);

    switch (data_type)
    {
    case OPERAND:
        fprintf(dump_file, "%c", value->arithmop.operand);
        break;
    case CONST:
        fprintf(dump_file, "%lg", value->number);
        break;
    case VARIABLE:
        fprintf(dump_file , "%lg", value->number);
        break;
    case FUNCTION:
        fprintf(dump_file, "%c", value->varaible.var);
        break;
    case SYNTAXERROR:
        assert(0);
    default:
        assert(0);
    }
}
#

void print_to_dump_file(const Node* node, FILE* dump_file, const Colors color)
{
    assert(node);
    assert(dump_file);

    fprintf(dump_file, "\"%p\" [style = \"filled\", fillcolor = ", node);

    switch (color)
    {
        case ROOT_COLOR:
            fprintf(dump_file, "\"#ced7ea\"");
            break;

        case BRANCH_COLOR:
            fprintf(dump_file, "\"#b18b62\"");
            break;

        case LEAF_COLOR:
            fprintf(dump_file, "\"lightgreen\"");
            break;

        default:
        {
            assert(0);
        }
    }

    fprintf(dump_file, ",label=<\n"
                       "<table border=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n"
                       "<tr><td colspan=\"2\">Parent:  %p</td></tr>\n"
                       "<tr><td colspan=\"2\">Address: %p</td></tr>\n",
                       node->parent, node);

    fprintf(dump_file, "<tr><td colspan=\"2\">Data Type: %s</td></tr>\n"
                       "<tr><td colspan=\"2\">", GetDataType(node));

    print_data_string(&node->value, node->data_type, node->data_size, dump_file);

    fprintf(dump_file, "</td></tr>\n"
                       "<tr><td align = \"center\" >Left:%p</td><td align = \"center\" >Right:%p</td></tr></table>>];\n\n",
                       node->left, node->right);
}

Dump_Errors create_png(Dump_St* General_Dump, Node* root)
{
    assert(General_Dump);
    assert(root);

    DUMP_ERR(prepare_graphic_file(*General_Dump), DUMP_TO_PNG_ERR);
    graphic_dump(root, General_Dump);
    close_graphic_dump(*General_Dump);
    dot_to_png(General_Dump->GRAPHIC_DUMP, General_Dump);

    return DUMP_IS_OKAY;
}

void create_html(Dump_St General_Dump)
{
    make_html_file(General_Dump.HTML_DUMP);
    size_t pngname_size = strlen("graph_dump0.png ");
    char* pngname = (char*)calloc(sizeof(char), pngname_size);
    for(int i = 0; i < General_Dump.filenum; i++)
    {
        snprintf(pngname, pngname_size, "%s%d%s", "graph_dump", i, ".png");
        fill_file_html(General_Dump.HTML_DUMP, pngname);
    }
    free(pngname);
    close_file_html(General_Dump.HTML_DUMP);
}

#if 0
void print_akinator_instruction()
{
    fprintf(stderr,
            "welcome to the \"which matvey are you\" game!\n"
            "here you can get your matvey portrait from many different questions\n"
            "if you want " YELLOW_TEXT("to play") ", start the program with " GREEN_TEXT("\"./bintree --play\"\n")
            "if you need " BLUE_TEXT("definition of matvey's condition ") "input " GREEN_TEXT("\"./bintree --define\"\n")
            "if you want " BLUE_TEXT("to compare definitions ") "input " GREEN_TEXT("\"./bintree --compare\"\n")
            "and if you need " BLUE_TEXT("help ") "input " GREEN_TEXT("\"./bintree --h\"\n")
            "enjoy)\n"
            "ver 1.1\n");
}
#endif


const char* GetDataType(const Node* node){
    switch (node->data_type)
    {
        case VARIABLE:
            return "variable";
            break;
        case CONST:
            return "constant";
            break;
        case OPERAND:
            return "operand";
            break;
        case FUNCTION:
            return "function";
            break;
        case SYNTAXERROR:
            return "SYNTAXERROR";
            break;
        default:
            assert(0);
    }

    return "SYNTAXERROR";
}