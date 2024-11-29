#ifndef TREERW_H_
#define TREERW_H_

#include "bintree.h"
#include "differ.h"


Differ_Err create_data_buffer(char** buffer, size_t* buffer_size);

Differ_Err write_tree_to_file(Node* root, const char* outpfilename);

void write_nodes_to_file(Node* node, FILE* file);

Differ_Err write_tree_to_file(Node* root, FILE* file);

Tree tree_ctor();

void tree_dtor(Tree* tree);

Differ_Err ReadTreeFromFileWithRecDescent(Tree* tree);

Node* InitNewNode(const Data_Type data_type, const Value_Type value, Node* left, Node* right);

Node* RecursiveDecent(const char*, size_t*);

Node* GetG(const char*, size_t*);

Node* GetE(const char*, size_t*);

Node* GetT(const char*, size_t*);

Node* GetP(const char*, size_t*);

Node* GetN(const char*, size_t*);

Node* GetPower(const char*, size_t*);

Node* GetFunction(const char*, size_t*);

void SyntaxError(const char* file, const size_t line);

#endif // TREERW_H_