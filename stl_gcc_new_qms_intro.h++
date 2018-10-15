#pragma once

// Algorithm implementation -*- C++ -*-

// Copyright (C) 2001-2017 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
*
* Copyright (c) 1994
* Hewlett-Packard Company
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Hewlett-Packard Company makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*
* Copyright (c) 1996
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*/

/** @file bits/stl_algo.h
*  This is an internal header file, included by other library headers.
*  Do not attempt to use it directly. @headername{algorithm}
*/

#ifndef STL_ALGO_H_QMS
#define STL_ALGO_H_QMS 1

#include <cstdlib>           // for rand
/*#include <bits/algorithmfwd.h>
#include <bits/stl_heap.h>
#include <bits/stl_tempbuf.h>  // for _Temporary_buffer
#include <bits/predefined_ops.h>

#if __cplusplus >= 201103L
#include <bits/uniform_int_dist.h>
#endif
*/
#include "quickmerge_worst_case_efficient_3_5_undersampling.h++"

// See concept_check.h for the __glibcxx_*_requires macros.
#include <iterator>
namespace stl_gcc_new_qms_intro
{

	/// Swaps the median value of *__a, *__b and *__c under __comp to *__result
	template<typename _Iterator, typename _Compare>
	void
		__move_median_to_first(_Iterator __result, _Iterator __a, _Iterator __b,
			_Iterator __c, _Compare __comp)
	{
		if (__comp(*__a, *__b))
		{
			if (__comp(*__b, *__c))
				std::iter_swap(__result, __b);
			else if (__comp(*__a, *__c))
				std::iter_swap(__result, __c);
			else
				std::iter_swap(__result, __a);
		}
		else if (__comp(*__a, *__c))
			std::iter_swap(__result, __a);
		else if (__comp(*__b, *__c))
			std::iter_swap(__result, __c);
		else
			std::iter_swap(__result, __b);
	}




	/// This is a helper function...
	template<typename _BidirectionalIterator, typename _Predicate>
	_BidirectionalIterator
		__partition(_BidirectionalIterator __first, _BidirectionalIterator __last,
			_Predicate __pred)
	{
		while (true)
		{
			while (true)
				if (__first == __last)
					return __first;
				else if (__pred(*__first))
					++__first;
				else
					break;
			--__last;
			while (true)
				if (__first == __last)
					return __first;
				else if (!bool(__pred(*__last)))
					--__last;
				else
					break;
			std::iter_swap(__first, __last);
			++__first;
		}
	}


	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	void
		__unguarded_linear_insert(_RandomAccessIterator __last,
			_Compare __comp)
	{
		typename std::iterator_traits<_RandomAccessIterator>::value_type
			__val = std::move(*__last);
		_RandomAccessIterator __next = __last;
		--__next;
		while (__comp(__val, *__next))
		{
			*__last = std::move(*__next);
			__last = __next;
			--__next;
		}
		*__last = std::move(__val);
	}

	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	void
		__insertion_sort(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		if (__first == __last) return;

		for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
		{
			if (__comp(*__i, *__first))
			{
				typename std::iterator_traits<_RandomAccessIterator>::value_type
					__val = std::move(*__i);
				std::move_backward(__first, __i, __i + 1);
				*__first = std::move(__val);
			}
			else
				stl_gcc_new_qms_intro::__unguarded_linear_insert(__i, __comp);
		}
	}

	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		__unguarded_insertion_sort(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
			stl_gcc_new_qms_intro::__unguarded_linear_insert(__i, __comp);
	}

	/**
	*  @doctodo
	*  This controls some aspect of the sort routines.
	*/
	enum { _S_threshold = 16 };

	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	void
		__final_insertion_sort(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		if (__last - __first > int(_S_threshold))
		{
			stl_gcc_new_qms_intro::__insertion_sort(__first, __first + int(_S_threshold), __comp);
			stl_gcc_new_qms_intro::__unguarded_insertion_sort(__first + int(_S_threshold), __last,
				__comp);
		}
		else
			stl_gcc_new_qms_intro::__insertion_sort(__first, __last, __comp);
	}

	/// This is a helper function...
	template<typename _RandomAccessIterator, typename _Compare>
	_RandomAccessIterator
		__unguarded_partition(_RandomAccessIterator __first,
			_RandomAccessIterator __last,
			_RandomAccessIterator __pivot, _Compare __comp)
	{
		while (true)
		{
			while (__comp(*__first, *__pivot))
				++__first;
			--__last;
			while (__comp(*__pivot, *__last))
				--__last;
			if (!(__first < __last))
				return __first;
			std::iter_swap(__first, __last);
			++__first;
		}
	}

	/// This is a helper function...
	template<typename _RandomAccessIterator, typename _Compare>
	inline _RandomAccessIterator
		__unguarded_partition_pivot(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		_RandomAccessIterator __mid = __first + (__last - __first) / 2;
		stl_gcc_new_qms_intro::__move_median_to_first(__first, __first + 1, __mid, __last - 1,
			__comp);
		return stl_gcc_new_qms_intro::__unguarded_partition(__first + 1, __last, __first, __comp);
	}



	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Size, typename _Compare>
	void
		__introsort_loop(_RandomAccessIterator __first,
			_RandomAccessIterator __last,
			_Size __depth_limit, _Compare __comp)
	{
		while (__last - __first > int(_S_threshold))
		{
			if (__depth_limit == 0)
			{
				//			stl_gcc_new_qms_intro::__partial_sort(__first, __last, __last, __comp);
				quickmerge_worst_case_efficient_3_5_undersampling::sort(__first, __last, __comp);
				if (__last - __first > 500000000)
					std::cout << "quickmerge_worst_case_efficient_3_5_undersampling sort_gcc_new: " << __last - __first << std::endl;
				return;
			}

			//			std::cout << "depth: " << __depth_limit <<"size: " << __last - __first << std::endl;
			--__depth_limit;
			_RandomAccessIterator __cut =
				stl_gcc_new_qms_intro::__unguarded_partition_pivot(__first, __last, __comp);
			stl_gcc_new_qms_intro::__introsort_loop(__cut, __last, __depth_limit, __comp);
			__last = __cut;
		}
	}

	// sort

	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		sort(_RandomAccessIterator __first, _RandomAccessIterator __last,
			_Compare __comp)
	{
		if (__first != __last)
		{
			stl_gcc_new_qms_intro::__introsort_loop(__first, __last,
				ilogb(__last - __first) * 2,
				__comp);
			stl_gcc_new_qms_intro::__final_insertion_sort(__first, __last, __comp);
		}
	}

	template<typename T>
	void sort(std::vector<T> &v) {
		stl_gcc_new_qms_intro::sort(v.begin(), v.end(), std::less<T>());
	}
}

#endif // C++17
