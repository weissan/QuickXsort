#pragma once
namespace stl_stable_sort {
	template<typename iter, typename Compare>
	void sort(iter begin, iter end, Compare less) {
		std::stable_sort(begin, end, less);
	}

	template<typename T>
	void sort(std::vector<T> &v) {
		std::stable_sort(v.begin(), v.end());
	}

}