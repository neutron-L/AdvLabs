#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "graph.h"

#define BUFFER_SIZE 8192
#define NUMBER 20
//#define WINDOW_SIZE

const char * content = "content.txt";
const char * keyword = "rank.txt";
const char *result = "result.txt";


char buffer[BUFFER_SIZE]; // the buffer of input


int main()
{
    clock_t start, end;
    clock_t build_time, calculate_time;
    /*
     read the dict.txt and string.txt
    */
    FILE *fr;

    unsigned int len;

    uint32_t wsz = 0;
#ifdef SLIDING_WINDOW
    printf("input the window size of graph: ");
    scanf("%s", buffer);
    wsz = (int) strtod(buffer, NULL);
#endif
//    init ac automaton
    Graph *pg = init_graph(wsz);

    printf("insert keyword...\n");
//    read keyword.txt and build the automaton
    fr = fopen(keyword, "r");
    while (fgets(buffer, BUFFER_SIZE, fr))
    {
        len = strlen(buffer);
        assert(buffer[len - 1] == '\n');
        buffer[len - 1] = '\0';
        char *word = strdup(buffer);
        insert_graph(pg, word);
    }
    build_graph(pg);
    fclose(fr);

    printf("search keyword...\n");
//    read the content.txt
    fr = fopen(content, "r");

    start=clock();
    // line number
    int line = 0;
    while (fgets(buffer, BUFFER_SIZE, fr))
    {
        len = strlen(buffer);
        assert(buffer[len - 1] == '\n');
        buffer[len - 1] = '\0';
        search(pg, buffer);
    }
//    close file and release the b+tree
    fclose(fr);

//    build matrix
    end = clock();

    build_time = end - start;

//    计算度、中心性、PageRank
    printf("start calculate...\n");
    start = clock();
    calculate(pg);
    end = clock();
    calculate_time = end - start;

// print statistics
    printf("print the result to file %s\n", result);
    print_result(pg, result, NUMBER);
    printf("nodes number: %u\nedges number: %u\n", pg->size, pg->edges);
    printf("Build graph time: %lu\n", build_time);
    printf("Calculate time: %lu\n", calculate_time);

    destroy_graph(pg);

    return 0;
}
