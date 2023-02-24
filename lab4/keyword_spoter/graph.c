//
// Created by 20550 on 2022/12/5.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "list.h"
#include "queue.h"

#define CAPACITY 100000

#define MAX(x, y) ((x) > (y) ? (x) : (y))


/* plt是一个”邻居“链表，所有元素两两是邻居，将他们添加到各自的邻居tree中 */
static void add_neighbor(Graph * pg, const List * plt);
/* 根据稀疏邻接矩阵计算u到所有其他节点的最短距离，存入*dist */
static void dijkstra_heap(Graph * pg, uint32_t u);
static void bfs(Graph * pg, uint32_t u);
/* 计算PageRank */
static void compute_pagerank(Graph * pg, double alpha);
/* 获取pg中某一项统计数据前k个 */
static void get_kth_elem(Graph * pg, double * ps, uint32_t * ids, double * values, uint32_t k, bool less);


Graph * init_graph(uint8_t wsz)
{
    Graph * pg = (Graph *)malloc(sizeof(Graph));
    pg->capacity = CAPACITY;
    pg->size = 0;
    pg->edges = 0;

    pg->key_arr = (const char **)calloc(pg->capacity, sizeof (const char *) );
    pg->degree = (double *) calloc(pg->capacity, sizeof(double ));
    pg->closeness = (double *) calloc(pg->capacity, sizeof(double ));
    pg->pagerank = (double *) calloc(pg->capacity, sizeof(double ));
    pg->dist = (double *) malloc(pg->capacity * sizeof(double));
    pg->neighbor_arr = (Tree **)calloc(pg->capacity, sizeof(Tree *));
    for (uint32_t i = 0; i < pg->capacity; ++i)
        pg->neighbor_arr[i] = init_tree();


    pg->plt = init_list();
    pg->ph = init_heap(pg->capacity, true, false);

    pg->pa = init_automaton(16);
    pg->pmx = NULL; // 当确定pg->size，即关键字个数后，再初始化稀疏矩阵
    pg->window_size = wsz;

    return pg;
}

void insert_graph(Graph * pg, const char * str)
{
    if (pg->size == pg->capacity)
    {
        pg->capacity <<= 1;
        pg->key_arr = realloc(pg->key_arr, pg->capacity *sizeof(const char *));
        pg->degree = realloc(pg->degree, pg->capacity *sizeof(uint32_t));
        pg->closeness = realloc(pg->closeness, pg->capacity *sizeof(double ));
        pg->dist = realloc(pg->dist, pg->capacity *sizeof(double ));
        pg->pagerank = realloc(pg->pagerank, pg->capacity *sizeof(double ));
        pg->neighbor_arr = realloc(pg->neighbor_arr, pg->capacity *sizeof(Tree *));
        for (uint32_t i = pg->size; i < pg->capacity; ++i)
            pg->neighbor_arr[i] = init_tree();
    }
    // insert to automaton
    insert_atm(pg->pa, str, pg->size);
    pg->key_arr[pg->size++] = str;
}

void build_graph(Graph * pg)
{
    set_fail_pointer(pg->pa);
}


void search(Graph * pg, const char * str)
{
    reset_list(pg->plt);
//    list保存搜索到的每个关键字的id
   match(pg->pa, str, pg->plt);
    // 遍历链表 添加邻居关系
    add_neighbor(pg, pg->plt);
}


void calculate(Graph * pg)
{
    uint32_t max_degree = 0;
    uint32_t i;
    // 统计 <关键字id, degree>
    for (i = 0; i < pg->size; ++i)
    {
        if (pg->neighbor_arr[i]->size)
        {
            pg->degree[i] = pg->neighbor_arr[i]->size - 1;
            pg->edges += (uint32_t)pg->degree[i];
        }
        else
            pg->degree[i] = 0;
        max_degree = MAX(max_degree, pg->degree[i]);
    }
    pg->edges >>= 1;

    // 初始化稀疏矩阵
    pg->pmx = init_matrix(pg->size, max_degree + 1);

    // 根据每个关键字id对应的neighbor二叉树 添加矩阵元素
    for (i = 0; i < pg->size; ++i)
    {
        Tuple **ptr = pg->pmx->elem_matrix[i];
//        前序遍历邻居tree 替稀疏矩阵中关键字id为i的关键字对应的那一行填入元
        traverse(pg->neighbor_arr[i], &ptr);
        destroy_tree(pg->neighbor_arr[i]);
    }
    // 计算最短路径和中心性
    clock_t start, end;
    start = clock();
    for (i = 0; i < pg->size; ++i)
    {
        bfs(pg, i);
        fprintf(stderr, "%d closeness %llu\n", i, (uint64_t )pg->closeness[i]);
    }
    end = clock();

    printf("calculate shortest path time: %ld\n", end-start);
    // 计算PageRank
    start=clock();
    compute_pagerank(pg, 0.15);
    end=clock();
    printf("calculate pagerank time: %ld\n", end-start);
}



void print_result(Graph *pg, const char * file, uint8_t k)
{
    FILE * fp = fopen(file, "w");
    uint32_t * ids = (uint32_t *) malloc(k * sizeof(uint32_t));
    double * values = (double *) malloc(k * sizeof (double ));
    uint32_t i;

    //    度数前k个的关键字
    get_kth_elem(pg, pg->degree, ids, values, k, true);
    fprintf(fp, "**********************Top 20 keywords by degree************************\n");
    for (i = 0; i < k; ++i)
    {
        fprintf(fp, "%s %u\n", pg->key_arr[ids[i]], (uint32_t )values[i]);
    }

//    //    中心性前k个的关键字
//    get_kth_elem(pg, pg->closeness, ids, values, k, false);
//    fprintf(fp, "**********************Top 20 keywords by closeness*********************\n");
//    for (i = 0; i < k; ++i)
//    {
//        fprintf(fp, "%s 1 / %llu\n", pg->key_arr[ids[i]], (uint64_t)values[i]);
//    }

    //    pagerank 前k个的关键字
    get_kth_elem(pg, pg->pagerank, ids, values, k , true);
    fprintf(fp, "**********************Top 20 keywords by pagerank*********************\n");
    for (i = 0; i < k; ++i)
    {
        fprintf(fp, "%s %f\n", pg->key_arr[ids[i]], values[i]);
    }

    fclose(fp);
}


void destroy_graph(Graph *pg)
{
//    free all keywords
    for (uint32_t i = 0; i < pg->size; ++i)
        free((void*)pg->key_arr[i]);
    free(pg->key_arr);
    free(pg->degree);
    free(pg->closeness);
    free(pg->pagerank);
    free(pg->dist);
    free(pg->neighbor_arr);
    destroy_heap(pg->ph);
    destroy_list(pg->plt);

//    free automaton and matrix
    destroy_automaton(pg->pa);
    destroy_matrix(pg->pmx);

    free(pg);
}


static void add_neighbor(Graph * pg, const List * plt)
{
    if (plt->RESET)
        return;
    ListNode *cur = plt->head;

    while (cur)
    {
        ListNode *v = cur;
        while (v)
        {
//            cur 和v之间建立邻居关系
            insert_tree(pg->neighbor_arr[cur->target], v->target);
            if (v->target != cur->target)
                insert_tree(pg->neighbor_arr[v->target], cur->target);
            if (v == plt->tail)
                break;
            else
                v = v->next;
        }
        if (cur == plt->tail)
            break;
        else
            cur = cur->next;
    }
}


void compute_shortest_path(Graph * pg, uint32_t u)
{
    dijkstra_heap(pg, u);
}


static void dijkstra_heap(Graph * pg, uint32_t u)
{
    double * dist = pg->dist;
    uint32_t i;
// id比自己更小的最短距离已经计算出来了
    for (i = 0; i < pg->size; ++i)
        dist[i] = pg->size;
    Heap * dist_pq = pg->ph;
    insert_heap_data(dist_pq, u, 0);

//    dijkstra
    while (!empty_heap(dist_pq))
    {
        Tuple * tv = extract_top(dist_pq);
        uint32_t next = tv->first;
        double next_hop = tv->second;
        if (next_hop < dist[tv->first])
        {
            dist[next] = next_hop;
            uint32_t v;
            double val;
            for (i = 0; i < pg->degree[next] + 1; ++i)
            {
                get_elem(pg->pmx, next, i, &v, &val);
                if (v != next && v != u && dist[v] > next_hop + 1)
                    insert_heap_data(dist_pq, v, next_hop + 1);
            }
        }
    }

//    计算中心性
    for (i = 0; i < pg->size; ++i)
        pg->closeness[u] += dist[i];
//    pg->closeness[u] = 1 / pg->closeness[u];
}


static void bfs(Graph * pg, uint32_t u)
{
    double *dist = pg->dist;
    uint32_t i;
// id比自己更小的最短距离已经计算出来了
    for (i = 0; i < pg->size; ++i)
        dist[i] = pg->size;

//    初始化bfs队列

    Queue *queue = init_queue();
    push(queue, init_tuple(u, 0));


//    bfs
    while (!empty_queue(queue))
    {
        Tuple *tv = pop(queue);
        uint32_t next = tv->first;
        double next_hop = tv->second;
        destroy_tuple(tv);
        if (next_hop < dist[next])
        {
            dist[next] = next_hop;
            uint32_t v;
            double val;
            for (i = 0; i < pg->degree[next] + 1; ++i)
            {
                if (get_elem(pg->pmx, next, i, &v, &val))
                {
                    if (v != next && v != u && dist[v] > next_hop + 1)
                        push(queue, init_tuple(v, next_hop + 1));
                }
            }
        }
    }
//    计算中心性
    for (i = 0; i < pg->size; ++i)
        pg->closeness[u] += dist[i];
    destroy_queue(queue);
//    pg->closeness[u] = 1 / pg->closeness[u];
}


static void compute_pagerank(Graph *pg, double alpha)
{
    uint32_t v, i, j, k;
    double val;
    double epsilon = 0.0001;
    double beta = 1 - alpha;
    double delta = alpha / pg->size;
    double squared_sum;
    double *res = (double *) malloc(sizeof(double) * pg->size);
    double *now, *next;


//    初始化pagerank向量
    for (i = 0; i < pg->size; ++i)
        pg->pagerank[i] = pg->degree[i] / pg->size;
    // Power Iteration
    uint32_t times = 0;
    now = pg->pagerank;
    next = res;
    do
    {
        k = 0;
        squared_sum = 0;
        while (k < pg->size)
        {
            i = 0;
            j = 0;
            next[k] = 0;
            while (j < pg->degree[k] + 1 && get_elem(pg->pmx, k, j, &v, &val))
            {
                while (i <= v)
                {
                    next[k] += now[i] * delta;
                    ++i;
                }
                if (k != v && pg->degree[v])
                    next[k] += now[i] * beta / pg->degree[v];
                ++i;
                ++j;
            }
            squared_sum += pow(fabs(next[k] - now[k]), 2);
            ++k;
        }
        times++;
        fprintf(stderr, "Iteration times: %u bias: %f\n", times, sqrt(squared_sum));
//        swap now and next
        double *t = now;
        now = next;
        next = t;
    } while (sqrt(squared_sum) >= epsilon);
    pg->pagerank = now;
    free(next);
}

static void get_kth_elem(Graph * pg, double * ps, uint32_t * ids, double * values, uint32_t k, bool less)
{
    Heap *ph = pg->ph;
    set_fixed(ph, true);
    set_less(ph, less);
    heap_clear(ph);
    set_capacity(ph, k);
    uint32_t i;

    for (i = 0; i < pg->size; ++i)
        insert_heap_data(ph, i, ps[i]);


    Tuple *item;
    for (i = 0; i < k; ++i)
    {
        item = extract_top(ph);
        ids[k - 1 - i] = item->first;
        values[k - 1 - i] = item->second;
    }
}
