#pragma once

namespace mergeinsertion_special2 {
    int greatestpowerof2(unsigned int n)
    {
        unsigned int m = n;
        unsigned int result = 1;
        while (m > 1)
        {
            m >>= 1;
            result <<= 1;
        }
        return result;
    }

    template<class it, class Compare> class weakheap
    {
        Compare less;
    public:
        using t = typename std::iterator_traits<it>::value_type;
        weakheap(it _root, it _final, Compare cmp) :
            root(_root), final(_final - _root + 1)
        {
            less = cmp;
            reverse = std::vector<bool>(this->final, false);
            a = std::vector<int>(2 * this->final);
            gpar = std::vector<int>(2 * this->final);
            for (int i = 0; i < 2 * final; i++)
                a[i] = i;
        }
        void heapify(int m)
        {
            int k = m;
            for (int i = m - 1; i >= 1; i--)
            {
                if (i + 1 == k)
                {
                    if (k % 2 == 0)
                    {
                        merge(gparent(i), i);
                    }
                    k /= 2;
                }
                else
                {
                    merge(gparent(i), i);
                }
            }
        }
        int insert(int from, int dist, int to)
        {
            /*		std::cout << "insert [" << from << "+ " << dist << "=" << from + dist
            << "] @:" << to << " l: "<< gpar[to] + 1  << std::endl;*/
            for (int j = to; j > from + dist; --j)
            {
                std::swap(a[j - 1], a[j]);
                std::swap(gpar[j - 1], gpar[j]);
            }
            int l = from;
            l = gpar[from + dist] + 1;
            int r = from + dist;
            while (l < r)
            {
                /*	     	int n = r - l;
                int gp = greatestpowerof2(n + 1);
                int m;
                if (n - gp / 2 < gp)
                m = r - gp / 2;
                else
                m = l + gp - 1;*/
                int m = (l + r) / 2;//- rand()%2
                                    //			int m = (l + r) / 2;
                if (comp(*(root + a[from + dist]), *(root + a[m])))
                    l = m + 1;
                else
                    r = m;
            }
            for (int j = from + dist; j > l; --j)
            {
                std::swap(a[j - 1], a[j]);
                std::swap(gpar[j - 1], gpar[j]);
 //               mimoves++;
            }
            return l;
        }

        void mi(int m)
        {
            //		std::cout << "---------------  merge insert -------" << m << std::endl;
            int odd = m % 2;
            int n = m - odd;
            for (int t = 0; t < m / 2; t++)
            {
                a[n - t - 1] = gchild(a[t], n);
                gpar[n - t - 1] = t;
            }
            if (m % 2 == 1)
                gpar[m - 1] = -1;
            int k = 1, c = 0, e = 1 << k;
            int sumfis = 0, f = 0;
            int fnext = 2;
            while (e < m)
            {
                f = fnext;
                k++;
                fnext = (((1 << (k + 1)) + (k % 2 ? -1 : 1)) / 3) + 1;
                int fis = fnext - f;
                int last = m / 2 + sumfis + odd;
                sumfis += fis;
                e <<= 1;
                for (int i = 0; (i < fis) && (++c < m / 2 + odd); i++)
                {
                    int l;
                    if (fnext > m / 2 + odd)
                    {
                        l = insert(i + 1 - odd, last, m - 1);
                    }
                    else
                    {
                        l = insert(m / 2 - (sumfis)+i, e - 1, m / 2 + (sumfis));
                    }
                    for (int j = m / 2 - (sumfis)+i + e; j < m; j++)
                    {
                        if (gpar[j] >= l)
                            gpar[j]++;
                    }
                }
                //			std::cout << std::endl;
            }
        }

        void mirec(int k)
        {
            if (k <= 2)
                return;
            mirec(k / 2);
            mi(k);
        }


        void mergeinsertion()
        {
            heapify(final);
 //           print();
            mirec(final);
 //           print();
        }

        void mergeinsertion(it target)
        {
            heapify(final);
            mirec(final);
            for (int i = 0; i < final; i++)
            {
                target[i] = std::move(root[a[i]]);
            }
        }

        void insertionsort()
        {
            for (int i = 1; i < final; i++)
            {
                int l = 0;
                int r = i;
                while (l < r)
                {
                    int m = (l + r) / 2;
                    if (comp(*(root + a[i]), *(root + a[m])))
                        l = m + 1;
                    else
                        r = m;
                }
                for (int j = i; j > l; --j)
                {
#ifdef COUNT_ISMOVES
                    ismoves++;
#endif // ISMOVES

                    
                    std::swap(a[j - 1], a[j]);
                }
            }
        }
        void print(void)
        {
            for (int i = 0; i < final; i++)
                std::cout << " a[" << i << "]:" << (int)a[i];
            std::cout << std::endl;
            for (int i = 0; i < final; i++)
                std::cout << " v[" << i << "]:" << (int)(*(root + a[i]));
            std::cout << std::endl;
            for (int i = 0; i < final; i++)
                std::cout << " r[" << i << "]:" << (int)(reverse[i]);
            std::cout << std::endl;
       //     std::cout << " number of comparisons = " << t::count() << std::endl;
            std::cout << std::endl;
        }

        bool is_sorted()
        {
            bool ret = true;
            for (int i = 1; i < final; i++)
            {
                //			std::cout <<  (*(root + a[i -1])).value() <<", ";
                if ((*(root + a[i - 1])) > (*(root + a[i])))
                    ret = false;
            }
            //		std::cout <<  (*(root + a[final - 1])).value() <<", ";
            //		std::cout<<std::endl;
            if (!ret)	std::cout << "eror" << std::endl;
            return ret;
        }

    protected:
        std::vector<bool> reverse;
        std::vector<int> a;
        std::vector<int> gpar;

        const it root;
        int final;
        virtual inline int first_child(int i)
        {
            return i + reverse[i] + i;
        }
        virtual inline int second_child(int i)
        {
            return i - (reverse[i] == true) + i + 1;
        }
        inline void merge(int i, int j)
        {
            if (comp(*(root + i), *(root + j)))
            {
                std::swap(*(root + i), *(root + j));
                reverse[j] = 1 - reverse[j];
            }
        }
        inline void change(int i, int j)
        {
            if (comp(*(root + a[i]), *(root + a[j])))
                std::swap(a[i], a[j]);
        }
        inline int gparent(int j)
        {
            while ((j & 1) == 0)
                j = parent(j);
            return parent(j);
        }
        inline int gchild(int f, int level)
        {
            int x = second_child(f);
            while (first_child(x) < level)
                x = first_child(x);
            return x;
        }
        inline int parent(int i)
        {
            return i >> 1;
        }
        inline bool comp(const t &a, const t &b)
        {
            return less(b, a);
        }
};


    template<typename iter, typename Compare>
    void sort(iter begin, iter end, Compare less) {
        using t = typename std::iterator_traits<iter>::value_type;
        std::vector<t> tempvec(begin, end);

        weakheap<iter, Compare> h(tempvec.begin(), tempvec.end() - 1, less);
        //       std::copy(target, target + n, begin);

        h.mergeinsertion(begin);
    }

    template<typename T>
    void sort(std::vector<T> &v) {
        typename std::vector<T>::iterator begin = v.begin();
        typename std::vector<T>::iterator end = v.end();
        mergeinsertion_special2::sort(begin, end, std::less<T>());
    }

}