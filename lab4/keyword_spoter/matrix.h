//
// Created by 20550 on 2022/12/5.
//

#ifndef KEYWORD_SPOTER_MATRIX_H
#define KEYWORD_SPOTER_MATRIX_H


#include <stdint.h>
#include <stdbool.h>
#include "tuple.h"

typedef struct
{
    Tuple *** elem_matrix;
    uint32_t rows;
    uint32_t max_cols;
} Matrix;


/* 操作：初始化一个稀疏矩阵 */
/* 前提条件：稀疏矩阵的行数 rows和列数cols */
/* 后置条件：返回指向一个已初始化的matrix的指针 */
Matrix * init_matrix(uint32_t rows, uint32_t cols);


/* 操作：设置matrix中指定的坐标为指定的值 */
/* 前提条件：pmx指向已经被初始化的matrix，row、col和val均为非负数 */
/* 后置条件：成功设置相应位置的值 */
void insert_mtx(Matrix * pmx, uint32_t row_idx, uint32_t col_idx, Tuple * item);




bool get_elem(Matrix * pmx, uint32_t row_idx, uint32_t j, uint32_t * pc, double * pv);

/* 操作：销毁pmx指向的稀疏矩阵 */
/* 前提条件：pmx指向已经被初始化的matrix */
/* 后置条件：matrix被成功销毁无内存泄露 */
void destroy_matrix(Matrix * pmx);

#endif //KEYWORD_SPOTER_MATRIX_H
