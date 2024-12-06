#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/dump.h"
#include "../inc/bintree.h"
#include "../../lib_file_proc/file.h"

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
    if(root->left){
        put_node_to_dot_file(dotfile, root->left);
    }
    else{
        fill_file_with_null(dotfile, &root->left);
        fprintf(dotfile, "\"%p\" -> \"%p\";\n", root, &root->left);
    }
    if(root->right){
        put_node_to_dot_file(dotfile, root->right);
    }
    else{
        fill_file_with_null(dotfile, &root->right);
        fprintf(dotfile, "\"%p\" -> \"%p\";\n", root, &root->right);

    }

    fill_file_with_data(dotfile, root);

}


void put_node_to_dot_file(FILE* dotfile, Node* node)
{
    if (node)
        fill_file_with_data(dotfile, node);
    else
        return;

    fprintf(dotfile, "\"%p\" -> \"%p\";\n", node->parent, node);

    if(!node->left)
    {
        fprintf(dotfile, "\"%p\" -> \"%p\";\n", node, &node->left);
        fill_file_with_null(dotfile, &node->left);
    }
    if(!node->right)
    {
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

    if(!node->left && !node->right)
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
    else{
        assert(0);
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


void print_data_string(const Value_Type* value, const Data_Type data_type, FILE* dump_file)
{
    assert(dump_file);

    switch (data_type)
    {
    case OPERATOR:
        fprintf(dump_file, "%c", value->arithmop.oper);
        break;
    case CONST:
        fprintf(dump_file, "%lg", value->number);
        break;
    case VARIABLE:
        fprintf(dump_file, "%c", value->varaible);
        break;
    case FUNCTION:
        fprintf(dump_file, "%s", value->funciton.func);
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

    print_data_string(&node->value, node->data_type, dump_file);

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

// TODO codegeneration???
const char* GetDataType(const Node* node){
    switch (node->data_type)
    {
        case VARIABLE:
            return "variable";
            break;
        case CONST:
            return "constant";
            break;
        case OPERATOR:
            return "operator";
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

FILE* PrepareTexDumpFile(const char* texfilename) {
    assert(texfilename);
    FILE* texfile = nullptr;
    if(file_write_open(&texfile, texfilename) == FILE_W_OPEN_ERR)
        return nullptr;

    fprintf(texfile,
            "\\documentclass[a4paper]{article}\n"
            "\\usepackage[utf8]{inputenc}\n"
            "\\usepackage[T2A]{fontenc}\n"
            "\\usepackage[english,russian]{babel}\n"
            "\\usepackage[left=25mm, top=20mm, right=25mm, bottom=30mm, nohead, nofoot]{geometry}\n"
            "\\usepackage{amsmath,amsfonts,amssymb}\n"
            "\\usepackage{fancybox,fancyhdr}\n"
            "\\pagestyle{fancy}\n"
            "\\fancyhf{}\n"
            "\\fancyfoot[R]{\\thepage}\n"
            "\\setcounter{page}{2}\n"
            "\\headsep=10mm\n"
            "\\usepackage{xcolor}\n"
            "\\usepackage{hyperref}\n"
            "\\newcommand{\\lr}[1]{\\left({#1}\\right)}\n"
            "\\usepackage{enumitem}\n"
            "\\usepackage{graphicx}\n"
            "\\usepackage{float}\n"
            "\\usepackage{multicol}\n"
            "\\usepackage{asymptote}\n"
            "\\usepackage{comment}\n"
            "\\usepackage{tikz}\n"
            "\\usepackage{wrapfig}\n"
            "\\setlength{\\footskip}{12.0pt}\n"
            "\\setlength{\\headheight}{12.0pt}\n"
            "\\begin{document}\n");

    return texfile;
}

Tree_Err CloseTeX(FILE* texfile) {
    assert(texfile);

    fprintf(texfile, "\\end{document}\n");

    if(file_close(texfile))
        return WFILE_CLOSE_ERR;

    return TREE_IS_OKAY_SH;
}


Tree_Err WriteTreeToTex(Node* root, FILE* texfile, const char* string) {
    assert(root);

    fprintf(texfile, "\n\\begin{equation}\n"
                     "%s", string);

    WriteNodeToTeX(root, texfile);

    fprintf(texfile, "\n\\end{equation}\n");

    return TREE_IS_OKAY_SH;
}

void WriteNodeToTeX(Node* node, FILE* texfile) {
    assert(node);
    assert(texfile);

    switch (node->data_type){
    case OPERATOR:
        OperToFile(node, texfile);
        break;

    case CONST:
        fprintf(texfile, "%lg", node->value.number);
        break;

    case VARIABLE:
        fprintf(texfile, "x");
        break;
    case FUNCTION:
        FuncToFile(node, texfile);
        break;
    case SYNTAXERROR:
        assert(0);
    default:
        assert(0);
    }

    return;
}

void OperToFile(Node* node, FILE* texfile) {
    if (node->value.arithmop.operator_num== ADD_NUM)
    {
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, " + ");
        WriteNodeToTeX(node->right, texfile);
    }
    if (node->value.arithmop.operator_num == SUB_NUM)
    {
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, " - ");
        WriteNodeToTeX(node->right, texfile);
    }
    if (node->value.arithmop.operator_num == MUL_NUM)
    {
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, " \\cdot ");
        WriteNodeToTeX(node->right, texfile);
        #if 0
        if((node->right->data_type == OP) && *(int*)(node->right->data) < 3)
        {
            fprintf(texfile, "(");
            WriteNodeToTeX(node->right, texfile);
            fprintf(texfile, ")");
        }
        else
        {
        }
            #endif

    }
    if (node->value.arithmop.operator_num == DIV_NUM)
    {
        fprintf(texfile, "\\frac{");
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, "}{");
        WriteNodeToTeX(node->right, texfile);
        fprintf(texfile, "}");
    }
    if (node->value.arithmop.operator_num == POW)
    {
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, " ^{ ");
        WriteNodeToTeX(node->right, texfile);
        fprintf(texfile, " }");
    }
    return;
}

void FuncToFile(Node* node, FILE* texfile) {
    if (node->value.funciton.func_num == LN_NUM)
    {
        fprintf(texfile, "\\ln{");
        WriteNodeToTeX(node->right, texfile);
        fprintf(texfile, "}");
    }
    if(node->value.funciton.func_num == SIN_NUM)
    {
        fprintf(texfile, "\\sin(");
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, ")");
    }
    if(node->value.funciton.func_num == COS_NUM)
    {
        fprintf(texfile, "\\cos(");
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, ")");
    }
    if(node->value.funciton.func_num == TAN_NUM)
    {
        fprintf(texfile, "\\tan(");
        WriteNodeToTeX(node->left, texfile);
        fprintf(texfile, ")");
    }
    return;
}