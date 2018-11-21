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


//#define BASE

#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"


#ifndef MEASURE_COMPARISONS
#define BASE

#ifndef INS_SIZE
#define INS_SIZE 42
#endif // !INS_SIZE

#else

#define INS_SIZE 1
#endif // !MEASURE_COMPARISONS




#ifdef BASE
#include "smallsort.h"
#endif

namespace mergesort {
    template<typename iter>
    inline void print(iter begin, iter end) {
        for (iter it = begin; it < end; it++)
        {
            std::cout << ", " << *it; // << std::endl;
        }
        std::cout << std::endl;
    }
    template<typename iter, typename Compare>
    inline void sorttest(iter begin, iter end, Compare less) {
        bool error = false;
        for (iter it = begin + 1; it < end; it++)
        {
            //	std::cout << ", " << *it; // << std::endl;
            if (less(*it, *(it - 1)))
            {
                std::cout << "error as position " << it - begin << std::endl;
                error = true;
            }
        }
        //	std::cout << std::endl;
        if (error) {
            //	print(begin, end);
        }
    }


    template<typename iter, typename Compare>
    inline void sorttest(iter begin, iter end, Compare less, std::string name) {
        for (iter it = begin + 1; it < end; it++)
        {
            //	std::cout << ", " << *it; // << std::endl;
            if (less(*it, *(it - 1)))
                std::cout << name << " : error as position " << it - begin << std::endl;
            //				std::cout <<"seed: "<<seedg<< ", " << name << " : error as position " << it - begin << std::endl;
        }
        //	std::cout << std::endl;
    }


#define BRANCHLESS_MERGE


#ifdef BRANCHLESS_MERGE
    template<typename iter1, typename iter2, typename Compare>
    inline void merge(iter1 i1, int64_t num, iter2 begintarget, iter2 endtarget, Compare less) {
        using t2 = typename std::iterator_traits<iter2>::value_type;
        iter2 ires = begintarget;
        iter2 i2 = begintarget + num;
        assert(num >= 1 && endtarget - i2 >= 1);
        if ((num < 1 || endtarget - i2 < 1))
            std::cout << "ERROR: merge: one part empty" << std::endl;
        iter1 end1 = i1 + num;
        while (i1 < end1 && i2 < endtarget) {
            bool smaller = less(*i1, *i2);    
            t2* tempit = &*i2 + smaller * (&*i1 - &*i2);
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;

            *ires++ = std::move(*tempit);
        }

        while (i1 < end1) {
            *ires++ = std::move(*i1++);
        }
    }

#else

    template<typename iter1, typename iter2, typename Compare>
    inline void merge(iter1 i1, int64_t num, iter2 begintarget, iter2 endtarget, Compare less) {
        using t2 = typename std::iterator_traits<iter2>::value_type;
        iter2 ires = begintarget;
        iter2 i2 = begintarget + num;
        assert(num >= 1 && endtarget - i2 >= 1);
        if ((num < 1 || endtarget - i2 < 1))
            std::cout << "ERROR: merge: one part empty" << std::endl;
        iter1 end1 = i1 + num;
        while (i1 < end1 && i2 < endtarget) {
       /*     bool smaller = less(*i1, *i2);
            *ires++ = std::move(smaller ? *i1 : *i2);
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;*/
            if (less(*i1, *i2)) 
                *ires++ = std::move(*i1++);
            else 
                *ires++ = std::move(*i2++);
        }

        while (i1 < end1) {
            *ires++ = std::move(*i1++);
        }
    }

#endif


    // [begin, begin + n - n / 2), [target, target + n) must be disjoint
    template<typename iter1, typename iter2, typename Compare>
    inline void msort(iter1 begin, iter1 end, iter2 target, Compare less) {
        //     typedef typename std::iterator_traits<iter1>::difference_type index;
        using t = typename std::iterator_traits<iter1>::value_type;
        int64_t n = end - begin;

        if (n == 1) { std::swap(begin[0], target[0]); return; }



        if (n <= INS_SIZE)
        {
            t buffer[INS_SIZE];
            std::move(begin, end, buffer);
            stl_gcc::insertion_sort(buffer, buffer + n, less);
            std::move(target, target + n, begin);
            std::move(buffer, buffer + n, target);
            return;
        }

#ifdef BASE
        if (n == 5) { smallsort::sort5(begin, target, less); return; }
        if (n == 6) { smallsort::sort6(begin, target, less); return; }
        if (n == 7) { smallsort::sort7(begin, target, less); return; }
        if (n == 8) { smallsort::sort8(begin, target, less); return; }
        if (n == 9) { smallsort::sort9(begin, target, less); return; }
#endif


        int64_t q = n / 2;


        msort(begin, end - q, target + q, less);
        msort(end - q, end, begin, less);
        //        		std::cout << "vor merge" << end - begin << std::endl;
        //       print(begin, end);
        merge(begin, q, target, target + n, less);
        //      		std::cout << "nach merge" << end - begin << std::endl;
        //              print(target, target + n);
        //             std::cout << std::endl;
        //merge(begin, q, target, target + n, less);
    }


    template<typename iter1, typename iter2, typename Compare>
    inline void msortOut(iter1 begin, iter1 end, iter2 temp, Compare less) {
        typedef typename std::iterator_traits<iter1>::difference_type index;
        index n = end - begin;
        if (n > 1) {
            index q;
            q = n / 2;
            index r = n - q;

#ifdef BASE
            if (n == 5) { smallsort::sort5(begin, begin, less); return; }
            if (n == 6) { smallsort::sort6(begin, begin, less); return; }
            if (n == 7) { smallsort::sort7(begin, begin, less); return; }
            if (n == 8) { smallsort::sort8(begin, begin, less); return; }
            if (n == 9) { smallsort::sort9(begin, begin, less); return; }
#endif

            msort(begin + q, end, temp, less);
            msort(begin, begin + q, begin + r, less);
            merge(temp, r, begin, end, less);
        }
    }





    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        using t = typename std::iterator_traits<iter>::value_type;
        index n = end - begin;
        index tempsize = n - n / 2;
        std::vector<t> temp;
        temp.resize(tempsize);
        msortOut(begin, end, temp.begin(), less);
    }


    template<typename T>
    void sort(std::vector<T> &v) {
        mergesort::sort(v.begin(), v.end(), std::less<T>());
    }



}