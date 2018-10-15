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
#include "smallsort.h"

#include "quickmerge.h"
namespace quickmerge_no_sampling_smaller_merge {
    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::sort_smaller_merge<true, quickmerge::Standard_sorter, partition::Stl_partition_no_sampling>(begin, end, less);
    }
    template<typename T>
    void sort(std::vector<T> &v) {
        typename std::vector<T>::iterator begin = v.begin();
        typename std::vector<T>::iterator end = v.end();
        //   quicksort::qsort_blocked(begin,end);
        quickmerge_no_sampling_smaller_merge::sort(begin, end, std::less<T>());
    }
}