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
#include "partition_mom.h"

#define WORST_CASE_RANGE_QMS 3


#include "quickmerge.h"

namespace quickmerge {

    template<typename iter>
    inline void easy_shuffle(iter begin, iter end) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        index n = end - begin;
        if (n < 10) return;


        iter it1 = begin + 1;
        iter it2 = end - 1;
        iter it3 = begin + n / 2 - 1;
        iter it4 = begin + n / 2;
        iter it5 = begin + 2;
        int i = 5;
        int j = 3;
        int k = 0;
        int l = 0;
        while (it1 < it2)
        {
            std::iter_swap(it1, it2);
            if (it1 + 10 < end)
                std::iter_swap(it1 - 1, it1 + 10);
            it1++;
            if (it3 > begin + 1 && it4 < end)
            {
                std::iter_swap(it3, it4);
            }
            it1 += i;
            it2 -= j;
            it3 -= j / 2;
            it4 += j / 2;
            i = (i == 2) ? 1 : 2;
            j = ((j * 3) + 2) % 5;
            k++;
            if (it3 < begin)
                it3 = begin;
            if (it4 >= end)
                it4 = end - 1;
            if (k == 5)
            {
                std::iter_swap(it3, it1);
                std::iter_swap(it4, it5);
                i += 4;
                if (it5 < end - 2)  it5++;
            }
            else if (k == 2)
            {
                std::iter_swap(it1, it5);
                std::iter_swap(it3, it2);
                it2++;
                if (it5 < end - 2)  it5++;
            }
            else if (k == 7)
            {
                std::iter_swap(it2, it5);
                std::iter_swap(it1, it4);
                if (it5 < end - 2)  it5++;
            }
            else if (k == 11)
            {
                k = 0;
                i += 3;
                std::iter_swap(it3, it5);
                std::iter_swap(it4, it2);
                if (it5 < end - 2)  it5++;
            }
            l++;
            if (l == 200)
            {
                l = 0;
                it5 = it5 + 11234;
                if (it5 > end)
                {
                    index subt = (it5 - begin) - ((it5 - begin) % n);
                    it5 -= subt;
                }
            }
        }
    }

    //begin1 < begintarget < begin2
    template<typename iter, typename Compare>
    inline void mergeRightdouble(iter begin, iter begin2, iter end, iter endtarget, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        t temp = std::move(*(endtarget - 1));
        iter ires = endtarget - 1;
        iter i1 = begin2 - 1;
        iter i2 = end - 1;
        iter begintarget = endtarget - (end - begin);
        iter tempit;
        while (ires > i2 && i2 >= begin2) {
            //assert(i1 >= begin);
            if (i1 < begin)
                std::cout << "ii1 < begin" << std::endl;
            bool smaller = less(*i2, *i1);    // tempit = (*i1 < *i2) ? i1++ : i2++;
                                              //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
            tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 - smaller;
            i2 = i2 - 1 + smaller;
            *ires-- = std::move(*tempit);
            *tempit = std::move(*ires);
        }

        /*		std::cout << "making sure thtat input 1 still sorted mergeRightdouble " <<  std::endl;
        sorttest(begin, i1 + 1, less);
        std::cout << "making sure thtat input 2 still sorted mergeRightdouble " << std::endl;
        sorttest(begin2, i2 + 1, less);


        std::cout << "testing after first stage of mergeRightdouble" << std::endl;
        sorttest(ires + 1, endtarget, less);
        */
        if (i2 < begin2) {

            while (i1 > begin) {
                *ires-- = std::move(*i1);
                *i1-- = std::move(*ires);
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
            //	std::cout << "testing mergeRightdouble nice case" << std::endl;
            //	sorttest(ires, endtarget, less, "testing mergeRightdouble nice case");
        }
        else {
            if (i2 != ires)
                std::cout << "i2 != ires" << std::endl;

            *ires = std::move(*tempit);
            *tempit = std::move(temp);

            //	std::cout << "testing after first stage of mergeRightdouble and back move position"<< begintarget - begin << std::endl;
            //	sorttest(ires, endtarget, less);

            iter iresf = begintarget;
            if (begintarget >= begin2)
                std::cout << "begintarget >= begin2" << std::endl;

            if (begintarget <= i1)
                std::cout << "begintarget <= i1: " << begintarget - i1 << std::endl;

            temp = std::move(*(begintarget));
            iter i1f = begin;
            iter i2f = begin2;

            while (i1f <= i1 && i2f <= i2) {
                bool smaller = less(*i1f, *i2f);
                tempit = i2f + smaller * (i1f - i2f);
                i1f = i1f + smaller;
                i2f = i2f + 1 - smaller;
                *iresf++ = std::move(*tempit);
                *tempit = std::move(*iresf);
            }

            //			std::cout << "testing after second stage of mergeRightdouble length: " << iresf - begintarget<< std::endl;
            //			sorttest(begintarget, iresf, less,"testing after second stage of mergeRightdouble length: " );

            if (i1f <= i1)
            {
                while (i1f < i1) {
                    *iresf++ = std::move(*i1f);
                    *i1f++ = std::move(*iresf);
                }
                *iresf = std::move(*i1f);
                *(i1f) = std::move(temp);
            }
            else {
                if (tempit != i1)
                    std::cout << "tempit != i1" << std::endl;
                //one element too much has been moved
                *iresf = std::move(*tempit);
                *tempit = std::move(temp);

            }
        }
    }

    template<typename iter, typename Compare>
    inline void mergeLeft(iter begin1, iter begin2, iter end2, iter target, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        /*		print(target, begin1);
        print(begin1, begin2);
        print(begin2, end2);*/

        t temp = std::move(*target);
        iter ires = target;
        iter i1 = begin1;
        iter i2 = begin2;
        iter end1 = begin2;
        while (i1 < end1 && i2 < end2) {
            bool smaller = less(*i1, *i2);
            iter tempit = i2 + smaller * (i1 - i2);
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
        }



        //		std::cout << "testing after first stage of mergeLeft" << std::endl;
        //		sorttest(target, ires, less);
        //		print(target, ires);

        if (i1 < end1)
        {
            while (i1 < end1 - 1) {
                *ires++ = std::move(*i1);
                *i1++ = std::move(*ires);
            }
            *ires = std::move(*i1);
            *(i1) = std::move(temp);
            assert(ires == target + (end2 - begin1) - 1);
            //			std::cout << "testing after second stage of mergeLeft with i1 copy" << std::endl;
            //			sorttest(target, target + (end2 - begin1), less);
        }
        else
        {
            while (i2 < end2 - 1) {
                *ires++ = std::move(*i2);
                *i2++ = std::move(*ires);
            }
            *ires = std::move(*i2);
            *(i2) = std::move(temp);
            //			std::cout << "testing after second stage of mergeLeft with i2 copy" << std::endl;
            //			sorttest(target, target + (end2 - begin1), less);
        }
    }



    //begin2 < endtarget < end
    //begin - target >= (end - begin2 + 1) / 2
    template<typename iter, typename Compare>
    inline void mergeLeftdouble(iter begin, iter begin2, iter end, iter target, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;

        t temp = std::move(*(target));
        iter ires = target;
        iter i1 = begin;
        iter i2 = begin2;
        iter end1 = begin2;
        iter end2 = end;
        iter endtarget = target + (end - begin);
        iter tempit;
        if (begin - target < (end - begin2 + 1) / 2)
            std::cout << "begin - target: " << begin - target << ", (end - begin2 + 1)" << (end - begin2 + 1) << std::endl;
        if (begin2 >= endtarget || endtarget >= end)
            std::cout << "begin2 - endtarget: " << begin2 - endtarget << ", endtarget - end " << endtarget - end << std::endl;


        //	assert(begin - target >= (end - begin2 + 1) / 2);
        //	assert(begin2 < endtarget &&  endtarget < end);
        while (ires < i1 && i1 < end1) {
            //assert(i1 >= begin);
            if (i2 >= end2)
                std::cout << "i2 < begin" << std::endl;
            bool smaller = less(*i1, *i2);
            tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
        }
        /*
        //				std::cout << "making sure thtat input 1 still sorted mergeLeftdouble " <<  std::endl;
        sorttest(i1, end1, less);
        //		std::cout << "making sure thtat input 2 still sorted mergeLeftdouble " << std::endl;
        sorttest(i2, end2, less);


        //		std::cout << "testing after first stage of mergeLeftdouble" << std::endl;
        sorttest(target,  ires, less);
        */
        if (i1 == end1) {

            while (i2 < end - 1) {
                *ires++ = std::move(*i2);
                *i2++ = std::move(*ires);
            }
            *ires = std::move(*i2);
            *(i2) = std::move(temp);
            //		std::cout << "testing mergLeftdouble nice case" << std::endl;
            //		sorttest(ires, endtarget, less, "testing mergLeftdouble nice case");
        }
        else {
            if (i1 != ires)
                std::cout << "mergeLeftdouble: i1 != ires" << std::endl;

            *ires = std::move(*tempit);
            *tempit = std::move(temp);

            //	std::cout << "testing after first stage of mergeRightdouble and back move position"<< begintarget - begin << std::endl;
            //				sorttest(target, ires, less, "testing after first stage of mergeLeftdouble and back move position");

            iter iresb = endtarget - 1;
            if (endtarget < end1)
                std::cout << "endtarget < end1" << std::endl;

            if (endtarget > i2)
                std::cout << "endtarget > i2: " << endtarget - i2 << std::endl;

            temp = std::move(*(endtarget - 1));
            iter i1b = end1 - 1;
            iter i2b = end2 - 1;

            while (i1b >= i1 && i2b >= i2) {
                bool smaller = less(*i2b, *i1b);
                tempit = i2b + smaller * (i1b - i2b);
                i1b = i1b - smaller;
                i2b = i2b - 1 + smaller;
                *iresb-- = std::move(*tempit);
                *tempit = std::move(*iresb);
            }

            //			std::cout << "testing after second stage of mergeRightdouble length: " << iresb - begintarget<< std::endl;
            //			sorttest(iresb + 1, endtarget, less, "testing after second stage of mergeLeftdouble length: " + std::to_string(endtarget - iresb));
            //			std::cout << "i2b - i2: " << i2b - i2 << ", begin - target: " << begin - target << ", iresb - begin: " << iresb - begin << std::endl;
            if (i2b >= i2)
            {
                while (i2b > i2) {
                    *iresb-- = std::move(*i2b);
                    *i2b-- = std::move(*iresb);
                }
                *iresb = std::move(*i2b);
                *(i2b) = std::move(temp);
            }
            else {
                //			std::cout << "asdfasdf  i2b - i2: " << i2b - i2 << ", begin - target: " << begin - target << ", iresb - begin: " << iresb - begin << std::endl;

                if (tempit != i2)
                    std::cout << "tempit != i2" << std::endl;
                //one element too much has been moved
                *iresb = std::move(*tempit);
                *tempit = std::move(temp);

            }
            //			sorttest(target, endtarget, less, "testing after of mergeLeftdouble ");

        }
    }


#ifdef blub
    //begin2 < endtarget < end
    //begin - target >= (begin2 - begin + 1) / 2 - 30
    template<typename iter, typename Compare>
    inline void mergeLeftdoubleBuffer(iter begin, iter begin2, iter end, iter target, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        typedef typename std::iterator_traits<iter>::difference_type index;
        t temp = std::move(*(target));
        const int buffersize = 30;
        t buffer[buffersize];
        iter ires = target;
        iter i1 = begin;
        iter i2 = begin2;
        iter end1 = begin2;
        iter end2 = end;
        iter endtarget = target + (end - begin);
        iter tempit;
        while (ires < i1 && i1 < end1) {
            if (i2 >= end2)
                std::cout << "i2 < begin" << std::endl;
            bool smaller = (*i2 > *i1);
            tempit = i2 + smaller * (i1 - i2);
            //iter tempit = smaller ? i1 : i2;
            i1 = i1 + smaller;
            i2 = i2 + 1 - smaller;
            *ires++ = std::move(*tempit);
            *tempit = std::move(*ires);
        }

        /*		std::cout << "making sure thtat input 1 still sorted mergeRightdouble " <<  std::endl;
        sorttest(begin, i1 + 1, less);
        std::cout << "making sure thtat input 2 still sorted mergeRightdouble " << std::endl;
        sorttest(begin2, i2 + 1, less);


        std::cout << "testing after first stage of mergeRightdouble" << std::endl;
        sorttest(ires + 1, endtarget, less);
        */
        if (i1 == end1) {

            while (i2 < end - 1) {
                *ires++ = std::move(*i2);
                *i2++ = std::move(*ires);
            }
            *ires = std::move(*i2);
            *(i2) = std::move(temp);
            //		std::cout << "testing mergLeftdouble nice case" << std::endl;
            sorttest(ires, endtarget, less, "testing mergLeftdouble nice case");
        }
        else {
            if (i1 != ires)
                std::cout << "mergeLeftdouble: i1 != ires" << std::endl;

            *ires = std::move(*tempit);
            *tempit = std::move(temp);

            //	std::cout << "testing after first stage of mergeRightdouble and back move position"<< begintarget - begin << std::endl;
            sorttest(target, ires, less, "testing after first stage of mergeRightdouble and back move position");

            iter iresb = endtarget - 1;
            if (endtarget < end1)
                std::cout << "endtarget < end1" << std::endl;

            if (endtarget > i2)
                std::cout << "begintarget <= i1" << std::endl;


            iter i1b = end1 - 1;
            iter i2b = end2 - 1;


            if (iresb - i2 + 1 > buffersize)
                std::cout << "resb - i2 + 1 > buffersize" << std::endl;
            iter endholes = end;
            //	iter endempty2 = i2b + 1;

            index tempsize = iresb - i2 + 1;
            for (int i = 0; i < tempsize; i++)
            {
                buffer[i] = std::move(*iresb);
                iresb--;
            }


            while (i1b >= i1 && i < buffersize) {
                bool smaller = (*i1b > buffer[i]);
                tempit = i2b + smaller * (i1b - i2b);
                i1b = i1b - smaller;
                i2b = i2b - 1 + smaller;
                *iresb-- = std::move(*tempit);
                *tempit = std::move(*iresb);
            }

            temp = std::move(*(iresb));


            while (i1b >= i1 && i2b >= i2) {
                bool smaller = (*i1b > *i2b);
                tempit = i2b + smaller * (i1b - i2b);
                i1b = i1b - smaller;
                i2b = i2b - 1 + smaller;
                *iresb-- = std::move(*tempit);
                *tempit = std::move(*iresb);
            }



            //			std::cout << "testing after second stage of mergeRightdouble length: " << iresb - begintarget<< std::endl;
            //			sorttest(begintarget, iresb, less);

            if (i2b >= i2)
            {
                while (i2b > i2) {
                    *iresb-- = std::move(*i2b);
                    *i2b-- = std::move(*iresb);
                }
                *iresb = std::move(*i2b);
                *(i2b) = std::move(temp);
            }
            else {
                if (tempit != i2)
                    std::cout << "tempit != i2" << std::endl;
                //one element too much has been moved
                *iresb = std::move(*tempit);
                *tempit = std::move(temp);

            }



        }
    }
}



//begin2 < endtarget < end
//begin - target >= (begin2 - begin + 1) / 2 - 30
template<typename iter, typename Compare>
inline void mergeLeftdoubleBuffer(iter begin, iter begin2, iter end, iter target, Compare less) {
    using t = typename std::iterator_traits<iter>::value_type;
    typedef typename std::iterator_traits<iter>::difference_type index;
    t temp = std::move(*(target));
    const int buffersize = 30;
    t buffer[buffersize];
    iter ires = target;
    iter i1 = begin;
    iter i2 = begin2;
    iter end1 = begin2;
    iter end2 = end;
    iter endtarget = target + (end - begin);
    iter tempit;
    while (ires < i1 && i1 < end1) {
        if (i2 >= end2)
            std::cout << "i2 < begin" << std::endl;
        bool smaller = (*i2 > *i1);
        tempit = i2 + smaller * (i1 - i2);
        //iter tempit = smaller ? i1 : i2;
        i1 = i1 + smaller;
        i2 = i2 + 1 - smaller;
        *ires++ = std::move(*tempit);
        *tempit = std::move(*ires);
    }

    /*		std::cout << "making sure thtat input 1 still sorted mergeRightdouble " <<  std::endl;
    sorttest(begin, i1 + 1, less);
    std::cout << "making sure thtat input 2 still sorted mergeRightdouble " << std::endl;
    sorttest(begin2, i2 + 1, less);


    std::cout << "testing after first stage of mergeRightdouble" << std::endl;
    sorttest(ires + 1, endtarget, less);
    */
    if (i1 == end1) {

        while (i2 < end - 1) {
            *ires++ = std::move(*i2);
            *i2++ = std::move(*ires);
        }
        *ires = std::move(*i2);
        *(i2) = std::move(temp);
        //		std::cout << "testing mergLeftdouble nice case" << std::endl;
        sorttest(ires, endtarget, less, "testing mergLeftdouble nice case");
    }
    else {
        if (i1 != ires)
            std::cout << "mergeLeftdouble: i1 != ires" << std::endl;

        *ires = std::move(*tempit);
        *tempit = std::move(temp);

        //	std::cout << "testing after first stage of mergeRightdouble and back move position"<< begintarget - begin << std::endl;
        sorttest(target, ires, less, "testing after first stage of mergeRightdouble and back move position");

        iter iresb = endtarget - 1;
        if (endtarget < end1)
            std::cout << "endtarget < end1" << std::endl;

        if (endtarget > i2)
            std::cout << "begintarget <= i1" << std::endl;


        iter i1b = end1 - 1;
        iter i2b = end2 - 1;


        if (iresb - i2 + 1 > buffersize)
            std::cout << "resb - i2 + 1 > buffersize" << std::endl;
        iter endholes = end;
        //	iter endempty2 = i2b + 1;

        index tempsize = iresb - i2 + 1;
        bool earlybreak = false;

        for (int i = 0; i < tempsize; i++)
        {
            if (i1b < i1 || i2b < i2)
            {
                earlybreak = true;
                break;
            }

            bool smaller = (*i1b > *i2b);
            tempit = i2b + smaller * (i1b - i2b);
            i1b = i1b - smaller;
            i2b = i2b - 1 + smaller;
            iresb--;
            buffer[i] = std::move(*tempit);
        }
        if (earlybreak)
        {
            if (i2b >= i2)
            {
                while (i2b > i2) {
                    *iresb-- = std::move(*i2b);
                    *i2b-- = std::move(*iresb);
                }
                *iresb = std::move(*i2b);
                *(i2b) = std::move(temp);
            }
            else {
                if (tempit != i2)
                    std::cout << "tempit != i2" << std::endl;
                //one element too much has been moved
                *iresb = std::move(*tempit);
                *tempit = std::move(temp);

            }
        }
        else {
            iter startempty1 = i1b + 1;
            iter startempty2 = i2b + 1;
            temp = std::move(*(ires));


            while (i1b >= i1 && i2b >= i2) {
                bool smaller = (*i1b > *i2b);
                tempit = i2b + smaller * (i1b - i2b);
                i1b = i1b - smaller;
                i2b = i2b - 1 + smaller;
                *iresb-- = std::move(*tempit);
                *tempit = std::move(*iresb);
            }



            //			std::cout << "testing after second stage of mergeRightdouble length: " << iresb - begintarget<< std::endl;
            //			sorttest(begintarget, iresb, less);

            if (i2b >= i2)
            {
                while (i2b > i2) {
                    *iresb-- = std::move(*i2b);
                    *i2b-- = std::move(*iresb);
                }
                *iresb = std::move(*i2b);
                *(i2b) = std::move(temp);
            }
            else {
                if (tempit != i2)
                    std::cout << "tempit != i2" << std::endl;
                //one element too much has been moved
                *iresb = std::move(*tempit);
                *tempit = std::move(temp);

            }

            //write buffer back
            int i = 0;
            iter ittarget = endtarget - 1;
            for (iter empty1 = endempty1 - 1; empty1 >= startempty1; empty1--, ttarget--)
            {
                *empty1 = std::move(*ittarget);
                *ittarget = std::move(buffer[i++]);
            }
            for (iter empty2 = endempty2 - 1; empty2 >= startempty2; empty2--, ttarget--)
            {
                *empty2 = std::move(*ittarget);
                *ittarget = std::move(buffer[i++]);
            }
        }
    }
}
}
#endif

//only if temporary space > (end - begin) / 2
// target on the left
template<typename iter, typename Compare>
inline void mergeRight(iter begin1, iter begin2, iter end2, iter endtarget, Compare less) {
    using t = typename std::iterator_traits<iter>::value_type;
    t temp = std::move(*(endtarget - 1));
    iter ires = endtarget - 1;
    iter i1 = begin2 - 1;
    iter i2 = end2 - 1;
    //	iter end1 = begin2;
    //	iter begintarget = endtarget - (end2 - begin1);
    while (i1 >= begin1 && i2 >= begin2) {
        bool smaller = less(*i2, *i1);
        iter tempit = i2 + smaller * (i1 - i2);
        i1 = i1 - smaller;
        i2 = i2 - 1 + smaller;
        *ires-- = std::move(*tempit);
        *tempit = std::move(*ires);
    }
    if (i1 >= begin1)
    {
        while (i1 > begin1) {
            *ires-- = std::move(*i1);
            *i1-- = std::move(*ires);
        }
        *ires = std::move(*i1);
        *(i1) = std::move(temp);
    }
    else
    {
        while (i2 > begin2) {
            *ires-- = std::move(*i2);
            *i2-- = std::move(*ires);
        }
        *ires = std::move(*i2);
        *(i2) = std::move(temp);
    }
}


//only if temporary space > (end - begin) / 2
// target on the left
template<typename iter, typename Compare>
inline void msortLeft(iter begin, iter end, iter target, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        q = n / 2;
        index r = n - q;
        msortOut(begin + r, end, target, less);
        msortOut(begin, begin + r, target, less);

        //			std::cout << "testing left part of msortLeft" << std::endl;
        //			sorttest(begin, begin + r, less);
        //			std::cout << "testing right part of msortLeft" << std::endl;
        //			sorttest(begin + r, begin + n, less);
        mergeLeft(begin, begin + r, end, target, less);
    }
}

// if temporary space <= (end - begin) / 2
// target on the right
template<typename iter, typename Compare>
inline void msortRight(iter begin, iter end, iter endtarget, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        q = n / 2;
        msortOut(begin + q, end, end, less);
        msortOut(begin, begin + q, end, less);
        mergeRight(begin, begin + q, end, endtarget, less);
    }
}

template<typename iter, typename Compare>
inline void msortOutRight(iter begin, iter end, iter endtemp, Compare less);
template<typename iter, typename Compare>
inline void msortOutLeft(iter begin, iter end, iter temp, Compare less);


//if temporary space <= (end - begin) / 2
// target on the left
template<typename iter, typename Compare>
inline void msortRightSmallTemp(iter begin, iter end, iter endtarget, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        index tempsize = endtarget - end;
        q = n / 2;

        index leftsize = std::max(tempsize + 1, q);
        leftsize = std::min(leftsize, 2 * tempsize - 1);
        index rightsize = n - leftsize;





        if (tempsize <= rightsize / 3)
            msortOutRight(begin + leftsize, end, endtarget, less);
        else
            msortOut(begin + leftsize, end, end, tempsize, less);


        //
        msortOut(begin, begin + leftsize, end, tempsize, less);
        mergeRightdouble(begin, begin + leftsize, end, endtarget, less);
    }
}


//only if temporary space <= (end - begin) / 2
// target on the left
template<typename iter, typename Compare>
inline void msortLeftSmallTemp(iter begin, iter end, iter target, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        q = n / 2;
        index tempsize = begin - target;
        index rightsize = std::max(tempsize + 1, q);
        rightsize = std::min(rightsize, 2 * tempsize - 1);
        //			if (rightsize >= q)
        //				std::cout << "rightsize > q in msortLeftSmallTemp: " << rightsize - q << std::endl;
        index leftsize = n - rightsize;

        //			std::cout << "target: " << target - begin << ", leftsize: " << leftsize << ", end: " << end - begin << std::endl;



        msortOut(begin + leftsize, end, target, tempsize, less);

        if (tempsize <= leftsize / 3)
            msortOutLeft(begin, begin + leftsize, target, less);
        else
            msortOut(begin, begin + leftsize, target, tempsize, less);




        //				std::cout << "testing left part of msortOutLeft" << std::endl;
        //			sorttest(begin + leftsize, end, less);
        //				std::cout << "testing right part of msortOutLeft" << std::endl;
        //			sorttest(begin, begin + leftsize, less);

        mergeLeftdouble(begin, begin + leftsize, end, target, less);
    }
}



// temporary space located on the left
template<typename iter, typename Compare>
inline void msortOutLeft(iter begin, iter end, iter temp, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        q = n / 2;
        index r = n - q;
        index tempsize = begin - temp;


        if (tempsize > (r + 1) / 2) { //if there is enough temporary space

            msortLeft(begin, begin + q, temp, less);
            msortLeft(begin + q, end, temp + q, less);

            //				std::cout << "testing left part of msortOutLeft" << std::endl;
            //				sorttest(temp, temp + q, less);
            //				std::cout << "testing right part of msortOutLeft" << std::endl;
            //				sorttest(temp + q, temp + n, less);
            mergeRightdouble(temp, temp + q, temp + n, end, less);
        }
        else
        {
            index leftsize = std::min(q, 2 * tempsize - 1);

            //			codecounter++;
            msortLeft(begin, begin + leftsize, temp, less);
            msortLeftSmallTemp(begin + leftsize, end, temp + leftsize, less);


            //				sorttest(temp, temp + leftsize, less, "testing left part of msortOutLeft");
            //				sorttest(temp + leftsize, temp + n, less, "testing right part of msortOutLeft SMALLTEMP");
            //	std::cout << "testing interesting case SMALLTEMP" << std::endl;

            mergeRightdouble(temp, temp + leftsize, temp + n, end, less);


            //		sorttest(begin, end, less, "testing interesting case SMALLTEMP end");

        }
#ifdef SORT_TEST
        std::cout << "testing whole msortOutLeft" << std::endl;
        sorttest(begin, end, less);
#endif
    }
}



// temporary space located on the right
template<typename iter, typename Compare>
inline void msortOutRight(iter begin, iter end, iter endtemp, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    index n = end - begin;
    if (n > 1) {
        index q;
        q = n / 2;
        index r = n - q;
        index tempsize = endtemp - end;


        if (tempsize > (r + 1) / 2) { //if there is enough temporary space
            msortRight(end - q, end, endtemp, less);
            msortRight(begin, end - q, endtemp - q, less);


            //				std::cout << "testing left part of msortOutRight" << std::endl;
            //				sorttest(endtemp - q, endtemp, less);
            //				std::cout << "testing right part of msortOutRight" << std::endl;
            //				sorttest(endtemp - n, endtemp - q, less);

            mergeLeftdouble(endtemp - n, endtemp - q, endtemp, begin, less);
        }
        else
        {
            //		codecounter++;
            index rightsize = std::min(q, 2 * tempsize - 1);
            //index leftsize = n - rightsize;
            msortRight(end - rightsize, end, endtemp, less);
            msortRightSmallTemp(begin, end - rightsize, endtemp - rightsize, less);

            //				std::cout << "testing left part of msortOutRight" << std::endl;
            //				sorttest(endtemp - rightsize, endtemp, less);
            //				std::cout << "testing right part of msortOutRight" << std::endl;
            //				sorttest(endtemp - n, endtemp - rightsize, less);

            mergeLeftdouble(endtemp - n, endtemp - rightsize, endtemp, begin, less);
        }
        //	std::cout << "testing whole msortOutRight" << std::endl;
        //	sorttest(begin, end, less);
    }
}






template<template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
void sort_worst_case_efficient(iter begin, iter end, Compare less) {
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

        if (n > 100) //Hoare_block_partition_median_of_medians_of_3
        {
            pivot = partition::Stl_partition_median_of_medians_of_3<iter, Compare>::partition(begin, end, less);
            index num = pivot - begin;
            //std::cout<<"l: "<< (pivot - begin)<<", r: "<<(end - pivot - 1)<<std::endl;
            if (num > n / 2) {
                //if (pivot < begin + n / 2) {
                if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                begin = pivot + 1;
            }
            else {
                if (pivot - begin <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                msortOut(pivot + 1, end, begin, pivot - begin, less);
                end = pivot;
            }
        }
        else
        {
            pivot = Partitioner<iter, Compare>::partition(begin, end, less);
            //         index num = pivot - begin;
                     //	if (num >  (end-pivot) / 2) {
            if (pivot < begin + n / 2) {
                //msortOut(begin, pivot, pivot + 1,num, less);
                msortOut(begin, pivot, pivot + 1, less);
                begin = pivot + 1;
            }
            else {
                //	msortOut(pivot + 1, end, begin, end - pivot - 1, less);
                msortOut(pivot + 1, end, begin, less);
                end = pivot;
            }

        }


    }
    //	std::sort(begin, end);
}

//#define SHOW_COMPARISONS

template<bool undersampling = false, template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
void sort_worst_case_efficient_3_5(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    bool inner_array = false;
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


        if (n > 200) //Hoare_block_partition_median_of_medians_of_3
        {

#ifdef SHOW_COMPARISONS   
            long long int comps = comparisons;
            iter tbegin = begin;
            iter tend = end;
#endif // SHOW_COMPARISONS

            //pivot = median::median_of_medians_partition_ninther<Partitioner, iter, Compare>(begin, end, 0.5, less);
            int pivot_width;
            pivot = partition::Stl_partition_median_of_medians_of_3_5_left_right<false, iter, Compare, undersampling>::partition(begin, end, less, inner_array, pivot_width);
            index num = pivot - begin;

            index num_right = end - pivot - pivot_width;

#ifdef WORST_CASE_SIMULATION
#ifdef MEASURE_COMPARISONS

            if (worst_case_simulation) {
                std::random_shuffle(begin, pivot);
                std::random_shuffle(pivot + pivot_width, end);
            }
#endif // MEASURE_COMPARISONS
#endif

#ifdef SHOW_COMPARISONS   


            std::cout << "l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
            std::cout << "anz: " << tend - begin << " MoMpartition " << (comparisons - comps) / (double)(tend - begin) << "log: " << std::log2(tend - begin) << std::endl;

            comps = comparisons;

#endif // SHOW_COMPARISONS


            if (pivot_width > n * 0.4 || (pivot_width > n * 0.1 && num < n / 2 && num_right < n / 2))
            {
                //                				std::cout << "l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;

                if (num < num_right)
                {
                    msortOut(begin, pivot, pivot + pivot_width, less);
                    begin = pivot + pivot_width;
                    inner_array = true;
                }
                else
                {
                    //	msortOut(pivot + 1, end, begin, end - pivot - 1, less);
                    msortOut(pivot + pivot_width, end, begin, less);
                    end = pivot;
                }
            }
            else {
                if (num > n / 2) { //sort left part with mergesort

#ifdef WORST_CASE_SIMULATION
//#ifdef MEASURE_COMPARISONS

                    if (worst_case_simulation) {
                        easy_shuffle(begin, pivot);
                    }
                    //#endif // MEASURE_COMPARISONS
#endif

                    if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    {
                        //				std::cout << "msortOutRight l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutRight(begin, pivot, end, less);
#ifdef SORT_TEST
                        sorttest(begin, pivot, less);
#endif
                        begin = pivot;
                        inner_array = true;
                    }
                    else {
                        msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                        begin = pivot + 1;
                        inner_array = true;
                    }

                }
                else {  //sort right part with mergesort 
#ifdef WORST_CASE_SIMULATION
                            //#ifdef MEASURE_COMPARISONS

                    if (worst_case_simulation) {
                        easy_shuffle(pivot + 1, end);
                    }
                    //#endif // MEASURE_COMPARISONS
#endif
                    if (pivot - begin <= (end - begin) / 3 - 2) {
                        // 			std::cout << "msortOutLeft: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutLeft(pivot + 1, end, begin, less);
#ifdef SORT_TEST
                        sorttest(pivot + 1, end, less);
#endif
                        end = pivot + 1;
                    }
                    else
                    {
                        msortOut(pivot + 1, end, begin, pivot - begin, less);
                        end = pivot;
                    }
                }
            }
#ifdef SHOW_COMPARISONS
            std::cout << "anz: " << tend - pivot << " mergesort " << ((comparisons - comps) - std::log2(tend - pivot)*(double)(tend - pivot)) / (tend - pivot) << "log: " << std::log2(tend - pivot) << std::endl;
#endif // SHOW_COMPARISONS

        }
        else
        {
            pivot = Partitioner<iter, Compare>::partition(begin, end, less);
            if (pivot < begin + n / 2) {
                //msortOut(begin, pivot, pivot + 1,num, less);
                msortOut(begin, pivot, pivot + 1, less);
                begin = pivot + 1;
                inner_array = true;
            }
            else {
                //	msortOut(pivot + 1, end, begin, end - pivot - 1, less);
                msortOut(pivot + 1, end, begin, less);
                end = pivot;
            }

        }



    }
    //	std::sort(begin, end);
}


template<template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
void sort_hybrid(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    bool next_linear_sample = false;
    bool inner_array = false;
    while (begin + 1 < end) {
        iter pivot;
        index n = end - begin;
        int pivot_width;

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

        if (n > 200 && next_linear_sample) //Hoare_block_partition_median_of_medians_of_3
        {
#ifdef asdf 
            pivot = partition::Hoare_block_partition_median_of_medians_of_3<iter, Compare>::partition(begin, end, less);
            index num = pivot - begin;
            //		std::cout<<"l: "<< (pivot - begin)<<", r: "<<(end - pivot - 1)<<std::endl;

            if (num > n / 2) {
                if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;

                msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                begin = pivot + 1;
            }
            else {
                if (pivot - begin <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;

                msortOut(pivot + 1, end, begin, pivot - begin, less);
                end = pivot;
            }
            next_linear_sample = false;
#endif
            //pivot = median::median_of_medians_partition_ninther<Partitioner, iter, Compare>(begin, end, 0.5, less);

#ifdef PARTIAL_SORT_COUNT	    
            partial_sort_count++;
#endif




            pivot = partition::Stl_partition_median_of_medians_of_3_5_left_right<true, iter, Compare>::partition(begin, end, less, inner_array, pivot_width);
            index num = pivot - begin;
            //std::cout<<"l: "<< (pivot - begin)<<", r: "<<(end - pivot - 1)<<std::endl;
            index num_right = end - pivot - pivot_width;
            if ((pivot_width > n / 3) || (7 * num - 5 < n) || (7 * num_right - 5 < n))
            {
                if (num < num_right)
                {
                    msortOut(begin, pivot, pivot + pivot_width, less);
                    begin = pivot + pivot_width;
                    inner_array = true;
                }
                else
                {
                    //	msortOut(pivot + 1, end, begin, end - pivot - 1, less);
                    msortOut(pivot + pivot_width, end, begin, less);
                    end = pivot;
                }
            }
            else {
                if (num > n / 2) {
                    //if (pivot < begin + n / 2) {
                    if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    {
                        //				std::cout << "msortOutRight l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutRight(begin, pivot, end, less);
                        begin = pivot;
                        inner_array = true;
                    }
                    else {
                        msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                        begin = pivot + 1;
                        inner_array = true;
                    }

                }
                else {
                    if (pivot - begin <= (end - begin) / 3 - 2) {
                        //			std::cout << "msortOutLeft: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutLeft(pivot + 1, end, begin, less);
                        end = pivot + 1;
                    }
                    else
                    {
                        msortOut(pivot + 1, end, begin, pivot - begin, less);
                        end = pivot;
                    }
                }
            }
        }
        else
        {
            pivot = partition::Stl_partition_mosqrt<iter, Compare>::partition(begin, end, less, inner_array, pivot_width);

            index num = pivot - begin;

            //worst case stopper if partition is bad
            next_linear_sample = false;
            if (n > 300 && ((n > (pivot - begin)* WORST_CASE_RANGE_QMS) || (n > (end - pivot)*WORST_CASE_RANGE_QMS)))
                next_linear_sample = true;


            if (num <= n / 3 || ((num + 1) / 2 < end - pivot  && num >= (n - 1) / 2)) {
                //if (pivot  - begin < end - (pivot + pivot_width)) {

                msortOut(begin, pivot, pivot + pivot_width, less);
                begin = pivot + pivot_width;
            }
            else {

                msortOut(pivot + pivot_width, end, begin, less);
                end = pivot + pivot_width;
            }

        }




    }
    //	std::sort(begin, end);
}


#define WORST_CASE_RANGE_QMS_MO3 16

template<template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
void sort_hybrid_Mo3(iter begin, iter end, Compare less) {
    typedef typename std::iterator_traits<iter>::difference_type index;
    bool next_linear_sample = false;
    bool inner_array = false;
    while (begin + 1 < end) {
        iter pivot;
        index n = end - begin;
        int pivot_width;

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

        if (n > 200 && next_linear_sample) //Hoare_block_partition_median_of_medians_of_3
        {
#ifdef asdf 
            pivot = partition::Hoare_block_partition_median_of_medians_of_3<iter, Compare>::partition(begin, end, less);
            index num = pivot - begin;
            //		std::cout<<"l: "<< (pivot - begin)<<", r: "<<(end - pivot - 1)<<std::endl;

            if (num > n / 2) {
                if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;

                msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                begin = pivot + 1;
            }
            else {
                if (pivot - begin <= (end - begin) / 3 - 2)
                    std::cout << "mistake: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;

                msortOut(pivot + 1, end, begin, pivot - begin, less);
                end = pivot;
            }
            next_linear_sample = false;
#endif
            //pivot = median::median_of_medians_partition_ninther<Partitioner, iter, Compare>(begin, end, 0.5, less);

#ifdef PARTIAL_SORT_COUNT	    
            partial_sort_count++;
#endif


            pivot = partition::Stl_partition_median_of_medians_of_3_5_left_right<true, iter, Compare>::partition(begin, end, less, inner_array, pivot_width);
            index num = pivot - begin;
            //std::cout<<"l: "<< (pivot - begin)<<", r: "<<(end - pivot - 1)<<std::endl;
            index num_right = end - pivot - pivot_width;
            if ((pivot_width > n / 3) || (7 * num - 5 < n) || (7 * num_right - 5 < n))
            {
                if (num < num_right)
                {
                    msortOut(begin, pivot, pivot + pivot_width, less);
                    begin = pivot + pivot_width;
                    inner_array = true;
                }
                else
                {
                    //	msortOut(pivot + 1, end, begin, end - pivot - 1, less);
                    msortOut(pivot + pivot_width, end, begin, less);
                    end = pivot;
                }
            }
            else {
                if (num > n / 2) {
                    //if (pivot < begin + n / 2) {
                    if (end - pivot - 1 <= (end - begin) / 3 - 2)
                    {
                        //				std::cout << "msortOutRight l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutRight(begin, pivot, end, less);
                        begin = pivot;
                        inner_array = true;
                    }
                    else {
                        msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                        begin = pivot + 1;
                        inner_array = true;
                    }

                }
                else {
                    if (pivot - begin <= (end - begin) / 3 - 2) {
                        //			std::cout << "msortOutLeft: l: " << (pivot - begin) << ", r: " << (end - pivot - 1) << std::endl;
                        msortOutLeft(pivot + 1, end, begin, less);
                        end = pivot + 1;
                    }
                    else
                    {
                        msortOut(pivot + 1, end, begin, pivot - begin, less);
                        end = pivot;
                    }
                }
            }
        }
        else
        {
            pivot = Partitioner<iter, Compare>::partition(begin, end, less);

            index num = pivot - begin;

            //worst case stopper if partition is bad
            next_linear_sample = false;
            if (n > 300 && ((n > (pivot - begin)* WORST_CASE_RANGE_QMS_MO3) || (n > (end - pivot)*WORST_CASE_RANGE_QMS_MO3)))
                next_linear_sample = true;


            if (num <= n / 3 || ((num + 1) / 2 < end - pivot  && num >= (n - 1) / 2)) {
                //if (pivot  - begin < end - (pivot + pivot_width)) {

                msortOut(begin, pivot, pivot + 1, less);
                begin = pivot + 1;
            }
            else {

                msortOut(pivot + 1, end, begin, less);
                end = pivot + 1;
            }

        }




    }
    //	std::sort(begin, end);
}


    template<template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
    void sort_Reinhardt(iter begin, iter end, Compare less) {
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
            else if (n < 40)
            {
                std::sort(begin, end, less);
                return;
            }

            pivot = Partitioner<iter, Compare>::partition(begin, end, less);
            index num = pivot - begin;

            index num_right = n - num - 1;
            index num_smaller = std::min(num_right, num);
            if (num_smaller >= n / 3 -2)
            {
     //           std::cout << "inside: " << num << " right: " << num_right << std::endl;
                if (num > num_right)
                {
                    msortOut(begin, pivot, pivot + 1, end - pivot - 1, less);
                    begin = pivot + 1;

                    sorttest(begin, pivot + 1, less);

                }
                else
                {
                    msortOut(pivot + 1, end, begin, pivot - begin, less);
                    end = pivot;
                    sorttest(pivot + 1, end, less);
                }
            }
           else if (num_smaller >= n / 15 && n> 200)
            {
                if (num > num_right)
                {
                    msortOutRight(begin, pivot, end, less);

                    begin = pivot;
                }
                else
                {
                    msortOutLeft(pivot + 1, end, begin, less);
                    end = pivot + 1;
                }
            }
            else
            {
       //         std::cout << "far out: " << num << " right: " << num_right << std::endl;

                if (num < num_right)
                {
                    msortOut(begin, pivot, pivot + 1, less);
                    begin = pivot + 1;
                }
                else
                {
                    msortOut(pivot + 1, end, begin, less);
                    end = pivot;
                }
            }
        }
    }


    

}
