#ifndef DUMP_H_
#define DUMP_H_

#include <stdio.h>

#include "dump.h"
#include "bintree.h"

static const char* const PATH   = "dumps/html/";
static const char* const GRAPH  = "graph_dump";
static const char* const DOT    = "dot";
static const char* const DOTXT  = ".dot";
static const char* const SPACE  = " ";
static const char* const OBJXT  = "-o";
static const char* const HTMLXT = ".html";
static const char* const PNGXT  = ".png";
static const char* const TPNG   = "-Tpng";

static const char* const EQU    = "y = ";
static const char* const DEQU   = "y' = ";

#define DUMP_ERR(result, error) if(dump_is_err(result, __FILE__, __LINE__) == MACRO_DUMP_ERR) return error

struct Dump_St
{
    const char* const GRAPHIC_DUMP = "dumps/graphic_dump.dot";
    const char* const HTML_DUMP    = "dumps/html/html_dump.html";
    const char* const TEX_DUMP     = "dumps/TeX/dump.tex";
    int filenum = 0;
};


enum Dump_Errors
{
    DUMP_IS_OKAY        = 0x00,
    DUMP_FILE_OPEN_ERR  = 0x01,
    DUMP_FILE_CLOSE_ERR = 0x02,
    DUMP_TO_PNG_ERR     = 0x03,

    MACRO_DUMP_GOOD = 0x20,
    MACRO_DUMP_ERR  = 0x30,
};


enum Colors
{
    ROOT_COLOR,
    BRANCH_COLOR,
    LEAF_COLOR,
};

Dump_Errors dump_is_err(Dump_Errors result, const char* name, const size_t line);

void graphic_dump(Node* root, Dump_St* General_Dump);

void put_node_to_dot_file(FILE* dotfile, Node* node);

void fill_dot_file(Node* root, FILE* dotfile);

Dump_Errors prepare_graphic_file(const Dump_St General_Dump);

void close_graphic_dump(const Dump_St General_Dump);

void fill_file_with_data(FILE* graph_dump_file, Node* node);

void fill_file_with_null(FILE* graph_dump_file, void* node);

void print_data_string(const Value_Type* value, const Data_Type data_type, FILE* dump_file);

void print_to_dump_file(const Node* node, FILE* dump_file, const Colors color);

void dot_to_png(const char* name, Dump_St* General_Dump);

void make_html_file(const char* filename);

void fill_file_html(const char* filename, const char* pngname);

void close_file_html(const char* filename);

void create_html(Dump_St General_Dump);

Dump_Errors create_png(Dump_St* General_Dump, Node* root);

const char* GetDataType(const Node* node);

FILE* PrepareTexDumpFile(const char* texfilename);

Tree_Err CloseTeX(FILE* texfile);

Tree_Err WriteTreeToTex(Node* root, FILE* texfile, const char*);

void WriteNodeToTeX(Node* node, FILE* texfile);

void OperToFile(Node* node, FILE* texfile);

void FuncToFile(Node* node, FILE* texfile);

#endif // DUMP_H_