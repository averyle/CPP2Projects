#pragma once
#include <algorithm>

namespace my {
    struct sortAnalytics {
        int comparisons = 0;
        int swaps = 0;
        void reset();
    };

    template<typename Iter, typename UnaryPredicate>
    Iter partition(Iter swapTo, Iter end, UnaryPredicate criteria, sortAnalytics& analytics) {
        while (swapTo < end) {
            if (criteria(*swapTo)) {
                ++swapTo;
            }
            else {
                break;
            }
        }
        if (swapTo == end) return swapTo;

        for (Iter swapFrom = std::next(swapTo); swapFrom < end; ++swapFrom) {
            analytics.comparisons++;
            if (criteria(*swapFrom)) {
                analytics.swaps++;
                std::iter_swap(swapTo, swapFrom);
                ++swapTo;
            }
        }

        return swapTo;
    }

    template<typename InputItr, typename OutputItr>
    void merge(InputItr aSource, InputItr aEnd, InputItr bSource, InputItr bEnd, OutputItr dest, sortAnalytics& analytics) {
        for (; aSource != aEnd; ++dest) {
            if (bSource == bEnd) {
                std::copy(aSource, aEnd, dest);
                break;
            }

            analytics.comparisons++;
            if (*aSource < *bSource) {
                *dest = *aSource;
                ++aSource;
            }
            else {
                *dest = *bSource;
                ++bSource;
            }
        }
        std::copy(bSource, bEnd, dest);
    }

    template<typename Iter>
    void selection_sort(Iter swapTo, Iter end, sortAnalytics& analytics) {
        for (; swapTo != end; ++swapTo) {
            Iter smallest = swapTo;
            for (Iter cursor = smallest; cursor != end; ++cursor) {
                analytics.comparisons++;
                if (*cursor < *smallest) {
                    smallest = cursor;
                }
            }

            if (smallest != end) {
                analytics.swaps++;
                std::iter_swap(swapTo, smallest);
            }
        }
    }

    template<typename Iter>
    void insertion_sort(Iter first, Iter end, sortAnalytics& analytics) {
        if (first == end) return;

        for (Iter nextElement = std::next(first); nextElement != end; ++nextElement) {
            for (Iter cursor = nextElement; cursor > first; --cursor) {
                Iter prev = std::prev(cursor);
                analytics.comparisons++;
                if (*cursor < *prev) {
                    analytics.swaps++;
                    std::iter_swap(cursor, prev);
                }
                else {
                    break;
                }
            }
        }
    }

    template<typename Iter>
    void merge_sort(Iter first, Iter end, sortAnalytics& analytics) {
        if (std::distance(first, end) <= 1) return;

        Iter middle = std::next(first, std::distance(first, end) / 2);
        std::vector<typename std::iterator_traits<Iter>::value_type> scratch;

        my::merge_sort(first, middle, analytics);
        my::merge_sort(middle, end, analytics);

        my::merge(first, middle, middle, end, std::back_inserter(scratch), analytics);
        analytics.swaps += std::distance(std::begin(scratch), std::end(scratch));
        std::copy(std::begin(scratch), std::end(scratch), first);
    }

    template<typename Iter>
    void quick_sort(Iter first, Iter end, sortAnalytics& analytics) {
        if (std::distance(first, end) <= 1) return;
        Iter pivotItr = std::next(first, std::distance(first, end) / 2);
        Iter leftCursor = my::partition(first, end, [&pivotItr](const typename std::iterator_traits<Iter>::value_type& item) {
            return item < *pivotItr;
            }, analytics);
        Iter rightCursor = my::partition(leftCursor, end, [&pivotItr](const typename std::iterator_traits<Iter>::value_type& item) {
            return item == *pivotItr;
            }, analytics);

        my::quick_sort(first, leftCursor, analytics);
        my::quick_sort(rightCursor, end, analytics);
    }

    template<typename Iter>
    void bubble_sort(Iter first, Iter end, sortAnalytics& analytics) {
        bool swapped = true;
        while (first != end-- && swapped) {
            swapped = false;
            for (auto i = first; i != end; ++i) {
                analytics.comparisons++;
                if (*(i + 1) < *i) {
                    analytics.swaps++;
                    std::iter_swap(i, i + 1);
                    swapped = true;
                }
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, const my::sortAnalytics& analytics) {
    out << analytics.comparisons << ", " << analytics.swaps;
    return out;
}

std::vector<int> clone(std::vector<int>& a) {
    std::vector<int> clone;
    std::copy(std::begin(a), std::end(a), std::back_inserter(clone));
    return clone;
}

void reset(my::sortAnalytics& a) {
    a.comparisons = 0;
    a.swaps = 0;
}