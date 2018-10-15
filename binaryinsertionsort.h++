#pragma once
#include<algorithm>

namespace binaryinsertionsort {

    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) 
    {
        int number = end - begin;
        for (int i = 1; i < number; i++)
        {
            int l = 0;
            int r = i;
            while (l < r) {
                int m = (l + r) / 2;
                if (less(begin[m],begin[i]))
                    l = m + 1;
                else
                    r = m;
            }
            for (int j = i; j > l; --j)
                std::swap(begin[j - 1], begin[j]);

        }
    }

    template<typename iter, typename Compare>
    void sortswap(iter begin, iter end, iter target, Compare less)
    {
        using t = typename std::iterator_traits<iter>::value_type;
        int number = end - begin;
        for (int i = 1; i < number; i++)
        {
            int l = 0;
            int r = i;
            while (l < r) {
                int m = (l + r) / 2;
                if (less(target[m], begin[i]))
                    l = m + 1;
                else
                    r = m;
            }
            t temp = std::move(target[i]);
            for (int j = i; j > l; --j)
                target[j] = std::move(target[j-1]);
            target[l] = std::move(begin[i]);
            begin[i] = std::move(temp);

        }
    }



    template<typename T>
    void sort(std::vector<T> &v) {
        typename std::vector<T>::iterator begin = v.begin();
        typename std::vector<T>::iterator end = v.end();
        binaryinsertionsort::sort(begin, end, std::less<T>());
    }

}
