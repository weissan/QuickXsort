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



#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"
#include "binaryinsertionsort.h++"



#include "quickmerge.h"





namespace quickmerge_binaryinsertionsort_mosqrt {
    template<typename iter, typename Compare>
    struct Ins_sorter {
        static void sort(iter begin, iter end, Compare less)
        {
            //            std::cout << "Ins_sorter" << std::endl;
            binaryinsertionsort::sort(begin, end, less);
        }

    };



    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        quickmerge::base_thr = 20 * ilogb(end - begin);
        quickmerge::sort_larger_merge<true, Ins_sorter, partition::Stl_partition_mosqrt>(begin, end, less);
        quickmerge::base_thr = 0;

    }
    template<typename T>
    void sort(std::vector<T> &v) {
        quickmerge_binaryinsertionsort_mosqrt::sort(v.begin(), v.end(), std::less<T>());
    }


}