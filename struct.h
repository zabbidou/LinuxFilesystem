#include <stdio.h>

#ifndef _HEADER_
#define _HEADER_

#define MAX_SIZE 37 // alphabet size

/* TYPEDEFS */

typedef int plm;

typedef struct trie_node {
	int is_final;
	int type;
	int file_level;
	struct trie_node * parent;
	struct trie_node * children[MAX_SIZE];
} TNode, *Trie;

/* FUNCTIONS */

int ceva();
Trie AllocTrie();
int mkdir(Trie a, char* name);
void print_shit(Trie a, int F);
void ls(Trie a, int is_root, int F, char* path, int level, int lvl_to_print, FILE* out);
Trie cd(Trie a, Trie current_dir, char* path, char* abs_path, int is_abs);
void parse_shit(char*string, char* object, char* path);
int is_path_valid(Trie a, char* path);

#endif