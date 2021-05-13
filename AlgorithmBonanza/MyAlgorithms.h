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

    template<typename Iter, typename T>
    Iter lower_bound(Iter left, Iter right, const T& value) {
        while (left < right) {
            auto mid = std::advance(left, std::distance(left, right) / 2);
            if (*mid == value) {
                return mid;
            }
            else if (*mid > value) {
                left = std::next(mid);
            }
            else {
                right = mid;
            }
        }
        return right;
    }

    template<typename Iter, typename T>
    Iter binary_search(Iter left, Iter right, const T& value) {
        auto it = my::lower_bound(left, right, value);
        if (*it != right && *it == value) { return it; }
        else { return right; }
    }

    template<typename Iter, typename T, typename binaryPredicate>
    Iter binary_search(Iter left, Iter right, const T& value, binaryPredicate func) {
        Iter mid;
        while (left <= right - 1) {
            mid = left + (right - left - 1) / 2;
            if (*mid == value) {
                return mid;
            }
            if (func(*mid, value)) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return right;
    }

    template<typename Iter>
    int count(Iter first, Iter second) {
        int count = 0;
        for (; first < second; first++) {
            count++;
        }
        return count;
    }

    template<typename Iter, typename UnaryPredicate>
    int count_if(Iter first, Iter second, UnaryPredicate func) {
        int count = 0;
        for (; first < second; first++) {
            if (func(*first)) {
                count++;
            }
        }
        return count;
    }
    
    template<typename InputIter, typename OutputIter>
    void merge(InputIter lefta, InputIter righta,
        InputIter leftb, InputIter rightb,
        OutputIter target) {
        for (; lefta != righta; ++target) {
            if (leftb == rightb || *lefta < *leftb) {
                *target = *lefta;
                ++lefta;
            }
            else {
                *target = *leftb;
                ++leftb;
            }
        }
    }

   template<typename Iter>
   bool is_sorted(Iter first, Iter last) {
        if (first == last) { return true; }
        Iter next = first;
        while (++next != last) {
            if (*next < *first)
                return false;
            ++first;
        }
        return true;
    }
    
   template<typename Iter, typename T>
    void partition(Iter left, Iter right, const T& value) {
        while (left != right && *left < value) {
            ++left;
        }
        if (left == right) return;

        for (Iter swapFrom = std::next(left); swapFrom != right; ++swapFrom) {
            if (*swapFrom < value) {
                auto *temp = left;
                *left = *swapFrom;
                *swapFrom = *temp;
                ++left;
            }
        }

        for (Iter pivotIter = std::next(left); pivotIter != right; ++pivotIter) {
            auto *temp = left;
            *left = *pivotIter;
            *pivotIter = *temp;
            break;
        }
    }

}