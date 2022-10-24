#pragma once

#include "basic.cc"
#include "array.cc"


namespace cp {

    template <typename T>
    T sum(T* begin, T* end) {
        T out_sum = 0;
        for (T* p = begin; p <= end; p++) {
            out_sum += *p;
        }
        return out_sum;
    }

    template <typename T>
    T sum_lmd(T* begin, T* end, T& (*access_lmd)(T*)) {
        T out_sum = 0;
        for (T* p = begin; p <= end; p++) {
            out_sum += access_lmd(p);
        }
        return out_sum;
    }


    template <typename T>
    void bubble_sort(T* begin, T* end, bool (*less_lmd)(T*, T*)) {
        bool was_permutations = false;
        do {
            was_permutations = false;
            for (T* p = begin; p < end - 1; p++) {
                if (less_lmd(p + 1, p)) {
                    swap(p, p + 1);
                    was_permutations = true;
                }
            }
        } while (was_permutations);
    }
    
    template <typename T>
    void qsort(T* first, T* last, bool (*greater_lmd)(T*, T*)) {

        struct BoundPack {
            T* first;
            T* last;
        };

        darr<BoundPack> stack;
        stack.init(0);
        stack.dpush({first, last});
    }

    template <typename t_iter> 
    t_iter find(t_iter it_begin, t_iter it_end, typename t_iter::type data) {
        for (auto it = it_begin; it != it_end; it++) {
            if (*it == data) {
                return it;
            }
        }
        return it_end;
    }

    template <typename t_iter, typename t_eq_fn> 
    t_iter find(t_iter it_begin, t_iter it_end, typename t_iter::type data, t_eq_fn eq_lmd) 
    {
        for (auto it = it_begin; it != it_end; it++) {
            if (eq_lmd(*it,data)) {
                return it;
            }
        }
        return it_end;
    }

    template <typename t_iterable> 
    typename t_iterable::iter 
    find(t_iterable &self, typename t_iterable::type data) {
        return find(begin(self), end(self), data);
    }

    template <typename T>
    bool default_eq_lmd(T f, T s) {
        return f == s;
    }

}
