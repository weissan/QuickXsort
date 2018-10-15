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



#define BASE

#ifdef BASE
#include "smallsort.h"
#endif
#define QS_THRESH 0


#define WORST_CASE_RANGE_QMS 3



#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"
#include "mergeinsertion_special2.h++"


#include "quickmerge.h"





namespace quickmerge_mergeinsertion {
    template<typename iter, typename Compare>
    struct Mi_sorter {
        static void sort(iter begin, iter end, Compare less)
        {
 //           std::cout << "Mi_sorter" << std::endl;

            mergeinsertion_special2::sort(begin, end, less);
        }

    };

    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::base_thr = 20 * ilogb(end - begin);
        quickmerge::sort_smaller_merge<true, Mi_sorter, partition::Stl_partition>(begin, end, less);
        quickmerge::base_thr = 0;

    }
    template<typename T>
    void sort(std::vector<T> &v) {
        quickmerge_mergeinsertion::sort(v.begin(), v.end(), std::less<T>());
    }


}