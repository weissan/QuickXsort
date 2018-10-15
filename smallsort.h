#pragma once
#include "median.h"

namespace smallsort {

//	t b[8]; // temporary
	

	template<typename iter, typename Compare>
	inline void sort4(iter begin,  Compare less) {
		leanswap(begin, begin + 1, less);
		leanswap(begin + 2, begin + 3, less);
		leanswap(begin, begin + 2, less); 
		leanswap(begin + 1, begin + 3, less);
		leanswap(begin + 1, begin + 2, less);
	}
	template<typename iter, typename Compare>
	inline void sort3(iter begin, Compare less) {
		leanswap(begin, begin + 1, less);
		leanswap(begin + 1, begin + 2, less);
		leanswap(begin, begin + 1, less);
	}

	template<typename iter, typename iter2, typename Compare>
	inline void sort4(iter begin, iter2 target, Compare less) {
		bool compare = less(begin[1], begin[0]);
		target[0] = std::move(compare ? begin[1] : begin[0]);
		target[1] = std::move(compare ? begin[0] : begin[1]);

		compare = less(begin[3], begin[2]);
		target[2] = std::move(compare ? begin[3] : begin[2]);
		target[3] = std::move(compare ? begin[2] : begin[3]);

		leanswap(target, target + 2, less);
		leanswap(target + 1, target + 3, less);
		leanswap(target + 1, target + 2, less);
	}
	template<typename iter, typename iter2, typename Compare>
	inline void sort3(iter begin, iter2 target, Compare less) {
		bool compare = less(begin[1], begin[0]);
		target[0] = std::move(compare ? begin[1] : begin[0]);
		target[1] = std::move(compare ? begin[0] : begin[1]);
		target[2] = std::move(begin[2]);
		leanswap(target + 1, target + 2, less);
		leanswap(target, target + 1, less);
	}
	template<typename iter, typename iter2, typename Compare>
	inline void sort2(iter begin, iter2 target, Compare less) {
		bool compare = less(begin[1], begin[0]);
		target[0] = std::move(compare ? begin[1] : begin[0]);
		target[1] = std::move(compare ? begin[0] : begin[1]);
	}



    template<typename iter, typename iter2, typename Compare>
    inline void sort4_target_swap(iter begin, iter2 target, Compare less) {
        typedef typename std::iterator_traits<iter>::value_type t;
        t b[4];       
        b[0] = std::move(target[0]);
        b[1] = std::move(target[1]);
        b[2] = std::move(target[2]);
        b[3] = std::move(target[3]);

        bool compare = less(begin[1], begin[0]);

        target[0] = std::move(compare ? begin[1] : begin[0]);
        target[1] = std::move(compare ? begin[0] : begin[1]);

        compare = less(begin[3], begin[2]);
        target[2] = std::move(compare ? begin[3] : begin[2]);
        target[3] = std::move(compare ? begin[2] : begin[3]);

        leanswap(target, target + 2, less);
        leanswap(target + 1, target + 3, less);
        leanswap(target + 1, target + 2, less);

        begin[0] = std::move(b[0]);
        begin[1] = std::move(b[1]);
        begin[2] = std::move(b[2]);
        begin[3] = std::move(b[3]);
    }
    template<typename iter, typename iter2, typename Compare>
    inline void sort3_target_swap(iter begin, iter2 target, Compare less) {
        typedef typename std::iterator_traits<iter>::value_type t;
        t b[3];
        b[0] = std::move(target[0]);
        b[1] = std::move(target[1]);
        b[2] = std::move(target[2]);

        bool compare = less(begin[1], begin[0]);
        target[0] = std::move(compare ? begin[1] : begin[0]);
        target[1] = std::move(compare ? begin[0] : begin[1]);
        target[2] = std::move(begin[2]);
        leanswap(target + 1, target + 2, less);
        leanswap(target, target + 1, less);
        begin[0] = std::move(b[0]);
        begin[1] = std::move(b[1]);
        begin[2] = std::move(b[2]);
    }
    template<typename iter, typename iter2, typename Compare>
    inline void sort2_target_swap(iter begin, iter2 target, Compare less) {
        typedef typename std::iterator_traits<iter>::value_type t;
        t b[2];
        b[0] = std::move(target[0]);
        b[1] = std::move(target[1]);

        bool compare = less(begin[1], begin[0]);
        target[0] = std::move(compare ? begin[1] : begin[0]);
        target[1] = std::move(compare ? begin[0] : begin[1]);
        begin[0] = std::move(b[0]);
        begin[1] = std::move(b[1]);
    }


	template<int num1, int num2, typename iter, typename iter2, typename Compare>
	inline void merge(iter begin, iter2 target, Compare less) {
		unsigned int i1, i2;
		i1 = 0;
		i2 = num1;
		while (i1 < num1 && i2 < num2 + num1) {
			bool smaller = (begin[i1] < begin[i2]);    // tempit = (*i1 < *i2) ? i1++ : i2++;
										   //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
			*target++ = std::move(smaller ? begin[i1] : begin[i2]);
			i1 += smaller;
			i2 += 1 - smaller;
		}
		while (i1 < num1)
			*(target++) = std::move(begin[i1++]);
		while (i2 < num2 + num1)
			*(target++) = std::move(begin[i2++]);
	}

	template< typename iter, typename iter2, typename Compare>
	inline void merge(unsigned int num1, unsigned int num2, iter begin, iter2 target, Compare less) {
		unsigned int i1, i2;
		i1 = 0;
		i2 = num1;
		while (i1 < num1 && i2 < num2 + num1) {
			bool smaller = (begin[i1] < begin[i2]);    // tempit = (*i1 < *i2) ? i1++ : i2++;
													   //iter tempit = i2 + (((unsigned int)smaller - 1) & (unsigned int)(i1 - i2));
			*target++ = std::move(smaller ? begin[i1] : begin[i2]);
			i1 += smaller;
			i2 += 1 - smaller;
		}
		while (i1 < num1)
			*(target++) = std::move(begin[i1++]);
		while (i2 < num2 + num1)
			*(target++) = std::move(begin[i2++]);
	}


	template<typename iter, typename Compare>
	inline void sort5(iter begin, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t temp[5];
		sort3(begin, temp, less);
		sort2(begin + 3, temp + 3, less);
		merge<3, 2>(temp, begin, less);
	}
	template<typename iter, typename Compare>
	inline void sort6(iter begin, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t; 
		t temp[6];
		sort3(begin, temp, less);
		sort3(begin + 3, temp + 3, less);
		merge<3, 3>(temp, begin, less);
	}
	template<typename iter, typename Compare>
	inline void sort7(iter begin, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t; 
		t temp[7];
		sort3(begin, temp, less);
		sort4(begin + 3, temp + 3, less);
		merge<3, 4>(temp, begin, less);
	}
	template<typename iter, typename Compare>
	inline void sort8(iter begin, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t; 
		t temp[8];
		sort4(begin, temp, less);
		sort4(begin + 4, temp + 4, less);
		merge<4, 4>(temp, begin, less);
	}



	template<typename iter, typename iter2, typename Compare>
	inline void sort5(iter begin, iter2 target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t temp[5];
		sort3(begin, temp, less);
		sort2(begin + 3, temp + 3, less);
		merge<3, 2>(temp, begin, less);
	}
	template<typename iter, typename iter2, typename Compare>
	inline void sort6(iter begin, iter2 target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t temp[6];
		sort3(begin, temp, less);
		sort3(begin + 3, temp + 3, less);
		merge<3, 3>(temp, begin, less);
	}
	template<typename iter, typename iter2, typename Compare>
	inline void sort7(iter begin, iter2 target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t temp[7];
		sort3(begin, temp, less);
		sort4(begin + 3, temp + 3, less);
		merge<3, 4>(temp, begin, less);
	}
	template<typename iter, typename iter2, typename Compare>
	inline void sort8(iter begin, iter2 target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t temp[8];
		sort4(begin, temp, less);
		sort4(begin + 4, temp + 4, less);
		merge<4, 4>(temp, begin, less);
	}





	template<typename iter, typename Compare>
	inline void sort_small(iter begin, iter end, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		typedef typename std::iterator_traits<iter>::difference_type index;
		index n = end - begin;
		index first_half = n / 2;
		t temp[16];
		assert(end - begin <= 16);
		switch (n - first_half){
		case 2:
			sort2(begin + first_half, temp + first_half, less);
			if(first_half == 2)
				sort2(begin, temp, less);
			break;
		case 3:
			smallsort::sort3(begin + first_half, temp + first_half, less);
			if (first_half == 2)
				sort2(begin, temp, less);
			else
				smallsort::sort3(begin, temp, less);
			break;
		case 4:
			smallsort::sort4(begin + first_half, temp + first_half, less);
			if (first_half == 3)
				smallsort::sort3(begin, temp, less);
			else
				smallsort::sort4(begin, temp, less);
			break;
		case 5:
			smallsort::sort5(begin + first_half, temp + first_half, less);
			if (first_half == 4)
				smallsort::sort4(begin, temp, less);
			else
				smallsort::sort5(begin, temp, less);
			break;
		case 6:
			smallsort::sort6(begin + first_half, temp + first_half, less);
			if (first_half == 5)
				smallsort::sort5(begin, temp, less);
			else
				smallsort::sort6(begin, temp, less);
			break;
		case 7:
			smallsort::sort7(begin + first_half, temp + first_half, less);
			if (first_half == 6)
				smallsort::sort6(begin, temp, less);
			else
				smallsort::sort7(begin, temp, less);
			break;
		case 8:
			smallsort::sort8(begin + first_half, temp + first_half, less);
			if (first_half == 7)
				smallsort::sort7(begin, temp, less);
			else
				smallsort::sort8(begin, temp, less);
			break;
		}

		merge(first_half, n - first_half, temp, begin, less);
	
	}



	template<typename iter, typename Compare>
	inline void sort5(iter begin, iter target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t b[5];
		t bb;
		bool flag = false;
		if (less(begin[0], begin[1])) {
			b[0] = std::move(begin[0]);
			b[2] = std::move(begin[1]);
		}
		else {
			b[2] = std::move(begin[0]);
			b[0] = std::move(begin[1]);
		}
		if (less(begin[2], begin[3])) {
			b[1] = std::move(begin[2]);
			b[3] = std::move(begin[3]);
		}
		else {
			b[3] = std::move(begin[2]);
			b[1] = std::move(begin[3]);
		}
		b[4] = std::move(begin[4]);
		begin[0] = std::move(target[0]);
		begin[1] = std::move(target[1]);
		begin[2] = std::move(target[2]);
		begin[3] = std::move(target[3]);
		begin[4] = std::move(target[4]);
		if (less(b[1], b[0])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(temp);
			temp = std::move(b[3]);
			b[3] = std::move(b[2]);
			b[2] = std::move(temp);
		}
		if (less(b[4], b[1])) {
			if (less(b[4], b[0])) {
				bb = std::move(b[4]);
				b[4] = std::move(b[3]);
				b[3] = std::move(b[1]);
				b[1] = std::move(b[0]);
				b[0] = std::move(bb);
				flag = true;
			}
			else {
				bb = std::move(b[4]);
				b[4] = std::move(b[3]);
				b[3] = std::move(b[1]);
				b[1] = std::move(bb);
			}
		}
		else {
			if (less(b[4], b[3])) {
				t temp = std::move(b[4]);
				b[4] = std::move(b[3]);
				b[3] = std::move(temp);
			}
		}
		if (less(b[2], b[3])) {
			if (!flag && less(b[2], b[1])) {
				t temp = std::move(b[1]);
				b[1] = std::move(b[2]);
				b[2] = std::move(temp);
			}
		}
		else {
			if (less(b[2], b[4])) {
				t temp = std::move(b[2]);
				b[2] = std::move(b[3]);
				b[3] = std::move(temp);
			}
			else {
				bb = std::move(b[4]);
				b[4] = std::move(b[2]);
				b[2] = std::move(b[3]);
				b[3] = std::move(bb);
			}
		}
		iter i = target;
		t* j = b;
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
	}


	template<typename iter, typename Compare>
	inline void sort6(iter begin, iter target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t bb;
		t b[6];
		bool flag = false;
		if (less(begin[0], begin[1])) {
			b[0] = std::move(begin[0]);
			b[3] = std::move(begin[1]);
		}
		else {
			b[3] = std::move(begin[0]);
			b[0] = std::move(begin[1]);
		}
		if (less(begin[2], begin[3])) {
			b[1] = std::move(begin[2]);
			b[4] = std::move(begin[3]);
		}
		else {
			b[4] = std::move(begin[2]);
			b[1] = std::move(begin[3]);
		}
		if (less(begin[4], begin[5])) {
			b[2] = std::move(begin[4]);
			b[5] = std::move(begin[5]);
		}
		else {
			b[5] = std::move(begin[4]);
			b[2] = std::move(begin[5]);
		}
		iter i = begin;
		iter j = target;
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		if (less(b[1], b[0])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(temp);
			temp = std::move(b[4]);
			b[4] = std::move(b[3]);
			b[3] = std::move(temp);
		}
		if (less(b[2], b[0])) {
			target[0] = std::move(b[2]);
			b[2] = std::move(b[1]);
			b[1] = std::move(b[0]);
			bb = std::move(b[3]);
			b[3] = std::move(b[5]);
			b[5] = std::move(b[4]);
			b[4] = std::move(bb);
		}
		else {
			target[0] = std::move(b[0]);
			if (less(b[2], b[1])) {
				t temp = std::move(b[1]);
				b[1] = std::move(b[2]);
				b[2] = std::move(temp);
				temp = std::move(b[4]);
				b[4] = std::move(b[5]);
				b[5] = std::move(temp);
			}
		}
		if (less(b[2], b[3])) {
			if (less(b[5], b[3])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[3]);
				b[3] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[3]);
			b[3] = std::move(b[2]);
			if (less(b[1], bb)) {
				b[2] = std::move(bb);
			}
			else {
				b[2] = std::move(b[1]);
				b[1] = std::move(bb);
				flag = true;
			}
		}
		if (less(b[3], b[4])) {
			if (less(b[5], b[4])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[4]);
				b[4] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[4]);
			b[4] = std::move(b[3]);
			if (flag || less(b[2], bb)) {
				b[3] = std::move(bb);
			}
			else {
				b[3] = std::move(b[2]);
				b[2] = std::move(bb);
			}
		}
		i = target;
		t* jj = b;
		*++i = std::move(*++jj);
		*++i = std::move(*++jj);
		*++i = std::move(*++jj);
		*++i = std::move(*++jj);
		*++i = std::move(*++jj);
	}

	template<typename iter, typename Compare>
	inline void sort7(iter begin, iter target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t bb;
		t b[7];
		bool flag = false;
		if (less(begin[0], begin[1])) {
			b[0] = std::move(begin[0]);
			b[3] = std::move(begin[1]);
		}
		else {
			b[3] = std::move(begin[0]);
			b[0] = std::move(begin[1]);
		}
		if (less(begin[2], begin[3])) {
			b[1] = std::move(begin[2]);
			b[4] = std::move(begin[3]);
		}
		else {
			b[4] = std::move(begin[2]);
			b[1] = std::move(begin[3]);
		}
		if (less(begin[4], begin[5])) {
			b[2] = std::move(begin[4]);
			b[5] = std::move(begin[5]);
		}
		else {
			b[5] = std::move(begin[4]);
			b[2] = std::move(begin[5]);
		}
		if (less(b[1], b[0])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(temp);
			temp = std::move(b[4]);
			b[4] = std::move(b[3]);
			b[3] = std::move(temp);
		}
		if (less(b[2], b[0])) {
			bb = std::move(b[2]);
			b[2] = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(bb);
			bb = std::move(b[3]);
			b[3] = std::move(b[5]);
			b[5] = std::move(b[4]);
			b[4] = std::move(bb);
		}
		else if (less(b[2], b[1])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[2]);
			b[2] = std::move(temp);
			temp = std::move(b[5]);
			b[5] = std::move(b[4]);
			b[4] = std::move(temp);
		}
		if (less(b[2], b[3])) {
			if (less(b[5], b[3])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[3]);
				b[3] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[3]);
			b[3] = std::move(b[2]);
			if (less(b[1], bb)) {
				b[2] = std::move(bb);
			}
			else {
				b[2] = std::move(b[1]);
				b[1] = std::move(bb);
				flag = true;
	}
}
		if (less(b[3], b[4])) {
			if (less(b[5], b[4])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[4]);
				b[4] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[4]);
			b[4] = std::move(b[3]);
			if (flag || less(b[2], bb)) {
				b[3] = std::move(bb);
			}
			else {
				b[3] = std::move(b[2]);
				b[2] = std::move(bb);
			}
		}
		bb = std::move(begin[6]);
		iter i = begin;
		iter j = target;
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		int index = 0;
		index += (less(b[3], bb)) << 2;
		index += (less(b[index + 1], bb)) << 1;
		index += (index < 6 && less(b[index], bb));
		target[index] = std::move(bb);
		i = target;
		t* jj = b;
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i + (index <= 0)) = std::move(*jj++);
  }

	template<typename iter, typename Compare>
	inline void sort8(iter begin, iter target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t bb;
		t b[8];
		bool flag = false;
		if (less(begin[0], begin[1])) {
			b[0] = std::move(begin[0]);
			b[4] = std::move(begin[1]);
		}
		else {
			b[4] = std::move(begin[0]);
			b[0] = std::move(begin[1]);
		}
		if (less(begin[2], begin[4])) {
			b[1] = std::move(begin[2]);
			b[5] = std::move(begin[4]);
		}
		else {
			b[5] = std::move(begin[2]);
			b[1] = std::move(begin[4]);
		}
		if (less(begin[5], begin[6])) {
			b[2] = std::move(begin[5]);
			b[6] = std::move(begin[6]);
		}
		else {
			b[6] = std::move(begin[5]);
			b[2] = std::move(begin[6]);
		}
		if (less(b[1], b[0])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(temp);
			temp = std::move(b[5]);
			b[5] = std::move(b[4]);
			b[4] = std::move(temp);
		}
		if (less(b[2], b[0])) {
			bb = std::move(b[2]);
			b[2] = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(bb);
			bb = std::move(b[4]);
			b[4] = std::move(b[6]);
			b[6] = std::move(b[5]);
			b[5] = std::move(bb);
		}
		else if (less(b[2], b[1])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[2]);
			b[2] = std::move(temp);
			temp = std::move(b[5]);
			b[5] = std::move(b[6]);
			b[6] = std::move(temp);
		}
		if (less(b[2], b[4])) {
			if (less(b[6], b[4])) {
				t temp = std::move(b[6]);
				b[6] = std::move(b[4]);
				b[4] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[4]);
			b[4] = std::move(b[2]);
			if (less(b[1], bb)) {
				b[2] = std::move(bb);
			}
			else {
				b[2] = std::move(b[1]);
				b[1] = std::move(bb);
				flag = true;
			}
		}
		if (less(b[4], b[5])) {
			if (less(b[6], b[5])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[6]);
				b[6] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[5]);
			b[5] = std::move(b[4]);
			if (flag || less(b[2], bb))
				b[4] = std::move(bb);
			else {
				b[4] = std::move(b[2]);
				b[2] = std::move(bb);
			}
		}
		bb = std::move(begin[3]);
		if (less(bb, b[2])) {
			b[3] = std::move(b[2]);
			if (less(bb, b[1])) {
				b[2] = std::move(b[1]);
				if (less(bb, b[0])) {
					b[1] = std::move(b[0]);
					b[0] = std::move(bb);
				}
				else
					b[1] = std::move(bb);
		}
			else
				b[2] = std::move(bb);
	}
		else {
			if (less(bb, b[5])) {
				if (less(bb, b[4])) {
					b[3] = std::move(bb);
				}
				else {
					b[3] = std::move(b[4]);
					b[4] = std::move(bb);
				}
			}
			else {
				b[3] = std::move(b[4]);
				b[4] = std::move(b[5]);
				if (less(bb, b[6])) {
					b[5] = std::move(bb);
				}
				else
				{
					b[5] = std::move(b[6]);
					b[6] = std::move(bb);
				}
			}
		}
		bb = std::move(begin[7]);
		iter i = begin;
		iter j = target;
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		int index = 0;
		index += (less(b[3], bb)) << 2;
		index += (less(b[index + 1], bb)) << 1;
		index += (less(b[index], bb));
		target[index] = std::move(bb);
		i = target;
		t* jj = b;
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i + (index <= 0)) = std::move(*jj++);
  }

	template<typename iter, typename Compare>
	inline void sort9(iter begin, iter target, Compare less) {
		typedef typename std::iterator_traits<iter>::value_type t;
		t bb;
		t b[9];
		bool flag = false;
		if (less(begin[0], begin[1])) {
			b[0] = std::move(begin[0]);
			b[5] = std::move(begin[1]);
		}
		else {
			b[5] = std::move(begin[0]);
			b[0] = std::move(begin[1]);
		}
		if (less(begin[2], begin[5])) {
			b[1] = std::move(begin[2]);
			b[6] = std::move(begin[5]);
		}
		else {
			b[6] = std::move(begin[2]);
			b[1] = std::move(begin[5]);
		}
		if (less(begin[6], begin[7])) {
			b[2] = std::move(begin[6]);
			b[7] = std::move(begin[7]);
		}
		else {
			b[7] = std::move(begin[6]);
			b[2] = std::move(begin[7]);
		}
		if (less(b[1], b[0])) {
			t temp = std::move(b[0]);
			b[0] = std::move(b[1]);
			b[1] = std::move(temp);
			temp = std::move(b[5]);
			b[5] = std::move(b[6]);
			b[6] = std::move(temp);
		}
		if (less(b[2], b[0])) {
			bb = std::move(b[2]);
			b[2] = std::move(b[1]);
			b[1] = std::move(b[0]);
			b[0] = std::move(bb);
			bb = std::move(b[5]);
			b[5] = std::move(b[7]);
			b[7] = std::move(b[6]);
			b[6] = std::move(bb);
		}
		else if (less(b[2], b[1])) {
			t temp = std::move(b[1]);
			b[1] = std::move(b[2]);
			b[2] = std::move(temp);
			temp = std::move(b[6]);
			b[6] = std::move(b[7]);
			b[7] = std::move(temp);
		}
		if (less(b[2], b[5])) {
			if (less(b[7], b[5])) {
				t temp = std::move(b[5]);
				b[5] = std::move(b[7]);
				b[7] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[5]);
			b[5] = std::move(b[2]);
			if (less(b[1], bb)) {
				b[2] = std::move(bb);
			}
			else {
				b[2] = std::move(b[1]);
				b[1] = std::move(bb);
				flag = true;
			}
		}
		if (less(b[5], b[6])) {
			if (less(b[7], b[6])) {
				t temp = std::move(b[6]);
				b[6] = std::move(b[7]);
				b[7] = std::move(temp);
			}
		}
		else {
			bb = std::move(b[6]);
			b[6] = std::move(b[5]);
			if (flag || less(b[2], bb)) {
				b[5] = std::move(bb);
			}
			else {
				b[5] = std::move(b[2]);
				b[2] = std::move(bb);
			}
		}
		bb = std::move(begin[3]);
		if (less(bb, b[2])) {
			b[3] = std::move(b[2]);
			if (less(bb, b[1])) {
				b[2] = std::move(b[1]);
				if (less(bb, b[0])) {
					b[1] = std::move(b[0]);
					b[0] = std::move(bb);
				}
				else {
					b[1] = std::move(bb);
				}
			}
			else {
				b[2] = std::move(bb);
			}
		}
		else {
			if (less(bb, b[6])) {
				if (less(bb, b[5])) {
					b[3] = std::move(bb);
				}
				else {
					b[3] = std::move(b[5]);
					b[5] = std::move(bb);
				}
			}
			else {
				b[3] = std::move(b[5]);
				b[5] = std::move(b[6]);
				if (less(bb, b[7])) {
					b[6] = std::move(bb);
				}
				else {
					b[6] = std::move(b[7]);
					b[7] = std::move(bb);
				}
			}
		}
		bb = std::move(begin[4]);
		if (less(bb, b[3])) {
			b[4] = std::move(b[3]);
			if (less(bb, b[1])) {
				b[3] = std::move(b[2]);
				b[2] = std::move(b[1]);
				if (less(bb, b[0])) {
					b[1] = std::move(b[0]);
					b[0] = std::move(bb);
				}
				else {
					b[1] = std::move(bb);
				}
			}
			else {
				if (less(bb, b[2])) {
					b[3] = std::move(b[2]);
					b[2] = std::move(bb);
				}
				else {
					b[3] = std::move(bb);
				}
			}
		}
		else {
			if (less(bb, b[6])) {
				if (less(bb, b[5])) {
					b[4] = std::move(bb);
				}
				else {
					b[4] = std::move(b[5]);
					b[5] = std::move(bb);
				}
			}
			else {
				b[4] = std::move(b[5]);
				b[5] = std::move(b[6]);
				if (less(bb, b[7])) {
					b[6] = std::move(bb);
				}
				else {
					b[6] = std::move(b[7]);
					b[7] = std::move(bb);
				}
			}
		}
		bb = std::move(begin[8]);
		iter i = begin;
		iter j = target;
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		*i++ = std::move(*j++);
		int index = 0;
		index += (less(b[3], bb)) << 2;
		index += (less(b[index + 1], bb)) << 1;
		index += (less(b[index], bb));
		index += (index == 7 && less(b[7], bb));
		target[index] = std::move(bb);
		i = target;
		t* jj = b;
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i++ + (index-- <= 0)) = std::move(*jj++);
		*(i + (index <= 0)) = std::move(*jj++);
	}


}