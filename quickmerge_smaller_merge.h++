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
#include <string>


#define WORST_CASE_RANGE_QMS 3



#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"

#include "quickmerge.h"



namespace quickmerge_smaller_merge {
    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::sort_smaller_merge(begin, end, less);
    }
    template<typename T>
    void sort(std::vector<T> &v) {
        quickmerge::sort_smaller_merge(v.begin(), v.end(), std::less<T>());
    }


}