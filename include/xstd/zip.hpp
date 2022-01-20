/**
 * \file       zip.hpp
 * \author     Bryan Flynt
 * \date       Jan 15, 2022
 */
#pragma once

#include <tuple>
#include <utility>

#include "algorithm.hpp"  // xstd::for_each

namespace xstd {

/** Iterator for zipped collection
 *
 * The actual iterator which is used to iterate over
 * all containers within the collection. When dereferenced
 * it returns a tuple of elements at the current position.
 * Since the collections might be of different lengths, this
 * iterator stops when the collection with the fewest elements
 * is exhausted.
 *
 * \tparam Iterators The iterator types to hold
 *
 * \code{.cpp}
 * const int N = 100;
 * std::vector<int> a(N);
 * std::vector<int> b(N);
 * std::for_each( zip_iterator(a.begin(),b.begin()),
 *                zip_iterator(a.end(),b.end()),
 *                [](auto vec_pair){
 *                    std::get<0>(vec_pair) = std::get<1>(vec_pair);
 *                });
 * \endcode
 *
 * Has the same behavior as
 *
 * \code{.cpp}
 * const int N = 100;
 * std::vector<int> a(N);
 * std::vector<int> b(N);
 * for(int i = 0; i < vec.size(); ++i){
 * 	   a[i] = b[i];
 * }
 * \endcode
 */
template <typename... Iterators>
struct zip_iterator {
    // ====================================================
    // Types
    // ====================================================

    using iterator_tuple          = std::tuple<Iterators...>;
    using difference_tuple        = std::tuple<typename std::iterator_traits<Iterators>::difference_type...>;
    using value_tuple             = std::tuple<typename std::iterator_traits<Iterators>::value_type...>;
    using pointer_tuple           = std::tuple<typename std::iterator_traits<Iterators>::pointer...>;
    using reference_tuple         = std::tuple<typename std::iterator_traits<Iterators>::reference...>;
    using iterator_category_tuple = std::tuple<typename std::iterator_traits<Iterators>::iterator_category...>;

    using difference_type   = std::tuple_element_t<0, difference_tuple>;
    using value_type        = value_tuple;
    using pointer           = pointer_tuple;
    using reference         = reference_tuple;
    using iterator_category = std::common_type_t<typename std::iterator_traits<Iterators>::iterator_category...>;

    // The standard states if an iterator wants to be categorized as forward iterator or above the
    // reference must be either value_type& or const value_type&. Based on this constraint, the most powerful
    // legal iterator_category for this iterator is actually std::input_iterator_tag. (Which in turn means
    // that std::sort won't work at all, as it requires random access iterators.)
    // Other iterators suffer from the same issue:
    // std::vector<bool>::iterator
    // boost::zip_iterator

    // ====================================================
    // Constructors
    // ====================================================

    zip_iterator(Iterators&&... iterators) : iterators_(std::make_tuple(iterators...)) {}

    // ====================================================
    // Operators
    // ====================================================

    zip_iterator& operator++() {
        xstd::for_each(iterators_, [](auto& iter) { iter++; });
        return *this;
    }

    zip_iterator operator++(int) {
        auto tmp = *this;
        (*this)++;
        return tmp;
    }

    zip_iterator& operator+=(const difference_type& inc) {
        xstd::for_each(iterators_, [inc](auto& iter) { iter += inc; });
        return *this;
    }

    zip_iterator& operator--() {
        xstd::for_each(iterators_, [](auto& iter) { iter--; });
        return *this;
    }

    zip_iterator operator--(int) {
        auto tmp = *this;
        (*this)--;
        return tmp;
    }

    zip_iterator& operator-=(const difference_type& inc) {
        xstd::for_each(iterators_, [inc](auto& iter) { iter -= inc; });
        return *this;
    }

    value_tuple operator[](const difference_type n) {
        return xstd::transform(iterators_, [n](auto iter) -> decltype(*iter)& { return iter[n]; });
    }

    reference_tuple operator*() {
        return xstd::transform(iterators_, [](auto iter) -> decltype(*iter)& { return *iter; });
    }

    // ====================================================
    // Friend Operators
    // ====================================================

    friend bool operator==(const zip_iterator& x, const zip_iterator& y) {
        return xstd::any_of(x.iterators_, y.iterators_, [](const auto& a, const auto& b) {
            return (a == b);  // We use any_of to stop at first match
        });
    }

    friend bool operator!=(const zip_iterator& x, const zip_iterator& y) { return not(x == y); }

    friend bool operator<(const zip_iterator& x, const zip_iterator& y) {
        return xstd::all_of(x.iterators_, y.iterators_, [](const auto& a, const auto& b) { return (a < b); });
    }

    friend difference_type operator-(const zip_iterator& x, const zip_iterator& y) {
        auto diff_tuple =
            xstd::transform(x.iterators_, y.iterators_, [](const auto& a, const auto& b) { return a - b; });
        return xstd::min(diff_tuple);  // Return minimum distance
    }

    friend zip_iterator operator+(zip_iterator x, difference_type y) { return x += y; }

    friend zip_iterator operator+(difference_type x, zip_iterator y) { return y += x; }

   private:
    iterator_tuple iterators_;
};

/** Proxy class returned by zip function
 *
 * Proxy class that implements the begin() and end()
 * methods used by range based for loops.  Users should
 * not need to use this method unless they are manually
 * iterating over zipped containers.
 */
template <typename ZipIterator>
struct zip_proxy {
    /** Constructor
     *
     * Takes variable arguments of individual begin and end iterators
     */
    zip_proxy(const ZipIterator& first, const ZipIterator& last) : first_(first), last_(last) {}

    /** Returns iterator to start of zipped containers
     */
    ZipIterator begin() { return first_; }

    /** Returns iterator one past end of zipped containers
     */
    ZipIterator end() { return last_; }

   private:
    ZipIterator first_;
    ZipIterator last_;
};

/** Zips multiple containers together for iteration
 *
 * Creates a zip_proxy object that a range based for loop
 * can use to loop over a collection simultaneously.
 *
 * Example of iterating over all elements:
 * \code{.cpp}
 * const int N = 100;
 * std::vector<atype> a(N);
 * std::list<btype>   b(N);
 * for(auto i : zip(a,b)){
 *    atype val_a = std::get<0>(i);
 *    btype val_b = std::get<1>(i);
 * }
 * \endcode
 *
 * To iterate over an increment other than 1:
 * \code{.cpp}
 * int increment = 2;
 * const int N = 100;
 * std::vector<atype>     a(N);
 * std::list<btype>       b(N);
 * std::array<ctype,100>  c(N);
 * for(auto i : zip(a,b,c).step(increment)){
 *    atype val_a = std::get<0>(i);
 *    btype val_b = std::get<1>(i);
 *    ctype val_c = std::get<2>(i);
 * }
 * \endcode
 *
 * \param args The collections to iterator over
 * \tparam Args Types for each collection
 * \returns A zip_iterator over all the collections
 */
template <typename... Args>
auto zip(Args&&... args) {
    return zip_proxy(zip_iterator(std::begin(args)...), zip_iterator(std::end(args)...));
}

} /* namespace xstd */
