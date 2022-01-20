/**
 * \file       range.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */
#pragma once

#include <cassert>   // assert
#include <cstddef>   // std::size_t, std::ptrdiff_t;
#include <iterator>  // std::random_access_iterator_tag

namespace xstd {

/** Range Iterator
 *
 * Iterator which contains a value that increments/decrements
 * as the operator is called.  Useful for filling arrays or
 * enumerating other iterators.
 *
 * \tparam Incrementable Value type to be incremented by iterator
 *
 * \code{.cpp}
 * const int N = 100;
 * std::vector<int> vec(N);
 * std::copy(range_iterator<int>(0),
 *           range_iterator<int>(N),
 *           vec.begin();
 * \endcode
 */
template <typename Incrementable>
struct range_iterator {
    // ====================================================
    // Types
    // ====================================================

    using difference_type   = std::ptrdiff_t;
    using value_type        = Incrementable;
    using pointer           = value_type*;
    using reference         = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    // ====================================================
    // Constructors
    // ====================================================

    range_iterator() : value_(0), step_(1) {}

    range_iterator(const range_iterator& other) : value_(other.value_), step_(other.step_) {}

    explicit range_iterator(const value_type& value) : value_(value), step_(1) {}

    explicit range_iterator(const value_type& value, const value_type& step) : value_(value), step_(step) {}

    // ====================================================
    // Operators
    // ====================================================

    range_iterator& operator=(range_iterator other) {
        assert(step_ == other.step_);
        std::swap(value_, other.value_);
        return *this;
    }

    range_iterator& operator++() {
        value_ += step_;
        return *this;
    }

    range_iterator operator++(int) {
        auto tmp = *this;
        value_ += step_;
        return tmp;
    }

    range_iterator& operator+=(const difference_type& inc) {
        value_ += inc * step_;
        return *this;
    }

    range_iterator& operator--() {
        value_ -= step_;
        return *this;
    }

    range_iterator operator--(int) {
        auto tmp = *this;
        value_ -= step_;
        return tmp;
    }

    range_iterator& operator-=(const difference_type& inc) {
        value_ -= inc * step_;
        return *this;
    }

    value_type operator[](const difference_type n) { return value_ + n * step_; }

    value_type operator*() { return value_; }

    // ====================================================
    // Friend Operators
    // ====================================================

    friend bool operator==(const range_iterator& x, const range_iterator& y) {
        assert(x.step_ == y.step_);
        return x.step_ > 0 ? (y.value_ < x.value_) || (y.value_ == x.value_) : (x.value_ < y.value_);
    }

    friend bool operator!=(const range_iterator& x, const range_iterator& y) { return not(x == y); }

    friend bool operator<(const range_iterator& x, const range_iterator& y) { return x.value_ < y.value_; }

    friend difference_type operator-(const range_iterator& x, const range_iterator& y) {
        assert(x.step_ == y.step_);
        return (x.value_ - y.value_) / x.step_;
    }

    friend range_iterator operator+(range_iterator x, difference_type y) { return x += y * x.step_; }

    friend range_iterator operator+(difference_type x, range_iterator y) { return y += x * y.step_; }

   private:
    Incrementable value_;
    const Incrementable step_;
};

/**
 * @brief
 * Proxy returned by range function
 *
 * This is the class returned by the range() function
 * within an range based for loop.
 */
template <typename T>
struct range_proxy {
    range_proxy() = delete;

    range_proxy(T first, T last, T step = 1) : first_(first), last_(last), step_(step) {}

    ~range_proxy() = default;

    auto begin() const { return range_iterator<T>(first_, step_); }

    auto end() const { return range_iterator<T>(last_, step_); }

    auto cbegin() const { return range_iterator<T>(first_, step_); }

    auto cend() const { return range_iterator<T>(last_, step_); }

   private:
    const T first_;
    const T last_;
    const T step_;
};  // struct range_proxy

/**
 * @brief
 * Range function from 0 to stop value
 *
 * @details
 * Allows the usage of the range based for loop from
 * indices 0 to stop number of values.
 *
 * \code{.cpp}
 * for (auto i : range(10)){
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 * is equivalent to
 *
 * \code{.cpp}
 * for(auto i = 0; i < 10; ++i) {
 *    cout << i << "\n";
 * }
 * \lastcode
 */
template <typename T>
range_proxy<T> range(const T last) {
    return {static_cast<T>(0), last, static_cast<T>(1)};
}

/**
 * @brief
 * Range function to return indices within a range
 *
 * @details
 * Allows the usage of the range based for loop over the
 * indices of the provided range.
 *
 * \code{.cpp}
 * for (auto i : range(2,10)){
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 * is equivalent to
 *
 * \code{.cpp}
 * for(auto i = 2; i < 10; ++i) {
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 */
template <typename T>
range_proxy<T> range(const T first, const T last) {
    return {first, last, static_cast<T>(1)};
}

/**
 * @brief
 * Range function to return indices within a range with steps
 *
 * @details
 * Allows the usage of the range based for loop over the
 * indices of the provided range at a given step size.
 *
 * \code{.cpp}
 * for (auto i : range(0,10,2)){
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 * is equivalent to
 *
 * \code{.cpp}
 * for(auto i = 0; i < 10; i+=2) {
 *    cout << i << "\n";
 * }
 * \lastcode
 *
 */
template <typename T>
range_proxy<T> range(const T first, const T last, const T step) {
    return {first, last, step};
}

} /* namespace xstd */
