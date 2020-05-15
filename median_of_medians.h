/******************************************************************************
* median.h
*
* Different pivot selection strategies
*
******************************************************************************
* Copyright (C) 2016 Stefan Edelkamp <edelkamp@tzi.de>
* Copyright (C) 2016 Armin Weiß <armin.weiss@fmi.uni-stuttgart.de>
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
//#define SHOW_COMPARISONS

#pragma once


#ifndef UNDERSAMPLING_SIZE
#define UNDERSAMPLING_SIZE 66
#endif // !UNDERSAMPLING_SIZE



#include "median.h"

#ifndef MEDIAN_OF_MEDIAN
#define MEDIAN_OF_MEDIAN 


namespace partition {
    template<typename iter, typename Compare>
    struct Stl_partition_mosqrt;/* {

                                // for adaptive selection
                                static inline iter partition(iter begin, iter end, float ratio, Compare less);
                                };*/

}
namespace median {


    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition(iter begin, iter end, Compare less);

    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition(iter begin, iter end, float ratio, Compare less);

    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition_ninther(iter begin, iter end, float ratio, Compare less);


    template<template<class, class> class Partitioner, typename iter, typename Compare>
    void kth_element(iter begin, iter kth, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;

        //       std::cout << "start kth: " << kth - begin << " n: " << end - begin << std::endl;
        while (begin + 30 < end) {

            iter pivot;
            index n = end - begin;
            double ratio = (double)(kth - begin + 1) / (double)(n + 1);
            bool right_side = (ratio > 0.5);
            if (right_side)
                ratio = 1 - ratio;

#ifdef SQRT_HYBRID



            //        std::cout << "k: " << kth - begin << " n: " << n << std::endl;
            if (n > 120 && ratio <= 0.4*sqrt(n) / n) //more or less arbitrary threshold
            {
                //            std::cout << "ratio: " << ratio << " n: " << n << std::endl;
                ratio += 0.000000001;
                ratio *= (sqrt(n) + 1);
                if (right_side)
                    ratio = 1 - ratio;
                pivot = partition::Stl_partition_mosqrt<iter, Compare>::partition(begin, end, ratio, less);

                /*                  if (!right_side) {
                if (pivot < kth)
                {
                std::cout << "too small pivot: " << pivot - begin << " k: " << kth - begin << std::endl;
                std::cout << "ratio: " << ratio << " ratio*n: " << ratio*sqrt(n) << std::endl;
                }
                }
                else
                if (pivot > kth)
                {
                std::cout << "too large pivot: " << pivot - begin << " k: " << kth - begin << std::endl;
                std::cout << "ratio: " << ratio << " ratio*n: " << ratio*sqrt(n) << std::endl;
                }
                */
            }
            else
#endif // SQRT_HYBRID
            {
                if (ratio < 0.2222222222222222222)
                {
                    ratio = ratio * 2.25; // 1.6666666666666666; //for median_of_medians_partition
                }
                else
                {
                    ratio = 0.5;
                }
                if (right_side)
                    ratio = 1 - ratio;
                pivot = median_of_medians_partition_ninther<Partitioner>(begin, end, ratio, less);
            }


            if (pivot == kth) {
                return;
            }
            else if (pivot < kth)
            {
                begin = pivot + 1;
            }
            else //(kth < pivot)
            {
                end = pivot;
            }

        }


        std::nth_element(begin, kth, end, less);

#ifdef WORST_CASE_SIMULATION
        if (worst_case_simulation) {
            std::partial_sort(begin, end, end, less);
        }
#endif // WORST_CASE_SIMULATION

    }



    template<template<class, class> class Partitioner, typename iter, typename Compare>
    void kth_element_hybrid(iter begin, iter kth, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        bool next_linear_sample = false;

        while (begin + 50 < end) {
            iter pivot;
            index n = end - begin;
            float ratio = (float)(kth - begin + 1) / (float)(n + 1);
            if (next_linear_sample)
            {
                bool right_side = (ratio > 0.5);
                if (right_side)
                    ratio = 1 - ratio;

                if (n > 120 && ratio <= 0.4*sqrt(n) / n)
                {
                    ratio *= sqrt(n);
                    if (right_side)
                        ratio = 1 - ratio;
                    pivot = partition::Stl_partition_mosqrt<iter, Compare>::partition(begin, end, ratio, less);
                }
                else {
                    if (ratio < 0.2222222222222222222)
                    {
                        ratio = ratio * 2.25; // 1.6666666666666666;
                    }
                    else
                    {
                        ratio = 0.5;
                    }
                    if (right_side)
                        ratio = 1 - ratio;
                    pivot = median_of_medians_partition_ninther<Partitioner>(begin, end, ratio, less);
                }
            }
            else
                pivot = partition::Stl_partition_mosqrt<iter, Compare>::partition(begin, end, ratio, less);

            //worst case stopper
            next_linear_sample = false;
            if (end - begin > WORST_CASE_RANGE *  (pivot - begin) || end - begin > WORST_CASE_RANGE *  (end - pivot))
                next_linear_sample = true;
            if (pivot == kth) {
                return;
            }
            else if (pivot < kth)
            {
                begin = pivot + 1;
            }
            else //(kth < pivot)
            {
                end = pivot;
            }

        }
        std::nth_element(begin, kth, end, less);

#ifdef WORST_CASE_SIMULATION
        if (worst_case_simulation) {
            std::partial_sort(begin, end, end, less);
        }
#endif // WORST_CASE_SIMULATION




    }



    template<template<class, class> class Partitioner, typename iter, typename Compare>
    inline iter median(iter begin, iter end, Compare less) {
        kth_element<Partitioner>(begin, begin + ((end - begin) / 2), end, less);
        return begin + ((end - begin) / 2);
    }




    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition(iter begin, iter end, Compare less) {
        if (end - begin < 45)
        {
            std::cout << "<45 shouldnt be here" << std::endl;
            if (end - begin == 2) {
                sort_pair(begin, begin + 1, less);
                return begin;
            }
            else if (end - begin > 2)
                return Partitioner<iter, Compare>::partition(begin, end, median_of_3(begin, end, less), less);
        }
        //	std::cout << "worstcase" << std::endl;
        iter searchit = begin;
        iter placeit = begin;
        while (searchit + 4 < end) {
            iter tempit = median_of_5(searchit, less);
            std::iter_swap(tempit, placeit);
            placeit++;
            searchit += 5;
        }

        //iter placeit = begin + 10;
        iter pivot = begin + (placeit - begin) / 2;
        kth_element<Partitioner>(begin, pivot, placeit, less);
        //std::iter_swap(placeit - 1, end - 1);
        std::swap_ranges(pivot + 1, placeit, end - (placeit - pivot - 1));
        return Partitioner<iter, Compare>::partition(pivot, end - (placeit - pivot - 1), pivot, less);
        //return Partitioner<iter, Compare>::partition(begin, end , pivot, less);
    }


    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition(iter begin, iter end, float ratio, Compare less) {
        if (end - begin < 45)
        {
            std::cout << "<45 shouldnt be here" << std::endl;
            if (end - begin == 2) {
                sort_pair(begin, begin + 1, less);
                return begin;
            }
            else if (end - begin > 2)
                return Partitioner<iter, Compare>::partition(begin, end, median_of_3(begin, end, less), less);
        }
        //	std::cout << "worstcase" << std::endl;
        iter searchit = begin;
        iter placeit = begin;
        while (searchit + 4 < end) {
            iter tempit = median_of_5(searchit, less);
            std::iter_swap(tempit, placeit);
            placeit++;
            searchit += 5;
        }

        //iter placeit = begin + 10;

        iter pivot = begin + ratio * (placeit - begin);
        kth_element<Partitioner>(begin, pivot, placeit, less);
        //std::iter_swap(placeit - 1, end - 1);

        std::swap_ranges(pivot + 1, placeit, end - (placeit - pivot - 1));
        iter result = Partitioner<iter, Compare>::partition(pivot, end - (placeit - pivot - 1), pivot, less);
        //	std::cout <<"n: "<<(end - begin)<< ", ratio: " << ratio << ", outcome: " << ((float)(result - begin) / (float)(end - begin)) << std::endl;
        return result;
        //return Partitioner<iter, Compare>::partition(begin, end , pivot, less);
    }


    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_partition_ninther(iter begin, iter end, float ratio, Compare less) {
        if (end - begin < 30)
        {
            std::cout << "<30 shouldnt be here" << std::endl;
            if (end - begin == 2) {
                sort_pair(begin, begin + 1, less);
                return begin;
            }
            else if (end - begin > 2)
                return Partitioner<iter, Compare>::partition(begin, end, median_of_3(begin, end, less), less);
        }
        //	std::cout << "worstcase" << std::endl;
        iter searchit = begin;
        iter placeit = begin;
        while (searchit + 8 < end) {
            iter tempit = median_of_3_medians_of_3(searchit, searchit + 9, less);
            std::iter_swap(tempit, placeit);
            placeit++;
            searchit += 9;
        }

        //iter placeit = begin + 10;

        iter pivot = begin + static_cast<size_t>(ratio * (placeit - begin));
        kth_element<Partitioner>(begin, pivot, placeit, less);
        //std::iter_swap(placeit - 1, end - 1);

        std::swap_ranges(pivot + 1, placeit, end - (placeit - pivot - 1));
#ifdef WORST_CASE_SIMULATION
        iter result;
        if (worst_case_simulation) {
            int offset;
            if (ratio < 0.5)
                offset = (end - begin) - 2 * ((end - begin) / 9);
            else
                offset = 2 * ((end - begin) / 9);
#ifdef MEASURE_COMPARISONS
            std::nth_element(begin, begin + offset, end);
            iter it = begin + offset;
            std::iter_swap(it, begin);
            result = Partitioner<iter, Compare>::partition(begin, end, begin, less);
#else
            std::nth_element(begin, begin + offset, end, less);
            result = begin + offset;
#endif // MEASURE_COMPARISONS
        }
        else
            result = Partitioner<iter, Compare>::partition(pivot, end - (placeit - pivot - 1), pivot, less);

#else
        iter result = Partitioner<iter, Compare>::partition(pivot, end - (placeit - pivot - 1), pivot, less);
#endif // WORST_CASE_SIMULATION

        //if(ratio < .1 || ratio >.9)
        //    std::cout << "n: " << (end - begin) << ", ratio: " << ratio << ", outcome: " << ((float)(result - begin) / (float)(end - begin)) << std::endl;
        return result;
    }


    template<template<class, class> class Partitioner, typename Function, typename iter, typename Compare>
    iter median_of_f_partition(iter begin, iter end, Function f, Compare less) {
        if (end - begin < 100)
        {
            if (end - begin == 2) sort_pair(begin, begin + 1, less);
            else if (end - begin > 2)
                return Partitioner<iter, Compare>::partition(begin, end, median_of_3(begin, end, less), less);
        }
        unsigned int num = (f(end - begin)) / 8;
        num = std::min(num, (unsigned int)((end - begin) / 16));
        num = std::max(num, (unsigned int)3);
        unsigned int step = (end - begin) / num;

        //		std::cout << "N: " << end - begin << ", num: " << num << std::endl;

        iter searchit_left = begin;
        iter searchit_right = end - 8;
        iter placeit = begin;
        for (unsigned int j = 0; j < num / 2; j++) {
            std::swap_ranges(placeit, placeit + 8, searchit_left);
            placeit += 8;
            std::swap_ranges(placeit, placeit + 8, searchit_right);
            placeit += 8;
            searchit_left += step;
            searchit_right -= step;
        }
        std::iter_swap(placeit, begin + (end - begin) / 2);
        ++placeit;
        //	std::cout << "searching median of : " << placeit - begin << ", elements: "  << std::endl;
        iter pivot = median<Partitioner>(begin, placeit, less);
        std::swap_ranges(pivot + 1, placeit, end - (placeit - pivot - 1));
        return Partitioner<iter, Compare>::partition(pivot, end - (placeit - pivot - 1), pivot, less);
    }







    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_of_3(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index num = end - begin;
        index k = num / 3 - 1;

        iter searchit_left = begin;
        iter searchit_right = end;
        iter placeit = begin;
        for (index j = 0; j < k / 2; j++) {
            std::iter_swap(placeit, median_of_3(searchit_left, searchit_left + 3, less));
            placeit++;
            std::iter_swap(placeit, median_of_3(searchit_right - 3, searchit_right, less));
            placeit++;
            searchit_left += 3;
            searchit_right -= 3;
        }
        iter middle = begin + (end - begin) / 2;
        std::iter_swap(placeit, median_of_3(middle - 1, middle + 2, less));
        ++placeit;
        middle = begin + (placeit - begin) / 2;
        //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
        kth_element<Partitioner>(begin, middle, placeit, less);  //
                                                                 //std::nth_element(begin, middle, placeit, less);
        std::swap_ranges(middle + 1, placeit, end - k / 2);
        return middle;
    }

    template<template<class, class> class Partitioner, bool hybrid, typename iter, typename Compare>
    iter median_of_medians_of_3_5__(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index num = end - begin;
        //mittleres Sample darf sich um maximal 5 mit den beiden Nachbarn überschneiden
        index k = (num + 10) / 15 - 1;

        iter searchit_left = begin;
        iter searchit_right = end;
        iter placeit = begin;
        for (index j = 0; j < k / 2; j++) {
            median_of_3(searchit_left, searchit_left + 3, less);
            median_of_3(searchit_left + 3, searchit_left + 6, less);
            median_of_3(searchit_left + 6, searchit_left + 9, less);
            median_of_3(searchit_left + 9, searchit_left + 12, less);
            median_of_3(searchit_left + 12, searchit_left + 15, less);
            //	std::iter_swap(placeit, median_of_5(searchit_left + 0, searchit_left + 3, searchit_left + 6, searchit_left + 9, searchit_left + 12, less));
            //	std::iter_swap(placeit, median_of_5(searchit_left + 1, searchit_left + 4, searchit_left + 7, searchit_left + 10, searchit_left + 13, less));
            std::iter_swap(placeit, median_of_5(searchit_left + 2, searchit_left + 5, searchit_left + 8, searchit_left + 11, searchit_left + 14, less));

            placeit++;

            median_of_3(searchit_right - 3, searchit_right, less);
            median_of_3(searchit_right - 6, searchit_right - 3, less);
            median_of_3(searchit_right - 9, searchit_right - 6, less);
            median_of_3(searchit_right - 12, searchit_right - 9, less);
            median_of_3(searchit_right - 15, searchit_right - 12, less);
            std::iter_swap(placeit, median_of_5(searchit_right - 2, searchit_right - 5, searchit_right - 8, searchit_right - 11, searchit_right - 14, less));
            placeit++;
            searchit_left += 15;
            searchit_right -= 15;
        }
        iter middle = begin + (end - begin) / 2;
        median_of_3(middle - 7, middle - 4, less);
        median_of_3(middle - 4, middle - 1, less);
        median_of_3(middle - 1, middle + 2, less);
        median_of_3(middle + 2, middle + 5, less);
        median_of_3(middle + 5, middle + 8, less);
        std::iter_swap(placeit, median_of_5(middle - 6, middle - 3, middle, middle + 3, middle + 6, less));
        placeit++;


        middle = begin + (placeit - begin) / 2;
        //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
        if (hybrid)
            kth_element_hybrid<Partitioner>(begin, middle, placeit, less);  //
        else
            kth_element<Partitioner>(begin, middle, placeit, less);  //


                                                                     //std::nth_element(begin, middle, placeit, less);
        std::swap_ranges(middle + 1, placeit, end - k / 2);
        return middle;
    }


    template<template<class, class> class Partitioner, typename iter, typename Compare>
    iter median_of_medians_of_3_5____old(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index num = end - begin;

        index k = 2 * ((num - 15) / 30);

        iter searchit_left = begin;
        iter searchit_right = end;
        iter placeit = begin;
        for (index j = 0; j < k / 2; j++) {
            median_of_3(searchit_left, searchit_left + 3, less);
            median_of_3(searchit_left + 3, searchit_left + 6, less);
            median_of_3(searchit_left + 6, searchit_left + 9, less);
            median_of_3(searchit_left + 9, searchit_left + 12, less);
            median_of_3(searchit_left + 12, searchit_left + 15, less);
            std::iter_swap(placeit, median_of_5(searchit_left + 0, searchit_left + 3, searchit_left + 6, searchit_left + 9, searchit_left + 12, less));
            //	std::iter_swap(placeit, median_of_5(searchit_left + 1, searchit_left + 4, searchit_left + 7, searchit_left + 10, searchit_left + 13, less));
            //std::iter_swap(placeit, median_of_5(searchit_left + 2, searchit_left + 5, searchit_left + 8, searchit_left + 11, searchit_left + 14, less));

            placeit++;

            median_of_3(searchit_right - 3, searchit_right, less);
            median_of_3(searchit_right - 6, searchit_right - 3, less);
            median_of_3(searchit_right - 9, searchit_right - 6, less);
            median_of_3(searchit_right - 12, searchit_right - 9, less);
            median_of_3(searchit_right - 15, searchit_right - 12, less);
            std::iter_swap(placeit, median_of_5(searchit_right - 3, searchit_right - 6, searchit_right - 9, searchit_right - 12, searchit_right - 15, less));
            //std::iter_swap(placeit, median_of_5(searchit_right - 2, searchit_right - 5, searchit_right - 8, searchit_right - 11, searchit_right - 14, less));
            //std::iter_swap(placeit, median_of_5(searchit_right - 1, searchit_right - 4, searchit_right - 7, searchit_right - 10, searchit_right - 14, less));

            placeit++;
            searchit_left += 15;
            searchit_right -= 15;
        }
        iter middle = begin + (end - begin) / 2;
        median_of_3(middle - 7, middle - 4, less);
        median_of_3(middle - 4, middle - 1, less);
        median_of_3(middle - 1, middle + 2, less);
        median_of_3(middle + 2, middle + 5, less);
        median_of_3(middle + 5, middle + 8, less);
        std::iter_swap(placeit, median_of_5(middle - 6, middle - 3, middle, middle + 3, middle + 6, less));
        placeit++;


        middle = begin + (placeit - begin) / 2;
        //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
        kth_element<Partitioner>(begin, middle, placeit, less);  //
                                                                 //std::nth_element(begin, middle, placeit, less);
        std::swap_ranges(middle + 1, placeit, end - k / 2);
        return middle;
    }
#define RICHJTIG

#ifdef RICHJTIG
    template<template<class, class> class Partitioner, bool hybrid, bool undersampling = false, typename iter, typename Compare>
    iter median_of_medians_of_3_5(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index num = end - begin;


        index k;
        if (!undersampling || num < 1000)
            k = 2 * ((num - 15) / 30) + 1;
        else
            k = 2 * ((num - 15) / UNDERSAMPLING_SIZE) + 1;

        index third = (end - begin) / 3;
        iter searchit_left = begin;
        iter searchit_right = end - 1;
        iter searchit_middle_left = begin + third;
        iter searchit_middle_middle = begin + third + 2 * k;
        iter start_sample = searchit_middle_middle;
        iter end_sample = begin + third + 3 * k;
        iter searchit_middle_right = begin + 2*third ;

#ifdef SHOW_COMPARISONS
        int comps = comparisons;
#endif // SHOW_COMPARISONS



        for (index j = 0; j < k; j++) {
            median_of_3(searchit_left++, searchit_middle_left++, searchit_right--, less);
            median_of_3(searchit_left++, searchit_middle_left++, searchit_right--, less);
            median_of_3(searchit_left++, searchit_middle_middle++, searchit_right--, less);
            median_of_3(searchit_left++, searchit_middle_right--, searchit_right--, less);
            median_of_3(searchit_left++, searchit_middle_right--, searchit_right--, less);

            median_of_5(searchit_middle_left - 2, searchit_middle_left - 1, searchit_middle_middle - 1, searchit_middle_right + 2, searchit_middle_right + 1, less);
        }

        assert(searchit_middle_middle == end_sample);
#ifdef SHOW_COMPARISONS
        std::cout << "anz: " << num << "finding sample: " << ((double)comparisons - comps) / num << std::endl;
        comps = comparisons;
#endif // SHOW_COMPARISONS
        iter middle = start_sample + k / 2;
        //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
        if (hybrid)
            kth_element_hybrid<Partitioner>(start_sample, middle, end_sample, less);  //
        else
            kth_element<Partitioner>(start_sample, middle, end_sample, less);  //


#ifdef SHOW_COMPARISONS
        std::cout << "anz: " << num << "kth_element: " << ((double)comparisons - comps) / num << std::endl;
#endif // SHOW_COMPARISONS

#ifdef WORST_CASE_SIMULATION
        iter result;
        if (worst_case_simulation) {

//#define FIXED_RATIO_PARTITIONING 


#ifdef FIXED_RATIO_PARTITIONING
        int offset = num - 0.5*(k + 1);
        if (num < 10000)
            offset = 3 * k;
        //	std::cout << "worst_case_simulation searching median of : " << 6*k << ", elements: " << std::endl;
        std::nth_element(begin, begin + offset, end, less);
        std::iter_swap(begin + offset, begin + k / 2);
        return begin + k / 2;


#else
#ifdef WORST_CASE_PARTITION_SIMULATION

        int offset = 3 * (k + 1);

        //	std::cout << "worst_case_simulation searching median of : " << 6*k << ", elements: " << std::endl;
        std::nth_element(begin, begin + offset, end);
        std::iter_swap(begin + offset, begin + k / 2);
        return begin + k / 2;
#else
        std::nth_element(begin, begin + num / 2, end);
        std::iter_swap(begin + num / 2, begin + k / 2);
        return begin + k / 2;
#endif
#endif // FIXED_RATIO_PARTITIONING
        }
       else
       {
           assert((middle + 1) - start_sample == k / 2 + 1);
           std::swap_ranges(start_sample, middle + 1, begin);
           assert(end_sample - (middle + 1) == k / 2);
           std::swap_ranges(middle + 1, end_sample, end - k / 2);
           return begin + k / 2;
       }

#else



    assert((middle + 1) - start_sample == k / 2 + 1);
    std::swap_ranges(start_sample, middle + 1, begin);
    assert(end_sample - (middle + 1) == k / 2);
    std::swap_ranges(middle + 1, end_sample, end - k / 2);

    /*	for (int i = 0; i < k / 2; i++)
    {
    assert(begin[i] <= *(begin+k/2));
    assert(end[-i-1] >= *(begin + k / 2));
    }*/

    return begin + k / 2;

#endif
    }

#else


template<template<class, class> class Partitioner, bool hybrid, bool undersampling = false, typename iter, typename Compare>
iter median_of_medians_of_3_5(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index num = end - begin;


    index k;
    if (!undersampling || num < 400)
        k = 2 * ((num - 5) / 5) + 1;
    else
        k = 2 * ((num - 5) / 7) + 1;

    iter middle = begin + k / 2;
    iter end_sample = begin + k;
    //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
    if (hybrid)
        kth_element_hybrid<Partitioner>(begin, middle, end_sample, less);  //
    else
        kth_element<Partitioner>(begin, middle, end_sample, less);  //


#ifdef SHOW_COMPARISONS
    std::cout << "anz: " << num << "kth_element: " << ((double)comparisons - comps) / num << std::endl;
#endif // SHOW_COMPARISONS

#ifdef WORST_CASE_SIMULATION
    iter result;
    if (worst_case_simulation) {

#ifdef WORST_CASE_PARTITION_SIMULATION
        int offset = 3 * (k + 1);
        //	std::cout << "worst_case_simulation searching median of : " << 6*k << ", elements: " << std::endl;
        std::nth_element(begin, begin + offset, end, less);
        std::iter_swap(begin + offset, begin + k / 2);
        return begin + k / 2;
#else
        std::nth_element(begin, begin + num / 2, end);

        std::iter_swap(begin + num / 2, begin + k / 2);
        return begin + k / 2;
#endif
    }
    else
    {
        //  assert((middle + 1) - start_sample == k / 2 + 1);
        //  std::swap_ranges(begin, middle + 1, begin);
         // assert(end_sample - (middle + 1) == k / 2);
        std::swap_ranges(middle + 1, end_sample, end - k / 2);
        return begin + k / 2;
    }

#else


    //  assert((middle + 1) - start_sample == k / 2 + 1);
    //  std::swap_ranges(begin, middle + 1, begin);
    // assert(end_sample - (middle + 1) == k / 2);
    std::swap_ranges(middle + 1, end_sample, end - k / 2);
    return begin + k / 2;

#endif
}





#endif





template<template<class, class> class Partitioner, bool hybrid, bool undersampling = false, typename iter, typename Compare>
iter median_of_medians_of_3_fifth_guarantee(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index num = end - begin;


    index k;
    if (!undersampling || num < 400)
        k = 6 * ((num - 5) / 10) + 1;
    else
        k = 6 * ((num - 5) / 13) + 1;

    index third = (end - begin) / 3;
    iter searchit_left = begin;
    iter searchit_right = end - 1;
    iter searchit_middle = begin + third;

    iter start_sample = searchit_middle;
    iter end_sample = searchit_middle + k;

#ifdef SHOW_COMPARISONS
    int comps = comparisons;
#endif // SHOW_COMPARISONS



    for (index j = 0; j < k; j++) {
        median_of_3(searchit_left++, searchit_middle++, searchit_right--, less);
    }

#ifdef SHOW_COMPARISONS
    std::cout << "anz: " << num << "finding sample: " << ((double)comparisons - comps) / num << std::endl;
    comps = comparisons;
#endif // SHOW_COMPARISONS
    iter middle = start_sample + k / 2;
    //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
    if (hybrid)
        kth_element_hybrid<Partitioner>(start_sample, middle, end_sample, less);  //
    else
        kth_element<Partitioner>(start_sample, middle, end_sample, less);  //


#ifdef SHOW_COMPARISONS
    std::cout << "anz: " << num << "kth_element: " << ((double)comparisons - comps) / num << std::endl;
#endif // SHOW_COMPARISONS

#ifdef WORST_CASE_SIMULATION
    iter result;
    if (worst_case_simulation) {

#ifdef WORST_CASE_PARTITION_SIMULATION
        int offset = 3 * (k + 1);
        //	std::cout << "worst_case_simulation searching median of : " << 6*k << ", elements: " << std::endl;
        std::nth_element(begin, begin + offset, end, less);
        std::iter_swap(begin + offset, begin + k / 2);
        return begin + k / 2;
#else
        std::nth_element(begin, begin + num / 2, end);

        std::iter_swap(begin + num / 2, begin + k / 2);
        return begin + k / 2;
#endif
    }
    else
    {
        assert((middle + 1) - start_sample == k / 2 + 1);
        std::swap_ranges(start_sample, middle + 1, begin);
        assert(end_sample - (middle + 1) == k / 2);
        std::swap_ranges(middle + 1, end_sample, end - k / 2);
        return begin + k / 2;
    }

#else



    assert((middle + 1) - start_sample == k / 2 + 1);
    std::swap_ranges(start_sample, middle + 1, begin);
    assert(end_sample - (middle + 1) == k / 2);
    std::swap_ranges(middle + 1, end_sample, end - k / 2);

    /*	for (int i = 0; i < k / 2; i++)
    {
    assert(begin[i] <= *(begin+k/2));
    assert(end[-i-1] >= *(begin + k / 2));
    }*/

    return begin + k / 2;

#endif
}



template<template<class, class> class Partitioner, bool hybrid, typename iter, typename Compare>
iter median_of_medians_of_5_3(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index num = end - begin;


    index k = 2 * ((num - 15) / 40) + 1;

    index fifth = (end - begin) / 5;
    iter searchit_left = begin;
    iter searchit_right = end - 1;
    iter searchit_middle_left = begin + 2 * fifth;
    iter searchit_middle_middle = searchit_middle_left + k;
    iter start_sample = searchit_middle_middle;
    iter end_sample = start_sample + k;
    iter searchit_middle_right = searchit_middle_left + 2 * k;

    for (index j = 0; j < k; j++) {

        median_of_5(searchit_left, searchit_left + 1, searchit_middle_left, searchit_right - 1, searchit_right, less);
        searchit_left += 2;
        searchit_right -= 2;
        median_of_5(searchit_left, searchit_left + 1, searchit_middle_middle, searchit_right - 1, searchit_right, less);
        searchit_left += 2;
        searchit_right -= 2;
        median_of_5(searchit_left, searchit_left + 1, searchit_middle_right, searchit_right - 1, searchit_right, less);
        searchit_left += 2;
        searchit_right -= 2;

        median_of_3(searchit_middle_left++, searchit_middle_middle++, searchit_middle_right++, less);
    }

    assert(searchit_middle_middle == end_sample);

    iter middle = start_sample + k / 2;
    //	std::cout << "searching median of : " << placeit - begin << ", elements: " << std::endl;
    if (hybrid)
        kth_element_hybrid<Partitioner>(start_sample, middle, end_sample, less);  //
    else
        kth_element<Partitioner>(start_sample, middle, end_sample, less);  //


    assert((middle + 1) - start_sample == k / 2 + 1);
    std::swap_ranges(start_sample, middle + 1, begin);
    assert(end_sample - (middle + 1) == k / 2);
    std::swap_ranges(middle + 1, end_sample, end - k / 2);

    /*	for (int i = 0; i < k / 2; i++)
    {
    assert(begin[i] <= *(begin+k/2));
    assert(end[-i-1] >= *(begin + k / 2));
    }*/

    return begin + k / 2;
}






}
#endif // !MEDIAN_OF_MEDIAN