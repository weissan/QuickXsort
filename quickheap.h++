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
#include <iterator>

//#include "insertionsort.h"
#include "median.h"
#include "partition_stl.h"




namespace quickheap {
   
        //
        // Heap ohne Verbesserungen
        //
        template<class it, class Compare>
        class heap {
            using t = typename std::iterator_traits<it>::value_type;
        public:
            heap(it _root, it _final, Compare less) :
                root(_root),
                final(_final - root),
                comp(less),
                last_parent((final - 1) >> 1) {}
            void heapify(void) {
                if (final > 0) {
                    int i = last_parent + 1;
                    while (i > 0) reheap(--i);
                }
            }
            void sort(void) {
                heapify();
                bool even = (final & 1) == 0;
                while (final > 0) {
                    std::swap(*root, *(root + final));
                    final--;
                    //Bei jedem zweiten Schritt last_parent um 1 verringern
                    if (even = !even) last_parent--;
                    reheap(0);
                }
            }
            void sink(void) {
                if (final > 0) {
                    int i = 0, i2;
                    
                    t inserted_element = std::move(*root);
                    do {
                        i2 = first_child(i);
                        if (i2 != final && comp(root[i2], root[i2 + 1])) i2++;
                        root[i] = std::move(root[i2]); i = i2;
                    } while (i <= last_parent);
                    root[i] = std::move(inserted_element);
                }
            }
        protected:
            Compare comp;
            const it root;
            int final, last_parent;
            inline int first_child(int i) { return (i << 1) + 1; }
            inline int parent(int i) { return (i - 1) >> 1; }
            void reheap(int start_i) {
                if (start_i <= last_parent) {
                    int i = start_i, i2;
                    t inserted_element = std::move(root[i]);
                    do {
                        i2 = first_child(i);
                        if (i2 != final && comp(root[i2], root[i2 + 1])) i2++;
                        if (!comp(inserted_element, root[i2])) break;
                        root[i] = std::move(root[i2]);
                        i = i2;
                    } while (i <= last_parent);
                    root[i] = std::move(inserted_element);
                }
            }
        };

   
    


    template<typename Comp,typename T>
    class Greater : Comp
    {
    public:
        Comp less;
        bool operator()(T const &a, T const &b) const { return less(b,a); }
        Greater(Comp le) { less = le ;}
    };



    template<template<class, class> class Partitioner = partition::Stl_partition, typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        typedef typename std::iterator_traits<iter>::difference_type index;
        using t = typename std::iterator_traits<iter>::value_type;
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

            pivot = Partitioner<iter, Greater<Compare, t> >::partition(begin, end, Greater<Compare,t>(less));
            index num = pivot - begin;
            if (num < n / 2) {
             /*   heap h(begin, pivot - 1);
                h.heapify();

                iter target = end - num;
                std::iter_swap(pivot, target - 1);
                if(target < pivot)
                    std::cout << "target wrong pivot" << std::endl;
                for (int i = 0; i < num; i++) {
                    std::iter_swap(begin, target);
                    target++;
                    h.sink();
                }
                if (target != end)
                    std::cout << "target wrong" << std::endl;
                quickmerge::sorttest(end - num - 1, end, less, "testing first case " + std::to_string(num));
               
                end = end - num - 1;
                */

                heap<std::reverse_iterator<iter>, Compare> h(std::reverse_iterator<iter>(pivot), std::reverse_iterator<iter>(begin+1), less);
                h.heapify();

                iter target = end - 1;
                
                if (target < pivot)
                    std::cout << "target wrong pivot" << std::endl;
                for (int i = 0; i < num; i++) {
                    std::iter_swap(pivot - 1, target);
                    target--;
                    h.sink();
                }

                std::iter_swap(pivot, target );
                if (target <= pivot - 1)
                    std::cout << "target wrong" << pivot - target << std::endl;
        //        quickmerge::sorttest(end - num - 1, end, less, "testing first case " + std::to_string(num));

                end = end - num - 1;


            }
            else {

                //ohne reverse iterator!!!!!!!!!!!!
     //           typedef riter reverse_iterator(iter);
                heap<iter, Greater<Compare, t> > h(pivot + 1, end - 1, Greater<Compare, t>(less));
                h.heapify();
                for (int i = 0; i <  n - num - 1; i++) {
                    std::iter_swap(begin, pivot + 1);
                    h.sink();
                    begin++;
                }
                std::iter_swap(begin, pivot);    
                begin++;
         //       quickmerge::sorttest(begin - (n - num), begin, less, "testing asecond case " + std::to_string(n));
                //begin = pivot + 1;
            }
        }
    }

    template<typename T>
    void sort(std::vector<T> &v) {
        quickheap::sort(v.begin(), v.end(), std::less<T>());
    }




}