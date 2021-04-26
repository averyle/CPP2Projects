#pragma once

namespace my {
    template<typename Iter, typename T>
    Iter find(Iter it, Iter end, const T& value) { 
        for (; it < end; it++) {
            if (*it == value) {
                return it;
            }
       }
        return end;
    }

    template<typename Iter, typename Predicate>
    Iter find_if(Iter it, Iter end, Predicate func) {
        for (; it < end; it++) {
            if (func(*it)) {
                return it;
            }
        }
        return end;
    }

    template<typename Iter>
    Iter min_element(Iter it, Iter end) {
        if (it == end) { return end; }
        auto min = it;
        for (; it < end; ++it) {
            if (*it < *min) {
                min = it;
            }
        }
        return min;
    }

    template<typename Iter, typename binaryPredicate>
    Iter min_element_when(Iter it, Iter end, binaryPredicate func) {
        if (it == end) { return end; }
        Iter min = it;
        ++min;
        for (; it < end; ++it) {
            if (func(*it, *min)) {
                min = it;
            }
        }
        return min;
    }

    template<typename Iter>
    Iter max_element(Iter it, Iter end) {
        if (it == end) { return end; }
        auto max = it;
        for (; it < end; ++it) {
            if (*it > *max) {
                max = it;
            }
        }
        return max;
    }

    template<typename Iter, typename binaryPredicate>
    Iter max_element_when(Iter it, Iter end, binaryPredicate func) {
        if (it == end) { return end; }
        Iter max = it;
        ++max;
        for (; it < end; ++it) {
            if (func(*it, *max)) {
                max = it;
            }
        }
        return max;
    }

    template<typename Iter>
    std::pair<int, int> minmax_element(Iter it, Iter end) {
        if (it == end) { return std::make_pair(*it, *end); }
        auto min = my::min_element(it, end);
        auto max = my::max_element(it, end);
        return std::make_pair(*min, *max);
    }

    /*template<typename Iter, typename T>
    Iter lower_bound(Iter left, Iter right, const T& value) { ... }

    template<typename Iter, typename T>
    Iter binary_search(Iter left, Iter right, const T& value) { ... }

    template<typename InputIter, typename OutputIter>
    void merge(InputIter lefta, InputIter righta,
        InputIter leftb, InputIter rightb,
        OutputIter target) {
        ...
    }

    template<typename Iter, typename T>
    void partition(Iter left, Iter right, const T& value) { ... }*/
}