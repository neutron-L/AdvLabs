#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "ac.h"

#define BUFFER_SIZE 5120
#define ANSWER  92454005

const char * content = "content.txt";
const char * keyword = "keyword.txt";

char buffer[BUFFER_SIZE]; // the buffer of input


int main() {
    clock_t start, mid, end;
    clock_t build_time, match_time;
    /*
     read the dict.txt and string.txt
    */
    FILE *fr;

    unsigned int len;

    uint32_t order;
    printf("input the order of ac automaton: ");
    scanf("%s", buffer);
    order = (int) strtod(buffer, NULL);

    start = clock();
//    init ac automaton
    Automaton *pa = init_automaton(order);

//    read keyword.txt and build the automaton
    fr = fopen(keyword, "r");
    while (fgets(buffer, BUFFER_SIZE, fr)) {
        len = strlen(buffer);
        assert(buffer[len - 1] == '\n');
        buffer[len - 1] = '\0';
        insert(pa, buffer);
    }
    fclose(fr);

//    set the fail pointer
    set_fail_pointer(pa);

    mid = clock();
    build_time = (mid - start);


//    read the content.txt
    fr = fopen(content, "r");

    uint64_t tot_match_times = 0;
    uint64_t tot_goto_times = 0;
    uint32_t goto_times;
    // line number
    uint32_t lines=0;
    while (fgets(buffer, BUFFER_SIZE, fr)) {
        len = strlen(buffer);
        assert(buffer[len - 1] == '\n');
        buffer[len - 1] = '\0';
        tot_match_times += match(pa, buffer, &goto_times);
        tot_goto_times += goto_times;
        ++lines;
    }
//    close file and release the b+tree
    fclose(fr);
    end = clock();
    match_time = end - mid;


// print statistics
    printf("Expect match times: %d\n", ANSWER);
    printf("Actual match times: %llu\n", tot_match_times);
    printf("Number of tree nodes: %d\n", pa->size);
    printf("Total number of state transitions: %llu\n", tot_goto_times);
    printf("Time spent in constructing automaton: %ld\n", build_time);
    printf("Time to match all strings: %ld\n", match_time);
    printf("lines: %u\n", lines);
    destroy_automaton(pa);

    return 0;
}
