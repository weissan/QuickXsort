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
    struct Hoare_block_partition_median_of_medians_of_3 {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid;
            if (end - begin < 50)
            {
                mid = median::median_of_3(begin, end, less);
                return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less);
            }
            else
            {
                mid = median::median_of_medians_of_3<partition::Hoare_block_partition>(begin, end, less);
                //partition
                return hoare_block_partition_unroll_loop(begin + (end - begin) / 6 - 1, end - (end - begin) / 6 + 1, mid, less);
            }
        }
    };



    template<typename iter, typename Compare>
    struct Hoare_block_partition_median_of_medians_of_3_5 {
        static inline iter partition(iter begin, iter end, Compare less) {
            //choose pivot
            iter mid;
            if (end - begin < 100)
            {
                mid = median::median_of_3(begin, end, less);
                return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less);
            }
            else
            {
                mid = median::median_of_medians_of_3_5<partition::Hoare_block_partition>(begin, end, less);
                //partition
                return hoare_block_partition_unroll_loop(begin + (end - begin) / 30 - 3, end - (end - begin) / 30 + 3, mid, less);
            }
        }
    };


    template<typename iter, typename Compare>
    struct Hoare_block_partition_median_of_linear_sqrt {
        static inline iter partition(iter begin, iter end, Compare less) {
            iter mid;
            if (end - begin > 2000)
            {
                unsigned int pivot_sample_size = (end - begin) / 24;
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                mid = median::median_of_medians_of_sqrt(begin, end, less); //choose pivot as median of sqrt(n)
                                                                           //partition
                return hoare_block_partition_unroll_loop(begin, end, mid, less);
            }
            else
            {
                if (end - begin > 200)
                    mid = median::median_of_5_medians_of_5(begin, end, less);
                else if (end - begin > 50)
                    mid = median::median_of_3_medians_of_3(begin, end, less);
                else
                    mid = median::median_of_3(begin, end, less);
                //partition
                return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less);
            }


        }

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
    };



    template<typename iter, typename Compare>
    struct Hoare_block_partition_median_of_linear {
        static inline iter partition(iter begin, iter end, Compare less) {
            iter mid;
            if (end - begin > 2000)
            {
                unsigned int pivot_sample_size = (end - begin) / 30;
                pivot_sample_size += (1 - (pivot_sample_size % 2));//make it an odd number
                mid = median::median_of_k_medians_of_5(begin, end, less, pivot_sample_size); //choose pivot as median of sqrt(n)
                                                                                             //partition
                return hoare_block_partition_unroll_loop(begin + pivot_sample_size / 2, end - pivot_sample_size / 2, mid, less);
            }
            else
            {
                if (end - begin > 200)
                    mid = median::median_of_5_medians_of_5(begin, end, less);
                else if (end - begin > 50)
                    mid = median::median_of_3_medians_of_3(begin, end, less);
                else
                    mid = median::median_of_3(begin, end, less);
                //partition
                return hoare_block_partition_unroll_loop(begin + 1, end - 1, mid, less);
            }


        }

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
    };




};