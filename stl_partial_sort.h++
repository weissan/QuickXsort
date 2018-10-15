#pragma once
namespace stl_partial_sort {
    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        std::partial_sort(begin, end,end, less);
    }

    template<typename T>
    void sort(std::vector<T> &v) {
        std::partial_sort(v.begin(), v.end());
    }

}