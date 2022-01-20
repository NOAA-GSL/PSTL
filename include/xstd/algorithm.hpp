/**
 * \file       algorithm.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */
#pragma once

#include <algorithm>  // std::min, std::max
#include <cstddef>    // std::size_t
#include <tuple>      // std::tuple, etc.
#include <utility>    // std::index_sequence

namespace xstd {
namespace detail {

template <class UnaryPredicate, class... Args>
constexpr UnaryPredicate for_each_arg(UnaryPredicate f, Args&&... args) {
    std::initializer_list<int>{((void)f(std::forward<Args>(args)), 0)...};
    return f;
}

template <class Tuple, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate for_each_impl(Tuple&& t, UnaryPredicate&& f, std::index_sequence<I...>) {
    return (void)std::initializer_list<int>{
               ((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple>(t))), 0)...},
           f;
}

template <class Tuple1, class Tuple2, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate for_each_impl(Tuple1&& t1, Tuple2&& t2, UnaryPredicate&& f, std::index_sequence<I...>) {
    return (void)std::initializer_list<int>{
               ((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple1>(t1)),
                                                      std::get<I>(std::forward<Tuple2>(t2))),
                0)...},
           f;
}

} /* namespace detail */

/// Apply predicate to each index of tuple
/**
 * Apply predicate to each index of tuple returning the
 * predicate in case it holds a state.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Unary predicate that was used
 */
template <typename Tuple, typename UnaryPredicate>
constexpr UnaryPredicate for_each(Tuple&& t, UnaryPredicate p) noexcept {
    std::apply([&p](auto&&... xs) { (..., static_cast<void>(p(std::forward<decltype(xs)>(xs)))); },
               std::forward<Tuple>(t));
    return p;
}

/// Apply predicate to each index of tuples
/**
 * Apply predicate to each index of the provided tuples
 * returning the predicate in case it holds a state.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Binary predicate that was used
 */
template <class Tuple1, class Tuple2, class BinaryPredicate>
constexpr BinaryPredicate for_each(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& f) {
    constexpr auto t1_size = std::tuple_size_v<std::remove_reference_t<Tuple1>>;
    constexpr auto t2_size = std::tuple_size_v<std::remove_reference_t<Tuple2>>;
    constexpr auto size    = std::min(t1_size, t2_size);
    detail::for_each_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(f),
                          std::make_index_sequence<size>{});
    return f;
}

} /* namespace xstd */
