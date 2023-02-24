//
// Created by 20550 on 2022/12/11.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tuple.h"


Tuple * init_tuple(uint32_t first, double second)
{
    Tuple  * pt = (Tuple*)malloc(sizeof(Tuple));
    pt->first = first;
    pt->second = second;

    return pt;
}

void destroy_tuple(Tuple *pt)
{
    free(pt);
}