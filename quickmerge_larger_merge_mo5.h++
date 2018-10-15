#pragma once
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


#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"

#include "quickmerge.h"
namespace quickmerge_larger_merge_mo5 {
    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::sort_larger_merge<false, quickmerge::Standard_sorter, partition::Stl_partition_mo5>(begin, end, less);
    }
    template<typename T>
    void sort(std::vector<T> &v) {
        typename std::vector<T>::iterator begin = v.begin();
        typename std::vector<T>::iterator end = v.end();
        //   quicksort::qsort_blocked(begin,end);
        quickmerge_larger_merge_mo5::sort(begin, end, std::less<T>());
    }
}