/**
 * \file       strided.hpp
 * \author     Bryan Flynt
 * \date       Jan 15, 2022
 */
#pragma once

#include <cassert>   // assert
#include <cstddef>   // std::size_t, std::ptrdiff_t;
#include <iterator>  // std::random_access_iterator_tag
#include <utility>   // std::declval

namespace xstd {

/** Strided Iterator
 *
 * Iterator which takes strided steps across a provided
 * iterator from another type.
 *
 * \tparam Iterator Type of iterator to be wrapped with strided_iterator
 *
 * \code{.cpp}
 * using vector_type     = std::vector<int>;
 * using difference_type = typename vector_type::difference_type;
 * using iterator_type   = typename vector_type::iterator;
 *
 * const difference_type N      = 100;
 * const difference_type stride = 5;
 * vector_type a(N);
 * vector_type b;
 * std::copy(strided_iterator<iterator_type>(a.begin(), stride),
 *           strided_iterator<iterator_type>(a.end(), stride),
 *           std::back_inserter(b));
 * \endcode
 */
template <typename Iterator>
struct strided_iterator {
    // ====================================================
    // Types
    // ====================================================

    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;
    //using const_reference   = typename std::iterator_traits<Iterator>::const_reference;

    // ====================================================
    // Constructors
    // ====================================================

    strided_iterator(Iterator iterator, const difference_type stride) : iterator_(iterator), stride_(stride) {
        assert(stride != 0);
    }

    strided_iterator(const strided_iterator<Iterator>& other) : iterator_(other.iterator_), stride_(other.stride_) {}

    // ====================================================
    // Operators
    // ====================================================

    strided_iterator& operator=(strided_iterator other) {
        assert(stride_ == other.stride_);
        std::swap(iterator_, other.iterator_);
        return *this;
    }

    strided_iterator& operator++() {
        std::advance(iterator_, stride_);
        return *this;
    }

    strided_iterator operator++(int) {
        auto tmp = *this;
        std::advance(iterator_, stride_);
        return tmp;
    }

    strided_iterator& operator+=(const difference_type& inc) {
        std::advance(iterator_, inc * stride_);
        return *this;
    }

    strided_iterator& operator--() {
        std::advance(iterator_, -stride_);
        return *this;
    }

    strided_iterator operator--(int) {
        auto tmp = *this;
        std::advance(iterator_, -stride_);
        return tmp;
    }

    strided_iterator& operator-=(const difference_type& inc) {
        std::advance(iterator_, -inc * stride_);
        return *this;
    }

    reference operator[](const difference_type n) {
        auto tmp = iterator_;
        std::advance(tmp, n * stride_);
        return *tmp;
    }

    const reference operator[](const difference_type n) const {
        auto tmp = iterator_;
        std::advance(tmp, n * stride_);
        return *tmp;
    }

    reference operator*() { return *iterator_; }
    const reference operator*() const { return *iterator_; }

    // ====================================================
    // Friend Operators
    // ====================================================

    friend bool operator==(const strided_iterator& x, const strided_iterator& y) {
        assert(x.stride_ == y.stride_);
        return ((x - y) == 0);
    }

    friend bool operator!=(const strided_iterator& x, const strided_iterator& y) {
        assert(x.stride_ == y.stride_);
        return not(x == y);
    }

    friend bool operator<(const strided_iterator& x, const strided_iterator& y) {
        assert(x.stride_ == y.stride_);
        return ((std::distance(x.iterator_, y.iterator_) / x.stride_) > 0);
    }

    friend difference_type operator-(const strided_iterator& x, const strided_iterator& y) {
        assert(x.stride_ == y.stride_);
        return (std::distance(y.iterator_, x.iterator_) / x.stride_);
    }

    friend strided_iterator operator+(strided_iterator x, difference_type y) { return x += y; }

    friend strided_iterator operator+(difference_type x, strided_iterator y) { return y += x; }

   private:
    Iterator iterator_;
    const difference_type stride_;
};

/**
 * @brief
 * Proxy returned by strided function
 *
 * This is the class returned by the strided() function
 * within a for loop.
 */
template <typename Iterator>
struct strided_proxy {
    // ====================================================
    // Types
    // ====================================================

    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;

    strided_proxy() = delete;

    strided_proxy(Iterator first, Iterator last, difference_type stride)
        : first_(first), last_(last), stride_(stride) {}

    ~strided_proxy() = default;

    auto begin() const { return strided_iterator<Iterator>(first_, stride_); }

    auto end() const { return strided_iterator<Iterator>(last_, stride_); }

    auto cbegin() const { return strided_iterator<Iterator>(first_, stride_); }

    auto cend() const { return strided_iterator<Iterator>(last_, stride_); }

    difference_type size() const { return std::distance(first_, last_) / stride_; }

    difference_type stride() const { return stride_; }

   private:
    const Iterator first_;
    const Iterator last_;
    const difference_type stride_;
};  // struct strided_proxy

/**
 * @brief
 * Strided function from first to last of provided iterators
 *
 * @details
 * Allows the usage of a strided range based for loop over
 * a subset of iterator values.
 *
 * \code{.cpp}
 * const int stride = 10;
 * std::vector<int> a(100, 5);
 * for (auto i : strided(a.begin(), a.end(), stride)){
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 * is equivalent to
 *
 * \code{.cpp}
 * const int stride = 10;
 * std::vector<int> a(100, 5);
 * for(auto i = 0; i < s.size(); i+=stride) {
 *    cout << i << "\n";
 * }
 * \lastcode
 */
template <typename Iterator, typename Difference = typename std::iterator_traits<Iterator>::difference_type>
strided_proxy<Iterator> strided(Iterator first, Iterator last, Difference stride = 1) {
    assert(stride != 0);
    return {first, last, stride};
}

/**
 * @brief
 * Strided function container
 *
 * @details
 * Allows the usage of a strided range based for loop
 * for whole container.
 *
 * \code{.cpp}
 * const int stride = 10;
 * std::vector<int> a(100, 5);
 * for (auto i : strided(a, stride)){
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 * is equivalent to
 *
 * \code{.cpp}
 * const int stride = 10;
 * std::vector<int> a(100, 5);
 * for(auto i = 0; i < s.size(); i+=stride) {
 *    cout << i << "\n";
 * }
 * \lastcode
 */
// NOTE: This template signiture was so I didn't need to copy Container checking tools
template <typename Container,
          typename = typename std::iterator_traits<decltype(std::declval<Container>().begin())>::difference_type>
auto strided(Container& content, typename std::iterator_traits<decltype(content.begin())>::difference_type stride)
    -> strided_proxy<decltype(content.begin())> {
    return strided(std::begin(content), std::end(content), stride);
}

} /* namespace xstd */
