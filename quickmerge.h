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

namespace quickmerge {

    int base_thr;

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
                //std::cout << "error as position " << it - begin << " value: " <<*it << " previous value: " << *(it-1) << std::endl;
                std::cout << "error as position " << it - begin << std::endl;

                error = true;
            }
        }
        //	std::cout << std::endl;
        if (error) {
            //print(begin, end);
        }
    }


    template<typename iter, typename Compare>
    inline void sorttest(iter begin, iter end, Compare less, std::string name) {
        int num_errors = 0;
        for (iter it = begin + 1; it < end; it++)
        {
            //	std::cout << ", " << *it; // << std::endl;
            if (less(*it, *(it - 1)))
                num_errors++;
  //              std::cout << name << " : error as position " << it - begin << std::endl;
            //				std::cout <<"seed: "<<seedg<< ", " << name << " : error as position " << it - begin << std::endl;
        }
        if(num_errors > 0)
            std::cout << name << " : error:  " << num_errors << std::endl;
        //	std::cout << std::endl;
    }


    template<typename iter, typename Compare>
    inline void merge(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*i2);
        iter ires = i2;
        i2 = i2 + num;
        assert(num >= 1 && endtarget - i2 >= 1);
        if ((num < 1 || endtarget - i2 < 1))
            std::cout << "ERROR: merge: one part empty" << std::endl;
        iter end1 = i1 + num;
#ifdef MEASURE_SCANS
        scanned_elements += 3;
#endif // MEASURE_SCANS
        while (i1 < end1 && i2 < endtarget) {
            //			std::cout << " merge: " << end1 -i1 << " i2: " << endtarget - i2<< std::endl;
            bool smaller = less(*i1, *i2);    // tempit = (*i1 < *i2) ? i1++ : i2++;
                                              //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
            iter tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
        if (i1 < end1)
        {
            while (i1 < end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
#ifdef MEASURE_SCANS
                scanned_elements += 2;
#endif // MEASURE_SCANS
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
        else
        {
            *ires = std::move(*(i1 - 1));
            *(i1 - 1) = std::move(temp);
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
    }




    template<typename iter, typename Compare>
    inline void merge____(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*i2);
        iter ires = i2;
        i2 = i2 + num;
        assert(num >= 1 && endtarget - i2 >= 1);
        if ((num < 1 || endtarget - i2 < 1))
            std::cout << "ERROR: merge: one part empty" << std::endl;
        iter end1 = i1 + num;
#ifdef MEASURE_SCANS
        scanned_elements += 3;
#endif // MEASURE_SCANS
        while (i1 < end1 && i2 < endtarget) {
            //			std::cout << " merge: " << end1 -i1 << " i2: " << endtarget - i2<< std::endl;
            bool smaller = less(*i1, *i2);    // tempit = (*i1 < *i2) ? i1++ : i2++;
            
        /*    if (smaller)
                *ires++ = std::move(*i1);
            else
                *ires++ = std::move(*i2);
            if (smaller)
                *i1 = std::move(*ires);
            else
                *i2 = std::move(*ires);
*/
            iter z;
            if (smaller) {
                z = i1;
                ++i1;
            }
            else {
                z = i2;
                ++i2;
            }

            //std::iter_swap(z, ires++);
            *ires++ =  std::move(*z);
            *z = std::move(*ires);


            //iter tempit = smaller ? i1 : i2;
   /*         i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;*/
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
        if (i1 < end1)
        {
            while (i1 < end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
#ifdef MEASURE_SCANS
                scanned_elements += 2;
#endif // MEASURE_SCANS
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
        else
        {
            *ires = std::move(*(i1 - 1));
            *(i1 - 1) = std::move(temp);
#ifdef MEASURE_SCANS
            scanned_elements += 2;
#endif // MEASURE_SCANS
        }
    }



    //needs the guarantee that source and target arrays do not overlap
    template<typename iter, typename Compare>
    inline void two_way_merge(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*i2);
        iter ires = i2;
        i2 = i2 + num;
        iter end1 = i1 + num;
        while (i1 < end1 && i2 < endtarget) {
            bool smaller = (*i1 < *i2);    // tempit = (*i1 < *i2) ? i1++ : i2++;
                                           //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
            iter tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
        }
        if (i1 != end1)
        {
            while (i1 != end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
        }
        else
            *(i1 - 1) = std::move(temp);
    }



    template<typename fiter, typename Compare>
    inline void merge_backward(fiter fend1, int num, fiter fend2, fiter fbegintarget, Compare less) {
        typedef std::reverse_iterator<fiter> iter;
        using t = typename std::iterator_traits<iter>::value_type;
        iter i1(fend1);
        iter i2(fend2);
        iter endtarget(fbegintarget);
        t temp = std::move(*i2);
        iter ires = i2;
        i2 = i2 + num;
        iter end1 = i1 + num;
        while (i1 < end1 && i2 < endtarget) {
            bool smaller = (*i1 > *i2);    // tempit = (*i1 < *i2) ? i1++ : i2++;
                                           //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
            iter tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
        }
        if (i1 != end1)
        {
            while (i1 != end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
        }
        else
            *(i1 - 1) = std::move(temp);

    }


    template<typename iter, typename Compare>
    inline void merge_arr(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        //	typedef typename std::iterator_traits<iter>::difference_type index;
        t temp = std::move(*i2);
        iter its[2];
        iter ires = i2;
        i2 = i2 + num;
        iter end1 = i1 + num;
        its[0] = i1;
        its[1] = i2;
        //iter & tempit;
        while (its[0] != end1 && its[1] != endtarget) {
            //bool smaller = (*(its[0]) < *(its[1]));    // tempit = (*i1 < *i2) ? i1++ : i2++;
            iter & tempit = its[(*(its[1]) < *(its[0]))]; // i2 + smaller * (i1 - i2);
                                                          //iter tempit = its[1] + smaller * (its[0] - its[1]);
                                                          //its[0] = its[0] + smaller;
                                                          //its[1] = its[1] + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit++ = std::move(*ires);
        }
        i1 = its[0];
        if (i1 != end1)
        {
            while (i1 != end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
        }
        else
            *(i1 - 1) = std::move(temp);
    }





    template<typename iter, typename Compare>
    inline void merge_l(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*i2);
        iter its[2];
        iter ires = i2;
        i2 = i2 + num;
        iter end1 = i1 + num;
        its[0] = i1;
        its[1] = i2;
        //iter & tempit;
        while (its[0] != end1 && its[1] != endtarget) {
            bool smaller = (*(its[1]) < *(its[0]));
            *ires++ = std::move(*(its[smaller]));
            *(its[smaller])++ = std::move(*ires);
        }
        i1 = its[0];
        while (i1 < end1) {
            *ires++ = std::move(*i1);
            *i1++ = std::move(*ires);
        }
        *(i1 - 1) = std::move(temp);
    }

    template<typename iter, typename Compare>
    inline void merge__(iter i1, int num, iter i2, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*i2);
        unsigned char index1[256], index2[256];
        iter its[2];
        iter ires = i2;
        i2 = i2 + num;
        iter end1 = i1 + num;
        its[0] = i1;
        its[1] = i2;
        //iter & tempit;

        while (i1 + 256 < end1 && i2 + 256 < endtarget) {
            int num1 = 0;
            int num2 = 0;
            for (int i = 0; i < 256; i++) {
                bool smaller = (i2[num2] < i1[num1]);
                index1[num1] = i;
                index2[num2] = i;
                num1 += (1 - smaller);
                num2 += smaller;
            }
            *ires = std::move(ires[index1[0]]);
            for (int i = 0; i < num1 - 1; i++) {
                ires[index1[i]] = std::move(i1[i]);
                i1[i] = std::move(ires[index1[i + 1]]);
            }
            ires[index1[num1 - 1]] = std::move(i1[num1 - 1]);
            i1[num1 - 1] = std::move(ires[index2[0]]);
            for (int i = 0; i < num2 - 1; i++) {
                ires[index2[i]] = std::move(i2[i]);
                i2[i] = std::move(ires[index2[i + 1]]);
            }
            i2[num2 - 1] = std::move(ires[256]);
            i1 += num1;
            i2 += num2;
            ires += 256;
        }
        its[0] = i1;
        its[1] = i2;
        while (its[0] != end1 && its[1] != endtarget) {
            bool smaller = (*(its[1]) < *(its[0]));
            *ires++ = std::move(*(its[smaller]));
            *(its[smaller])++ = std::move(*ires);
        }
        i1 = its[0];
        while (i1 < end1) {
            *ires++ = std::move(*i1);
            *i1++ = std::move(*ires);
        }
        *(i1 - 1) = std::move(temp);
        /*
        if (i1 != end1)
        {
        while (i1 != end1 - 1) {
        *ires++ = *i1;
        *i1++ = *ires;
        }
        *ires++ = *i1;
        *(i1) = temp;
        }
        else
        {
        *(its[1]) = temp;
        }
        */
    }

    template<typename iter, typename Compare>
    struct Standard_sorter{
        static void sort(iter begin, iter end, Compare less)
        {
            std::sort(begin, end, less);
        }

    };



    // [begin, begin + n - n / 2), [target, target + n) must be disjoint
    template<bool base_cases = false, template<class, class> class base_sorter = Standard_sorter, typename iter, typename Compare>
    inline void msort(iter begin, iter end, iter target, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        using t = typename std::iterator_traits<iter>::value_type;
        index n = end - begin;

        if (n == 1) { std::swap(begin[0], target[0]); return; }

        if (base_cases && n <= base_thr) {
            base_sorter<iter, Compare>::sort(begin, end, less);
            std::swap_ranges(begin, end, target);
            return;
        }


        /*       std::cout << "beginin n:" << n << std::endl;
        print(begin, end);
        */

        if (n <= INS_SIZE && (target>= end || target + n <= begin))
        {
            t buffer[INS_SIZE];
            std::move(begin, end, buffer);
            stl_gcc::insertion_sort(buffer, buffer + n, less);
            std::move(target, target + n, begin);
            std::move(buffer, buffer + n, target);
            return;
            /*           std::cout << "nach inssort n:" << n << std::endl;
            print(begin, end);
            std::cout << std::endl;

            sorttest(target, target + n, less, "testing after second stage of insertionsort length: " + std::to_string(n));
            */
        }

#ifdef BASE
        if (n == 5) { smallsort::sort5(begin, target, less); return; }
        if (n == 6) { smallsort::sort6(begin, target, less); return; }
        if (n == 7) { smallsort::sort7(begin, target, less); return; }
        if (n == 8) { smallsort::sort8(begin, target, less); return; }
        if (n == 9) { smallsort::sort9(begin, target, less); return; }

        if (n == 4) { smallsort::sort4_target_swap(begin, target, less);  return; }
        if (n == 3) { smallsort::sort3_target_swap(begin, target, less);  return; }
        if (n == 2) { smallsort::sort2_target_swap(begin, target, less);  return; }
#endif

        //        if (n <= 4) { std::cout << "small merge" << end - begin << std::endl; }
        index q = n / 2;
        /*		msort<Partitioner>(begin + q, end, target + q, less);
        msort<Partitioner>(begin, begin + q, begin + q, less);
        merge(begin + q, q, target, target + n, less);*/



        msort<base_cases, base_sorter>(begin, end - q, target + q, less);
        msort<base_cases, base_sorter>(end - q, end, begin, less);
        //        		std::cout << "vor merge" << end - begin << std::endl;
        //       print(begin, end);
        merge(begin, q, target, target + n, less);
        //      		std::cout << "nach merge" << end - begin << std::endl;
        //              print(target, target + n);
        //             std::cout << std::endl;
        //merge(begin, q, target, target + n, less);
    }




    template<bool base_cases = false, template<class, class> class base_sorter = Standard_sorter, typename iter, typename Compare>
    inline void msortOut(iter begin, iter end, iter temp, unsigned int max_temp, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index n = end - begin;
        if (n > 10) {
            index r = std::min((unsigned int)n / 2, max_temp);
            //std::cout << "asdf"<< end - begin << std::endl;
            index q = n - r;
            if (max_temp < r || r < (n + 2) / 3)
                std::cout << "maxtemp too small:" << max_temp << ", " << end - begin << std::endl;
            msort<base_cases, base_sorter>(begin + q, end, temp, less);
            msort<base_cases, base_sorter>(begin, begin + q, begin + r, less);

            merge(temp, r, begin, end, less);
        }
        else {
            std::cout << "asdf big mistake in msortOut with max_temp" << std::endl;
        }
    }

    template<bool base_cases = false, template<class, class> class base_sorter = Standard_sorter, typename iter, typename Compare>
    inline void msortOut(iter begin, iter end, iter temp, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index n = end - begin;
        if (n > 1) {
            index q;
            q = n / 2;
            index r = n - q;

            msort<base_cases, base_sorter>(begin + q, end, temp, less);
            msort<base_cases, base_sorter>(begin, begin + q, begin + r, less);
            merge(temp, r, begin, end, less);
        }
    }

 


    template< bool base_cases = false, template<class, class> class base_sorter = Standard_sorter, template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
    void sort_smaller_merge(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        while (begin + 1 < end) {

            iter pivot;
            index n = end - begin;

            if (n == 2) {
                leanswap(begin, end - 1, less);
                return;
            }
            if (n == 3) {
                leanswap(begin, end - 1, less);
                leanswap(begin + 1, end - 1, less);
                leanswap(begin, begin + 1, less);
                return;
            }

            pivot = Partitioner<iter, Compare>::partition(begin, end, less);
            if (pivot < begin + n / 2) {
                msortOut<base_cases, base_sorter>(begin, pivot, pivot + 1, less);
                begin = pivot + 1;
            }
            else {
                msortOut<base_cases, base_sorter>(pivot + 1, end, begin, less);
                end = pivot;
            }
        }
    }



    template< bool base_cases = false, template<class, class> class base_sorter = Standard_sorter, template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
    void sort_larger_merge(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        while (begin + 1 < end) {
            iter pivot;
            index n = end - begin;

            if (n == 2) {
                leanswap(begin, end - 1, less);
                return;
            }
            if (n == 3) {
                leanswap(begin, end - 1, less);
                leanswap(begin + 1, end - 1, less);
                leanswap(begin, begin + 1, less);
                return;
            }


            pivot = Partitioner<iter, Compare>::partition(begin, end, less);
            index num = pivot - begin;
            if (num <= n / 3 || ((num + 1) / 2 < end - pivot  && num >= (n - 1) / 2)) {
                msortOut<base_cases, base_sorter>(begin, pivot, pivot + 1, less);
                begin = pivot + 1;
            }
            else {
                msortOut<base_cases, base_sorter>(pivot + 1, end, begin, less);
                end = pivot;
            }
        }
    }



}