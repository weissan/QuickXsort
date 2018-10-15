/******************************************************************************
* partition.h
*
* Different partition algorithms with interfaces for different pivot selection strategies:
* 4x block partition (simple, loops unrolled, loops unrolled + duplicate check, Hoare finish),
* Lomuto partitioner by Elmasry, Katajainen and Stenmark, and Hoare parititioner
*
******************************************************************************
* Copyright (C) 2016 Stefan Edelkamp <edelkamp@tzi.de>
* Copyright (C) 2016 Armin Weiﬂ <armin.weiss@fmi.uni-stuttgart.de>
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


#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <assert.h>
#include <functional>


//#defineMOREPARTITIONERS
#ifndef BLOCKSIZE
#define BLOCKSIZE 32
#endif
#ifndef PIVOTSAMPLESIZE
#define PIVOTSAMPLESIZE 23
#endif
#ifndef MO3_THRESH
#define MO3_THRESH (PIVOTSAMPLESIZE*PIVOTSAMPLESIZE)
#endif

#include "stl_gcc.h++"
#include "median.h"



namespace partition {

    template<typename iter, typename Compare>
    inline void partitiontest(iter begin, iter pivot, iter end, Compare less) {
        bool error = false;
        for (iter it = begin; it < pivot; it++)
        {
            //	std::cout << ", " << *it; // << std::endl;
            if (less(*pivot, *it))
            {
                //std::cout << "error as position " << it - begin << " value: " <<*it << " previous value: " << *(it-1) << std::endl;
                std::cout << "error as position " << it - begin << std::endl;

                error = true;
            }
        }
        for (iter it = pivot; it < end; it++)
        {
            //	std::cout << ", " << *it; // << std::endl;
            if (less( *it,*pivot))
            {
                //std::cout << "error as position " << it - begin << " value: " <<*it << " previous value: " << *(it-1) << std::endl;
                std::cout << "error as position " << it - begin << std::endl;

                error = true;
            }
        }
        //	std::cout << std::endl;
     /*   if (error) {
            quickmerge::print(begin, end);
        }*/
    }




    template<typename iter, typename Compare>
    struct Stl_partition__ {

        // for adaptive selection
        static inline iter partition(iter begin, iter end, Compare less) {
            iter mid;

            iter middle = begin + (end - begin) / 2;
            median::median_of_3(begin, middle, end - 1, less);


            std::iter_swap(middle, begin);
            iter second = stl_gcc::unguarded_partition(begin + 1, end, *begin);
            std::iter_swap(begin, second - 1);

            return second - 1;



        }


    };

    template<typename iter, typename Compare>
    struct Stl_partition {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid = median::median_of_3(begin, end, less);
            //partition
            std::iter_swap(begin + 1, mid);

            mid = stl_gcc::unguarded_partition(begin + 2, end - 1, *(begin + 1), less);
            //			if (less(*(begin + 1), *mid))
            mid--;

            std::iter_swap(begin + 1, mid);


            return mid;
        }

        static inline iter partition(iter begin, iter end, iter pivot, Compare less) {

            //partition
            std::iter_swap(begin, pivot);

            iter mid = stl_gcc::unguarded_partition(begin + 1, end, *begin, less);
            mid--;

            std::iter_swap(begin, mid);


            return mid;
        }
        /*
                //with duplicate check
                static inline iter partition(iter begin, iter end, Compare less, int & pivot_length) {
                    //choose pivot
                    iter mid = median::median_of_3(begin, end, less);
                    //partition
                    return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less, pivot_length);
                }
                static inline iter partition(iter begin, iter end, iter pivot, Compare less) {
                    //partition
                    return hoare_block_partition_unroll_loop(begin, end, pivot, less);
                }*/
    };



    

    template<typename iter, typename Compare>
    struct Stl_partition_no_sampling {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter __first = begin+1;
            iter __last = end;
            iter __pivot = begin;


             while (true)
            {
                while (__first< end && less(*__first, *__pivot))
                    ++__first;
                --__last;
                while (less(*__pivot, *__last))
                    --__last;
                if (!(__first < __last))
                    goto finish;
                std::iter_swap(__first, __last);
                ++__first;
            }


 /*          while (true)
            {
                while (true)
                    if (__first == __last)
                        goto finish;
                    else if (less(*begin,*__first))
                        ++__first;
                    else
                        break;
                --__last;
                while (true)
                    if (__first == __last)
                        goto finish;
                    else if (less(*__last, *begin))
                        --__last;
                    else
                        break;
                std::iter_swap(__first, __last);
                ++__first;
            }*/
       finish:
           std::iter_swap(begin, __first - 1);
           return __first - 1;

            //partition
         /*   while (last != begin && less(*pivot, *last))
                last--;
            if (last != begin)
            {
                iter first = begin + 1;
                iter mid;
                while (first != last && less(*first,*pivot))
                    first++;
                if (first == last)
                {
                    mid = first + 1;
                }
                else {
                    mid = stl_gcc::unguarded_partition(first, last, *begin, less);
                    mid--;
                }

                std::iter_swap(begin, mid);


                return mid;
            }
            else
                return begin;

     */
        }
    };


    template<typename iter, typename Compare>
    struct Stl_partition_mo5_mo5 {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid;
            if (end - begin > 2000)
                mid = median::median_of_5_medians_of_5(begin, end, less);
            else if (end - begin > 200)
                mid = median::median_of_3_medians_of_3(begin, end, less);
            else
                mid = median::median_of_3(begin, end, less);
            //partition
            std::iter_swap(begin + 1, mid);

            mid = stl_gcc::unguarded_partition(begin + 2, end - 1, *(begin + 1), less);
            //			if (less(*(begin + 1), *mid))
            mid--;

            std::iter_swap(begin + 1, mid);


            return mid;
        }

        static inline iter partition(iter begin, iter end, iter pivot, Compare less) {

            //partition
            std::iter_swap(begin, pivot);

            iter mid = stl_gcc::unguarded_partition(begin + 1, end, *begin, less);
            mid--;

            std::iter_swap(begin, mid);


            return mid;
        }
        /*
        //with duplicate check
        static inline iter partition(iter begin, iter end, Compare less, int & pivot_length) {
        //choose pivot
        iter mid = median::median_of_3(begin, end, less);
        //partition
        return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less, pivot_length);
        }
        static inline iter partition(iter begin, iter end, iter pivot, Compare less) {
        //partition
        return hoare_block_partition_unroll_loop(begin, end, pivot, less);
        }*/
    };

    /// This is a helper function...
    template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
    _RandomAccessIterator
        unguarded_partition_right(_RandomAccessIterator first,
            _RandomAccessIterator last,
            const _Tp& pivot, _Compare comp)
    {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (last >= first && !comp(*last, pivot))
            --last;
        if (!(first < last))
            return first;
        std::iter_swap(first, last);
        ++first;
        while (true)
        {
            while (comp(*first, pivot))
                ++first;
            --last;
            while (!comp(*last, pivot))
                --last;
            if (!(first < last))
                return first;
            std::iter_swap(first, last);
            ++first;
        }
    }
    /// This is a helper function...
    template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
    _RandomAccessIterator
        unguarded_partition_left(_RandomAccessIterator first,
            _RandomAccessIterator last,
            const _Tp& pivot, _Compare comp)
    {
        while (first < last && !comp(pivot, *first))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        std::iter_swap(first, last);
        ++first;
        while (true)
        {
            while (!comp(pivot, *first))
                ++first;
            --last;
            while (comp(pivot, *last))
                --last;
            if (!(first < last))
                return first;
            std::iter_swap(first, last);
            ++first;
        }
    }

    template<typename iter, typename Compare>
    struct Stl_partition_mosqrt {

        // for adaptive selection
        static inline iter partition(iter begin, iter end, double ratio, Compare less) {
            iter mid;
            if (end - begin > 120)
            {
                unsigned int n = end - begin;
                double sqrt_inv = sqrt(n*log(n)) / (double)n;
                double sample_size_factor = 1.0;
                bool right_side = false;
                if (ratio > 0.5)
                {
                    right_side = true;
                    ratio = 1.0 - ratio;
                }
                if (ratio < 0.3)
                    ratio += sqrt_inv;


                if (ratio < 0.1) {
                    if (ratio > 0.05)
                        ratio = 0.05 + 0.5*ratio;
                    else
                        ratio = 2 * ratio;
                    if (ratio < 0.02)
                        sample_size_factor = 1 + (0.02 - ratio)*(0.02 - ratio) * 50 * 50 * log(n);
                }

                if (right_side)
                    ratio = 1.0 - ratio;

                // float sample_size_factor = 2;//std::max(0.24+ 0.05*log(end - begin),1.5);
                unsigned int pivot_sample_size = sqrt(end - begin)*sample_size_factor;
                if (pivot_sample_size > n - 3)
                    pivot_sample_size = n - 2;
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                unsigned int k = pivot_sample_size*ratio;
               

#ifdef WORST_CASE_SIMULATION
                iter second;
                
                if (worst_case_simulation) {
                    mid = median::kth_element_of_n(begin, end, less, k, pivot_sample_size);
                    if (ratio > 0.5)
                    {
                        mid = median::kth_element_of_n(begin, end, less, 0, pivot_sample_size);
                        //std::nth_element(begin, begin + k, end, less);
                        //return begin + k;
                        second = stl_gcc::unguarded_partition(mid + 1, end - pivot_sample_size + k + 1, *mid, less);
                        std::iter_swap(mid, second - 1);
                        return second - 1;
                    }
                    else
                    {
                        mid = median::kth_element_of_n(begin, end, less, pivot_sample_size - 1, pivot_sample_size);
                        //std::nth_element(begin, end - (pivot_sample_size - k), end, less);
                       // return end - (pivot_sample_size - k);
                        second = stl_gcc::unguarded_partition(mid + 1, end - pivot_sample_size + k + 1, *mid, less);
                        std::iter_swap(mid, second - 1);
                        return second - 1;
                    }
                }
                else
                {
                    mid = median::kth_element_of_n(begin, end, less, k, pivot_sample_size);
                    second = stl_gcc::unguarded_partition(mid + 1, end - pivot_sample_size + k + 1, *mid, less);
                    std::iter_swap(mid, second - 1);
                    return second - 1;
                }
#else

                mid = median::kth_element_of_n(begin, end, less, k, pivot_sample_size);
                // std::nth_element(begin, begin + k, begin + pivot_sample_size, less);
                //std::swap_ranges( begin + k + 1, begin + pivot_sample_size, end - pivot_sample_size + k + 1);
                //partition
                // mid = begin + k;

                //std::iter_swap(mid, begin);
                iter second = stl_gcc::unguarded_partition(mid + 1, end - pivot_sample_size + k + 1, *mid, less);
                std::iter_swap(mid, second - 1);
                return second - 1;
#endif




            }
            else
            {

#ifdef WORST_CASE_SIMULATION
                iter second;
                if (worst_case_simulation) {
                    //                   std::cout << "asdf" << std::endl;
                    if (ratio > 0.5)
                    {
                        if (ratio > 1.0 - 0.182) {
                            std::nth_element(begin, end - 1, end, less);
                            return end - 1;
                        }
                        else {
                            std::nth_element(begin, begin + 1, end, less);
                            return begin + 1;
                        }

                    }
                    else
                    {
                        if (ratio < 0.182) {
                            std::nth_element(begin, begin, end, less);
                            return begin;
                        }
                        else {
                            std::nth_element(begin, end - 2, end, less);
                            return end - 2;
                        }
                        
                    }
                }
                else
                {
                    iter middle = begin + (end - begin) / 2;
                    median::median_of_3(begin, middle, end - 1, less);
                    if (ratio > 1.0 - 0.182)
                        std::iter_swap(end - 1, middle);
                    else if (ratio >= 0.182)
                        std::iter_swap(begin, middle);


                    iter second = stl_gcc::unguarded_partition(begin + 1, end, *begin, less);
                    std::iter_swap(begin, second - 1);
                    return second - 1;
                }
#else

                iter middle = begin + (end - begin) / 2;
                median::median_of_3(begin, middle, end - 1, less);
                if (ratio > 1.0 - 0.182)
                    std::iter_swap(end - 1, middle);
                else if (ratio >= 0.182)
                    std::iter_swap(begin, middle);


                iter second = stl_gcc::unguarded_partition(begin + 1, end, *begin, less);
                std::iter_swap(begin, second - 1);
 //               if (second == begin + 1)
 //                   std::cout << " ratio: " << ratio << " n: " << (end - begin) << " return: " << second - begin - 1 << std::endl;
                return second - 1;
#endif
            }


        }


        static inline iter partition(iter begin, iter end, Compare less, bool inner_array, int & pivot_width) {
            //choose pivot
            iter mid;
            pivot_width = 1;
            if (end - begin < 4000)
            {
                if (end - begin > 500)
                    mid = median::median_of_5_medians_of_5(begin, end, less);
                else if (end - begin > 60)
                    mid = median::median_of_3_medians_of_3(begin, end, less);
                else
                    mid = median::median_of_3(begin, end, less);
                return Stl_partition<iter, Compare>::partition(begin + 1, end - 1, mid, less);
            }
            else
            {
                unsigned int pivot_sample_size = sqrt(end - begin);
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                mid = median::median_of_k(begin, end, less, pivot_sample_size);//choose pivot as median of sqrt(n)

                if (inner_array && !less(*(begin - 1), *mid))
                {
                    iter cut = unguarded_partition_left(mid + 1, end - (mid - begin), *(mid), less);
                    //if (less(*mid, *cut))
                    pivot_width = cut - begin;
                    return begin;
                }
                else {

#ifdef WORST_CASE_SIMULATION
                    iter cut;
              //      asdfasdfasdfasfd

                    if (worst_case_simulation) {
                        cut = begin + pivot_sample_size / 2;
                        std::nth_element(begin, begin + pivot_sample_size / 2, end, less);
                    }
                    else
                    {
                        cut = unguarded_partition_right(mid + 1, end - (mid - begin), *(mid), less);
                        cut--;

                        std::iter_swap(cut, mid);
                    }
#else


                    iter cut = unguarded_partition_right(mid + 1, end - (mid - begin), *(mid), less);
                    cut--;

                    std::iter_swap(cut, mid);
#endif



                    if (cut - begin < (end - begin) / 5 - 30)
                    {
                        iter cut2 = unguarded_partition_left(cut + 1, end, *(cut), less);
                        //if (less(*mid, *cut))
                        //cut2--;

                        //std::iter_swap(cut2, cut);
                        pivot_width = cut2 - cut;
                    }

                    return cut;
                }
            }
        }

/*
        //with duplicate check
        static inline iter partition(iter begin, iter end, Compare less, int & pivot_length) {
            iter mid;
            if (end - begin > 20000)
            {
                unsigned int pivot_sample_size = sqrt(end - begin);
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                mid = median::median_of_k(begin, end, less, pivot_sample_size);//choose pivot as median of sqrt(n)
                                                                               //partition
                return hoare_block_partition_unroll_loop(begin + pivot_sample_size / 2, end - pivot_sample_size / 2, mid, less, pivot_length);
            }
            else
            {
                if (end - begin > 800)
                    mid = median::median_of_5_medians_of_5(begin, end, less);
                else if (end - begin > 100)
                    mid = median::median_of_3_medians_of_3(begin, end, less);
                else
                    mid = median::median_of_3(begin, end, less);
                //mid = median::median_of_3(begin , end , less);
                return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less, pivot_length);
            }
        }
*/


        //without duplicate check
        static inline iter partition(iter begin, iter end, Compare less) {
  //          std::cout << "mosqrt" << std::endl;
            iter mid;
            if (end - begin > 20000)
            {

                unsigned int pivot_sample_size = sqrt(end - begin);
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                mid = median::median_of_k(begin, end, less, pivot_sample_size);//choose pivot as median of sqrt(n)

                std::iter_swap(mid, begin);

                iter second = stl_gcc::unguarded_partition(begin + pivot_sample_size / 2, end - pivot_sample_size / 2, *begin, less);
                std::iter_swap(begin, second - 1);
                
                //partitiontest(begin, second - 1, end, less);

                return second - 1;           
            }
            else
            {
                if (end - begin > 800)
                    mid = median::median_of_5_medians_of_5(begin, end, less);
                else if (end - begin > 100)
                    mid = median::median_of_3_medians_of_3(begin, end, less);
                else
                    mid = median::median_of_3(begin, end, less);
                //mid = median::median_of_3(begin , end , less);
                std::iter_swap(begin, begin + 1);
                std::iter_swap(mid, begin);
                iter second = stl_gcc::unguarded_partition(begin + 2, end - 1, *begin, less);
                std::iter_swap(begin, second - 1);
                return second - 1;

                //return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less);
            }
        }


    };



    template<typename iter, typename Compare>
    struct Stl_partition_mo5 {

        static inline iter partition(iter begin, iter end, Compare less) {
            //          std::cout << "mosqrt" << std::endl;
            iter mid;
            if (end - begin > 80)
                mid = median::median_of_5(begin, end, less);
            else
                mid = median::median_of_3(begin, end, less);
            //mid = median::median_of_3(begin , end , less);
            std::iter_swap(mid, begin + 1);
            iter second = stl_gcc::unguarded_partition(begin + 2, end - 1, *(begin + 1), less);
            std::iter_swap(begin + 1, second - 1);
            return second - 1;

        }


    };




    template<typename iter, typename Compare>
    struct Hoare_partition {
        static inline iter partition(iter begin, iter end, Compare less) {
            using t = typename std::iterator_traits<iter>::value_type;
            iter last = end - 1;
            iter mid = begin + ((end - begin) / 2);

            leanswap(begin, last, less);
            leanswap(mid, last, less);
            leanswap(mid, begin, less);

            t q = std::move(*begin);
            mid = begin;
        loop:
            do; while (less(*(++begin), q));
            do; while (less(q, *(--last)));
            if (begin <= last) {
                std::swap(*begin, *last);
                goto loop;
            }
            std::swap(*mid, *last);
            return last;
        }
    };








};