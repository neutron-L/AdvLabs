#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bplustree.h"

#define BUFFER_SIZE 256
#define ANSWER 49425

int main() {
    /*
     read the dict.txt and string.txt
     write the searched string to result.txt
    */
    FILE *fr, *fw;
    BplusTree * bplusTree;
    int m;
    char buffer[BUFFER_SIZE]; // the buffer of input
    unsigned int item_number = 0; // The total number of strings inserted

    printf("input the order m of b+ tree: ");
    scanf("%s", buffer);
    m = (int)strtod(buffer, NULL);

//    init bplusTree
    bplusTree = init_bplus_tree(m, strcmp);

//    read dict.txt and build the b+tree
    fr = fopen("dict.txt", "r");
    while (fgets(buffer, BUFFER_SIZE, fr))
    {
        assert(buffer[strlen(buffer) - 1] == '\n');
        buffer[strlen(buffer) - 1] = '\0';
        char * data = strdup(buffer);
        insert(bplusTree, &data);
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
        char * data = strdup(buffer);
        times = 0;
        if (search(bplusTree, &data, &times))
        {
            fputs(buffer, fw);
            fputs("\n", fw);
            hits++;
            all_times += times;
        }
        free(data);
    }
//    close file and release the b+tree
    fclose(fr);
    fclose(fw);

// print statistics
    printf("Expected hits: %d\n", ANSWER);
    printf("Actual hits: %d\n", hits);
    printf("Number of tree nodes: %d\n", bplusTree->size);
    printf("Number of tree layers: %d\n", bplusTree->layers);
    printf("Average lookup cost: %f\n", 1.0 * all_times / hits);
    printf("Average storage cost: %f\n", 1.0 * bplusTree->size / item_number);

//destroy the b+ tree
    destroy(bplusTree);

    return 0;
}
