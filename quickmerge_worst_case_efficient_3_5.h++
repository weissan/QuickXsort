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



#include "insertionsort.h"
#include "median_of_medians.h"
#include "partition_stl.h"
#include "smallsort.h"

#include "quickmerge_worst_case.h"

namespace quickmerge_worst_case_efficient_3_5 {
	template<typename iter, typename Compare>
	void sort(iter begin, iter end, Compare less) {
		quickmerge::sort_worst_case_efficient_3_5(begin, end, less);
	}
	template<typename T>
	void sort(std::vector<T> &v) {
		typename std::vector<T>::iterator begin = v.begin();
		typename std::vector<T>::iterator end = v.end();
		//   quicksort::qsort_blocked(begin,end);
		quickmerge::sort_worst_case_efficient_3_5(begin, end, std::less<T>());
	}
}