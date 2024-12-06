#ifndef TREERW_H_
#define TREERW_H_

#include "bintree.h"
#include "differ.h"

Differ_Err create_data_buffer(char** buffer, size_t* buffer_size, const char* database);

Differ_Err write_tree_to_file(Node* root, const char* outpfilename);

void write_nodes_to_file(Node* node, FILE* file);

Differ_Err write_tree_to_file(Node* root, FILE* file);

Tree tree_ctor(const char* database);

void tree_dtor(Tree* tree);

Node* GetLexicalAnalyzedBuf(const char* buffer, const size_t bufsize, size_t* analyzedbufsize);

Node GetAnalyzedNode(const char* buffer, size_t* position);

Data_Type AnalyzeDataType(const char* symbol);

Value_Type AnalyzeValue(const Data_Type data_type, const char* symbol, size_t* position);

double GetNum(const char* string, size_t* position);

Differ_Err ReadTreeFromFileWithRecDescent(Tree* tree, const char* database);

Node* RecursiveDescent(const char*, size_t*);

Node* GetRoot(const char*, size_t*);

Node* GetSumOrSub(const char*, size_t*);

Node* GetMulOrDiv(const char*, size_t*);

Node* GetScopeExpr(const char*, size_t*);

Node* GetConstOrVar(const char*, size_t*);

Node* GetPower(const char*, size_t*);

Node* GetFunction(const char*, size_t*);

void SyntaxError(const char* file, const size_t line, const size_t position);

#endif // TREERW_H_