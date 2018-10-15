#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <cmath>
#include <assert.h>

#define BASE
#define INS_SIZE 1

#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"


#include "quickmerge.h"
namespace quickmerge_mosqrt_small_base {
    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::sort_larger_merge<false, quickmerge::Standard_sorter, partition::Stl_partition_mosqrt>(begin, end, less);
    }
    template<typename T>
    void sort(std::vector<T> &v) {
        typename std::vector<T>::iterator begin = v.begin();
        typename std::vector<T>::iterator end = v.end();
        //   quicksort::qsort_blocked(begin,end);
        quickmerge_mosqrt_small_base::sort(begin, end, std::less<T>());
    }
}