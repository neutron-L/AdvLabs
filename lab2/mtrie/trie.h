//
// Created by 20550 on 2022/10/11.
//

#ifndef MTRIE_TRIE_H
#define MTRIE_TRIE_H

#include <stdbool.h>
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// the struct of Trie Tree (non-compress)
typedef struct TrieNode
{
    struct TrieNode ** children;
    bool is_key;
}TrieNode;


typedef struct CTrieNode
{
    char * data;
    unsigned int len; // length of data`s bits
    struct CTrieNode ** children;
    bool is_key;
}CTrieNode;


// trie tree
typedef struct TrieTree
{
    union {
        TrieNode * troot;
        CTrieNode * ctroot;
    } root;
    unsigned int mask_len; // 2-tree is 1 4-tree is 2 16-tree is 4...
    unsigned int size;   // the total number of node
    bool is_compress;
} TrieTree;


/* initialize the trie tree
 * and indicate whether compression technology is required */
TrieTree * init_trie(unsigned int, bool);
/* search a specified string in the trie tree, and save the compared times */
bool search(TrieTree *, const char *, unsigned int *);
/* insert a specified string to a trie tree */
void insert(TrieTree *, char *);
/* destroy the trie tree */
void destroy(TrieTree *);

#endif //MTRIE_TRIE_H
