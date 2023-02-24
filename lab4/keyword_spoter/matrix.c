//
// Created by 20550 on 2022/12/5.
//

#include <stdlib.h>
#include "matrix.h"


Matrix * init_matrix(uint32_t rows, uint32_t cols)
{
    Matrix *pmx = (Matrix *) malloc(sizeof(Matrix));
    pmx->rows = rows;
    pmx->max_cols = cols;

    pmx->elem_matrix = (Tuple ***) calloc(rows, sizeof(Tuple **));
    for (int i = 0; i < rows; ++i)
        pmx->elem_matrix[i] = (Tuple **) calloc(cols, sizeof(Tuple *));

    return pmx;
}


void insert_mtx(Matrix * pmx, uint32_t row_idx, uint32_t col_idx, Tuple * item)
{
    pmx->elem_matrix[row_idx][col_idx] = item;
}

bool get_elem(Matrix * pmx, uint32_t row_idx, uint32_t j, uint32_t * pc, double * pv)
{
    Tuple *item = pmx->elem_matrix[row_idx][j];
    if (item)
    {
        *pc = pmx->elem_matrix[row_idx][j]->first;
        *pv = pmx->elem_matrix[row_idx][j]->second;
        return true;
    }
    return false;
}

void destroy_matrix(Matrix * pmx)
{
    for (int i = 0; i < pmx->rows; ++i)
    {
        for (int j = 0; j < pmx->max_cols; ++j)
            if (pmx->elem_matrix[i][j])
            {
                destroy_tuple(pmx->elem_matrix[i][j]);
                pmx->elem_matrix[i][j] = NULL;
            }
        free(pmx->elem_matrix[i]);
        pmx->elem_matrix[i] = NULL;
    }
    free(pmx->elem_matrix);
    free(pmx);
}

