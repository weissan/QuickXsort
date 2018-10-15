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

#ifndef STL_ALGO_H
#define STL_ALGO_H 1
#include <random>
#include <cstdlib>           // for rand
/*#include <bits/algorithmfwd.h>
#include <bits/stl_heap.h>
#include <bits/stl_tempbuf.h>  // for _Temporary_buffer
#include <bits/predefined_ops.h>

#if __cplusplus >= 201103L
#include <bits/uniform_int_dist.h>
#endif
*/

// See concept_check.h for the __glibcxx_*_requires macros.
#include <iterator>
namespace stl_gcc_new_random_pivot
{


std::mt19937 random_gen(1234);

#ifdef BLUB



	template<typename _RandomAccessIterator, typename _Distance, typename _Tp,
		typename _Compare>
		void
		__push_heap(_RandomAccessIterator __first,
			_Distance __holeIndex, _Distance __topIndex, _Tp __value,
			_Compare __comp)
	{
		_Distance __parent = (__holeIndex - 1) / 2;
		while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value))
		{
			*(__first + __holeIndex) = std::move(*(__first + __parent));
			__holeIndex = __parent;
			__parent = (__holeIndex - 1) / 2;
		}
		*(__first + __holeIndex) = std::move(__value);
	}


	template<typename _RandomAccessIterator, typename _Distance,
		typename _Tp, typename _Compare>
		void
		__adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
			_Distance __len, _Tp __value, _Compare __comp)
	{
		const _Distance __topIndex = __holeIndex;
		_Distance __secondChild = __holeIndex;
		while (__secondChild < (__len - 1) / 2)
		{
			__secondChild = 2 * (__secondChild + 1);
			if (__comp(*__first + __secondChild,
				*(__first + (__secondChild - 1))))
				__secondChild--;
			*(__first + __holeIndex) = std::move(*(__first + __secondChild));
			__holeIndex = __secondChild;
		}
		if ((__len & 1) == 0 && __secondChild == (__len - 2) / 2)
		{
			__secondChild = 2 * (__secondChild + 1);
			*(__first + __holeIndex) = std::move(*(__first
				+ (__secondChild - 1)));
			__holeIndex = __secondChild - 1;
		}
//		__decltype(__gnu_cxx::__ops::__iter_comp_val(std::move(__comp)))
//			__cmp(std::move(__comp));
		stl_gcc_new_random_pivot::__push_heap(__first, __holeIndex, __topIndex,
			std::move(__value), __comp);
	}

	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		__pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
			_RandomAccessIterator __result, _Compare& __comp)
	{
		typedef typename std::iterator_traits<_RandomAccessIterator>::value_type
			_ValueType;
		typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type
			_DistanceType;

		_ValueType __value = std::move(*__result);
		*__result = std::move(*__first);
		stl_gcc_new_random_pivot::__adjust_heap(__first, _DistanceType(0),
			_DistanceType(__last - __first),
			std::move(__value), __comp);
	}

	/**
	*  @brief  Pop an element off a heap.
	*  @param  __first  Start of heap.
	*  @param  __last   End of heap.
	*  @pre    [__first, __last) is a valid, non-empty range.
	*  @ingroup heap_algorithms
	*
	*  This operation pops the top of the heap.  The elements __first
	*  and __last-1 are swapped and [__first,__last-1) is made into a
	*  heap.
	*/
	template<typename _RandomAccessIterator>
	inline void
		pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
	{
		// concept requirements
		__glibcxx_function_requires(_Mutable_RandomAccessIteratorConcept<
			_RandomAccessIterator>)
			__glibcxx_function_requires(_LessThanComparableConcept<
				typename std::iterator_traits<_RandomAccessIterator>::value_type>)
			__glibcxx_requires_non_empty_range(__first, __last);
		__glibcxx_requires_valid_range(__first, __last);
		__glibcxx_requires_irreflexive(__first, __last);
		__glibcxx_requires_heap(__first, __last);

		if (__last - __first > 1)
		{
			--__last;
			__gnu_cxx::__ops::_Iter_less_iter __comp;
			stl_gcc_new_random_pivot::__pop_heap(__first, __last, __last, __comp);
		}
	}

	/**
	*  @brief  Pop an element off a heap using comparison functor.
	*  @param  __first  Start of heap.
	*  @param  __last   End of heap.
	*  @param  __comp   Comparison functor to use.
	*  @ingroup heap_algorithms
	*
	*  This operation pops the top of the heap.  The elements __first
	*  and __last-1 are swapped and [__first,__last-1) is made into a
	*  heap.  Comparisons are made using comp.
	*/
	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		pop_heap(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		// concept requirements
		__glibcxx_function_requires(_Mutable_RandomAccessIteratorConcept<
			_RandomAccessIterator>)
			__glibcxx_requires_valid_range(__first, __last);
		__glibcxx_requires_irreflexive_pred(__first, __last, __comp);
		__glibcxx_requires_non_empty_range(__first, __last);
		__glibcxx_requires_heap_pred(__first, __last, __comp);

		if (__last - __first > 1)
		{
			typedef __decltype(__comp) _Cmp;
			__gnu_cxx::__ops::_Iter_comp_iter<_Cmp> __cmp(std::move(__comp));
			--__last;
			stl_gcc_new_random_pivot::__pop_heap(__first, __last, __last, __cmp);
		}
	}

	template<typename _RandomAccessIterator, typename _Compare>
	void
		__make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
			_Compare& __comp)
	{
		typedef typename std::iterator_traits<_RandomAccessIterator>::value_type
			_ValueType;
		typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type
			_DistanceType;

		if (__last - __first < 2)
			return;

		const _DistanceType __len = __last - __first;
		_DistanceType __parent = (__len - 2) / 2;
		while (true)
		{
			_ValueType __value = std::move(*(__first + __parent));
			stl_gcc_new_random_pivot::__adjust_heap(__first, __parent, __len, std::move(__value),
				__comp);
			if (__parent == 0)
				return;
			__parent--;
		}
	}

	/**
	*  @brief  Construct a heap over a range.
	*  @param  __first  Start of heap.
	*  @param  __last   End of heap.
	*  @ingroup heap_algorithms
	*
	*  This operation makes the elements in [__first,__last) into a heap.
	*/
	template<typename _RandomAccessIterator>
	inline void
		make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
	{

		stl_gcc_new_random_pivot::__make_heap(__first, __last, __comp);
	}

	/**
	*  @brief  Construct a heap over a range using comparison functor.
	*  @param  __first  Start of heap.
	*  @param  __last   End of heap.
	*  @param  __comp   Comparison functor to use.
	*  @ingroup heap_algorithms
	*
	*  This operation makes the elements in [__first,__last) into a heap.
	*  Comparisons are made using __comp.
	*/


	template<typename _RandomAccessIterator, typename _Compare>
	void
		__sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
			_Compare& __comp)
	{
		while (__last - __first > 1)
		{
			--__last;
			stl_gcc_new_random_pivot::__pop_heap(__first, __last, __last, __comp);
		}
	}

	

	/**
	*  @brief  Sort a heap using comparison functor.
	*  @param  __first  Start of heap.
	*  @param  __last   End of heap.
	*  @param  __comp   Comparison functor to use.
	*  @ingroup heap_algorithms
	*
	*  This operation sorts the valid heap in the range [__first,__last).
	*  Comparisons are made using __comp.
	*/
	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
			_Compare __comp)
	{
		// concept requirements
		__glibcxx_function_requires(_Mutable_RandomAccessIteratorConcept<
			_RandomAccessIterator>)
			__glibcxx_requires_valid_range(__first, __last);
		__glibcxx_requires_irreflexive_pred(__first, __last, __comp);
		__glibcxx_requires_heap_pred(__first, __last, __comp);

		typedef __decltype(__comp) _Cmp;
		__gnu_cxx::__ops::_Iter_comp_iter<_Cmp> __cmp(std::move(__comp));
		stl_gcc_new_random_pivot::__sort_heap(__first, __last, __cmp);
	}

#endif // BLUB


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

#ifdef BLUB

	/// This is a helper function for the sort routines.
	template<typename _RandomAccessIterator, typename _Compare>
	void
		__heap_select(_RandomAccessIterator __first,
			_RandomAccessIterator __middle,
			_RandomAccessIterator __last, _Compare __comp)
	{
		stl_gcc_new_random_pivot::__make_heap(__first, __middle, __comp);
		for (_RandomAccessIterator __i = __middle; __i < __last; ++__i)
			if (__comp(*__i, *__first))
				stl_gcc_new_random_pivot::__pop_heap(__first, __middle, __i, __comp);
	}

	// partial_sort

	template<typename _InputIterator, typename _RandomAccessIterator,
		typename _Compare>
		_RandomAccessIterator
		__partial_sort_copy(_InputIterator __first, _InputIterator __last,
			_RandomAccessIterator __result_first,
			_RandomAccessIterator __result_last,
			_Compare __comp)
	{
		typedef typename iterator_traits<_InputIterator>::value_type
			_InputValueType;
		typedef std::iterator_traits<_RandomAccessIterator> _RItTraits;
		typedef typename _RItTraits::difference_type _DistanceType;

		if (__result_first == __result_last)
			return __result_last;
		_RandomAccessIterator __result_real_last = __result_first;
		while (__first != __last && __result_real_last != __result_last)
		{
			*__result_real_last = *__first;
			++__result_real_last;
			++__first;
		}

		stl_gcc_new_random_pivot::__make_heap(__result_first, __result_real_last, __comp);
		while (__first != __last)
		{
			if (__comp(*__first, *__result_first))
				stl_gcc_new_random_pivot::__adjust_heap(__result_first, _DistanceType(0),
					_DistanceType(__result_real_last
						- __result_first),
					_InputValueType(*__first), __comp);
			++__first;
		}
		stl_gcc_new_random_pivot::__sort_heap(__result_first, __result_real_last, __comp);
		return __result_real_last;
	}

	/**
	*  @brief Copy the smallest elements of a sequence.
	*  @ingroup sorting_algorithms
	*  @param  __first   An iterator.
	*  @param  __last    Another iterator.
	*  @param  __result_first   A random-access iterator.
	*  @param  __result_last    Another random-access iterator.
	*  @return   An iterator indicating the end of the resulting sequence.
	*
	*  Copies and sorts the smallest N values from the range @p [__first,__last)
	*  to the range beginning at @p __result_first, where the number of
	*  elements to be copied, @p N, is the smaller of @p (__last-__first) and
	*  @p (__result_last-__result_first).
	*  After the sort if @e i and @e j are iterators in the range
	*  @p [__result_first,__result_first+N) such that i precedes j then
	*  *j<*i is false.
	*  The value returned is @p __result_first+N.
	*/
	template<typename _InputIterator, typename _RandomAccessIterator>
	inline _RandomAccessIterator
		partial_sort_copy(_InputIterator __first, _InputIterator __last,
			_RandomAccessIterator __result_first,
			_RandomAccessIterator __result_last)
	{
#ifdef _GLIBCXX_CONCEPT_CHECKS
		typedef typename iterator_traits<_InputIterator>::value_type
			_InputValueType;
		typedef typename iterator_traits<_RandomAccessIterator>::value_type
			_OutputValueType;
#endif

		// concept requirements
		__glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
			__glibcxx_function_requires(_ConvertibleConcept<_InputValueType,
				_OutputValueType>)
			__glibcxx_function_requires(_LessThanOpConcept<_InputValueType,
				_OutputValueType>)
			__glibcxx_function_requires(_LessThanComparableConcept<_OutputValueType>)
			__glibcxx_requires_valid_range(__first, __last);
		__glibcxx_requires_irreflexive(__first, __last);
		__glibcxx_requires_valid_range(__result_first, __result_last);

		return stl_gcc_new_random_pivot::__partial_sort_copy(__first, __last,
			__result_first, __result_last,
			__gnu_cxx::__ops::__iter_less_iter());
	}

	/**
	*  @brief Copy the smallest elements of a sequence using a predicate for
	*         comparison.
	*  @ingroup sorting_algorithms
	*  @param  __first   An input iterator.
	*  @param  __last    Another input iterator.
	*  @param  __result_first   A random-access iterator.
	*  @param  __result_last    Another random-access iterator.
	*  @param  __comp    A comparison functor.
	*  @return   An iterator indicating the end of the resulting sequence.
	*
	*  Copies and sorts the smallest N values from the range @p [__first,__last)
	*  to the range beginning at @p result_first, where the number of
	*  elements to be copied, @p N, is the smaller of @p (__last-__first) and
	*  @p (__result_last-__result_first).
	*  After the sort if @e i and @e j are iterators in the range
	*  @p [__result_first,__result_first+N) such that i precedes j then
	*  @p __comp(*j,*i) is false.
	*  The value returned is @p __result_first+N.
	*/
	template<typename _InputIterator, typename _RandomAccessIterator,
		typename _Compare>
		inline _RandomAccessIterator
		partial_sort_copy(_InputIterator __first, _InputIterator __last,
			_RandomAccessIterator __result_first,
			_RandomAccessIterator __result_last,
			_Compare __comp)
	{



		return stl_gcc_new_random_pivot::__partial_sort_copy(__first, __last,
			__result_first, __result_last,
			__gnu_cxx::__ops::__iter_comp_iter(__comp));
	}


    template<typename _RandomAccessIterator, typename _Compare>
    inline void
        __partial_sort(_RandomAccessIterator __first,
            _RandomAccessIterator __middle,
            _RandomAccessIterator __last,
            _Compare __comp)
    {
        stl_gcc_new_random_pivot::__heap_select(__first, __middle, __last, __comp);
        stl_gcc_new_random_pivot::__sort_heap(__first, __middle, __comp);
    }

#endif

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
				stl_gcc_new_random_pivot::__unguarded_linear_insert(__i,__comp);
		}
	}

	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	inline void
		__unguarded_insertion_sort(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
			stl_gcc_new_random_pivot::__unguarded_linear_insert(__i,__comp);
	}

	/**
	*  @doctodo
	*  This controls some aspect of the sort routines.
	*/
	//enum { _S_threshold = 16 };
	enum { _S_threshold = 3 };

	/// This is a helper function for the sort routine.
	template<typename _RandomAccessIterator, typename _Compare>
	void
		__final_insertion_sort(_RandomAccessIterator __first,
			_RandomAccessIterator __last, _Compare __comp)
	{
		if (__last - __first > int(_S_threshold))
		{
			stl_gcc_new_random_pivot::__insertion_sort(__first, __first + int(_S_threshold), __comp);
			stl_gcc_new_random_pivot::__unguarded_insertion_sort(__first + int(_S_threshold), __last,
				__comp);
		}
		else
			stl_gcc_new_random_pivot::__insertion_sort(__first, __last, __comp);
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
		
		int n = __last - __first;
		std::iter_swap(__first+1, __first + (random_gen() % n));
		std::iter_swap(__mid, __first + (random_gen() % n));
		std::iter_swap(__last - 1, __first + (random_gen() % n));
		
		stl_gcc_new_random_pivot::__move_median_to_first(__first, __first + 1, __mid, __last - 1,
			__comp);
		__mid = stl_gcc_new_random_pivot::__unguarded_partition(__first + 1, __last, __first, __comp);
		std::iter_swap(__first, __mid - 1);
		return __mid - 1;
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
	//			stl_gcc_new_random_pivot::__partial_sort(__first, __last, __last, __comp);
				std::partial_sort(__first, __last, __last, __comp);
		//		if(__last - __first > 100000)
		//			std::cout << "partial sort_gcc_new: " << __last - __first << std::endl;
				return;
			}

//			std::cout << "depth: " << __depth_limit <<"size: " << __last - __first << std::endl;
			--__depth_limit;
			_RandomAccessIterator __cut =
				stl_gcc_new_random_pivot::__unguarded_partition_pivot(__first, __last, __comp);
			stl_gcc_new_random_pivot::__introsort_loop(__cut+1, __last, __depth_limit, __comp);
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
			stl_gcc_new_random_pivot::__introsort_loop(__first, __last,
				ilogb(__last - __first) * 2,
				__comp);
			stl_gcc_new_random_pivot::__final_insertion_sort(__first, __last, __comp);
		}
	}

	template<typename T>
	void sort(std::vector<T> &v) {
		stl_gcc_new_random_pivot::sort(v.begin(), v.end(), std::less<T>());
	}
}

#endif // C++17
