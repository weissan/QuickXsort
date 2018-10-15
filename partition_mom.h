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


namespace partition {



    template<typename iter, typename Compare>
    struct Stl_partition_median_of_medians_of_3 {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid;
            if (end - begin < 50)
            {
                mid = median::median_of_3(begin, end, less);
                return partition::Stl_partition<iter, Compare>::partition(begin + 1, end - 1, mid, less);
            }
            else
            {
                mid = median::median_of_medians_of_3<partition::Stl_partition>(begin, end, less);
                //partition
                iter cut = stl_gcc::unguarded_partition(mid + 1, end - (mid - begin), *(mid), less);
                if (less(*mid, *cut))
                    cut--;

                std::iter_swap(cut, mid);

                return cut;
            }
        }
    };




    template<bool hybrid, typename iter, typename Compare>
    struct Stl_partition_median_of_medians_of_3_5 {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid;
            if (end - begin < 100)
            {
                mid = median::median_of_3(begin, end, less);
                return partition::Stl_partition<iter, Compare>::partition(begin + 1, end - 1, mid, less);
            }
            else
            {
                mid = median::median_of_medians_of_3_5<partition::Stl_partition, hybrid>(begin, end, less);

                iter cut = stl_gcc::unguarded_partition(mid + 1, end - (mid - begin), *(mid), less);
                if (less(*mid, *cut))
                    cut--;

                std::iter_swap(cut, mid);


                return cut;

                //			return hoare_block_partition_unroll_loop(begin + (end - begin) / 30 - 3, end - (end - begin) / 30 + 3, mid, less);
            }
        }
    };




    template<bool hybrid, typename iter, typename Compare, bool undersampling = false>
    struct Stl_partition_median_of_medians_of_3_5_left_right {


        static inline iter partition(iter begin, iter end, Compare less, bool inner_array, int & pivot_width) {
            //choose pivot
            iter mid;
            pivot_width = 1;
            if (end - begin < 100)
            {
                mid = median::median_of_3(begin, end, less);
                return partition::Stl_partition<iter, Compare>::partition(begin + 1, end - 1, mid, less);
            }
            else
            {

                mid = median::median_of_medians_of_3_5<partition::Stl_partition, hybrid, undersampling>(begin, end, less);
                if (inner_array && !less(*(begin - 1), *mid))
                {
                    iter cut = unguarded_partition_left(mid + 1, end - (mid - begin), *(mid), less);
                    //if (less(*mid, *cut))
                    pivot_width = cut - begin;
                    return begin;
                }
                else {
#ifdef SHOW_COMPARISONS
                    int comps = comparisons;
#endif // SHOW_COMPARISONS              

                    iter cut = unguarded_partition_right(mid + 1, end - (mid - begin), *(mid), less);
                    //if (less(*mid, *cut))
                    cut--;

#ifdef SHOW_COMPARISONS
                    std::cout << "anz: " << end- begin << "partitioning: " << ((double)comparisons - comps) / (end - begin) << std::endl;
#endif // SHOW_COMPARISONS

                    std::iter_swap(cut, mid);

                    if (cut - begin < (end - begin) / (5*(double)UNDERSAMPLING_SIZE/30) - 30)
                    {
                        iter cut2 = unguarded_partition_left(cut + 1, end, *(cut), less);
                        //if (less(*mid, *cut))
                        //cut2--;

                        //std::iter_swap(cut2, cut);
                        pivot_width = cut2 - cut;
                    }

                    return cut;
                }


                //			return hoare_block_partition_unroll_loop(begin + (end - begin) / 30 - 3, end - (end - begin) / 30 + 3, mid, less);
            }
        }
    };








};