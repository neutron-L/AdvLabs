#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trie.h"

#define BUFFER_SIZE 256
#define ANSWER 49425

int main() {
    /*
     read the dict.txt and string.txt
     write the searched string to result.txt
    */
    FILE *fr, *fw;
    TrieTree * trie;
    int m;
    unsigned int item_number = 0; // the total number of strings inserted
    char buffer[BUFFER_SIZE]; // the buffer of input

    printf("input the order m of trie tree: ");
    scanf("%s", buffer);
    char * p;
    m = (int)strtod(buffer, &p);

//    init bplusTree
    trie = init_trie(m, *p == 'c');

//    read dict.txt and build the b+tree
    fr = fopen("dict.txt", "r");
    while (fgets(buffer, BUFFER_SIZE, fr))
    {
        assert(buffer[strlen(buffer) - 1] == '\n');
        buffer[strlen(buffer) - 1] = '\0';
        insert(trie, buffer);
        ++item_number;
    }
    fclose(fr);

//    read the string.txt and write the result
    fr = fopen("string.txt", "r");
    fw = fopen("result.txt", "w");

    unsigned int hits = 0;
    unsigned int all_times = 0;
    unsigned int times;
    while (fgets(buffer, BUFFER_SIZE, fr))
    {
        assert(buffer[strlen(buffer) - 1] == '\n');
        buffer[strlen(buffer) - 1] = '\0';
        times=0;
        if (search(trie, buffer, &times))
        {
            fputs(buffer, fw);
            fputs("\n", fw);
            hits++;
            all_times += times;
        }
//        else
//            printf("%s\n", buffer);
    }
//    close file and release the b+tree
    fclose(fr);
    fclose(fw);

// print statistics
    printf("Expected hits: %d\n", ANSWER);
    printf("Actual hits: %d\n", hits);
    printf("Number of tree nodes: %d\n", trie->size);
    printf("Average lookup cost: %f\n", 1.0 * all_times / hits);
    printf("Average storage cost: %f\n", 1.0 * trie->size / item_number);

    destroy(trie);

    return 0;
}
