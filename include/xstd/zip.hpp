/**
 * \file       zip.hpp
 * \author     Bryan Flynt
 * \date       Jan 15, 2022
 */
#pragma once

#include <tuple>
#include <utility>

// #include "algorithm.hpp"  // xstd::for_each

#include <cstddef>     // std::size_t
#include <algorithm>   // std::min, std::max

/**
 * \file
 * algorithm.hpp
 *
 * \brief
 * Element wise algorithmic operations for Tuples
 *
 * \details
 * The functions within this file provide element by element
 * operators for tuples of values similar to the standard
 * library algorithm header does for standard container types.
 * Each function uses the +,-,*,/,%,min,max operators so user
 * types with properly overloaded operators can be used within
 * the tuples. If two tuples are different length the resulting
 * tuple provided as the answer will only be of the shortest
 * length.
 */
/// @cond SKIP_DETAIL
namespace xstd {
namespace detail {

template<class UnaryPredicate, class...Args>
constexpr UnaryPredicate
for_each_arg(UnaryPredicate f, Args&&...args) {
  std::initializer_list<int>{((void)f(std::forward<Args>(args)), 0)...};
  return f;
}

template <class Tuple, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate
for_each_impl(Tuple&& t, UnaryPredicate&& f, std::index_sequence<I...>){
    return (void)std::initializer_list<int>{((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple>(t))),0)...}, f;
}

template <class Tuple1, class Tuple2, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate
for_each_impl(Tuple1&& t1, Tuple2&& t2, UnaryPredicate&& f, std::index_sequence<I...>) {
    return (void)std::initializer_list<int>{((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))),0)...}, f;
}

template<typename...Ts, typename UnaryPredicate, std::size_t... Is>
auto transform_impl(std::tuple<Ts...> const& inputs, UnaryPredicate pred, std::index_sequence<Is...>){
    return std::tuple<std::result_of_t<UnaryPredicate(Ts)>...>{pred(std::get<Is>(inputs))...};
}

template<typename...Ts1, typename...Ts2, typename BinaryPredicate, std::size_t... Is>
auto transform_impl(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, BinaryPredicate pred, std::index_sequence<Is...>){
    return std::tuple<std::result_of_t<BinaryPredicate(Ts1,Ts2)>...>{pred(std::get<Is>(t1),std::get<Is>(t2))...};
}

} /* namespace detail */
} /* namespace xstd */
/// @endcond


namespace xstd {

//
// Forward Declarations
//
template<typename... Ts, typename Function>
constexpr auto
transform(std::tuple<Ts...> const& inputs, Function function);

template<typename... Ts1, typename... Ts2, typename Function>
constexpr auto
transform(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, Function function);



/// Return minimum value within a tuple
/**
 * Returns the minimum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to evaluate
 *
 * \return Minimum value within the tuple
 */
template<typename... Ts>
constexpr auto
min(const std::tuple<Ts...>& t) {
	using std::min;
	using return_type = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return min({static_cast<return_type>(xs)...}); }, std::tuple<Ts...>(t));
}

/// Return minimum value within a tuple
/**
 * Returns the minimum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to move in and evaluate
 *
 * \return Minimum value within the tuple
 */
template<typename... Ts>
constexpr auto
min(std::tuple<Ts...>&& t) {
	using std::min;
	using return_type = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return min({static_cast<return_type>(xs)...}); }, std::forward<std::tuple<Ts...>>(t));
}

/// Return maximum value within a tuple
/**
 * Returns the maximum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to evaluate
 *
 * \return Maximum value within the tuple
 */
template<typename... Ts>
constexpr auto
max(const std::tuple<Ts...>& t) {
	using std::max;
	using return_type  = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return max({static_cast<return_type>(xs)...}); }, std::tuple<Ts...>(t));
}

/// Return maximum value within a tuple
/**
 * Returns the maximum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to move in and evaluate
 *
 * \return Maximum value within the tuple
 */
template<typename... Ts>
constexpr auto
max(std::tuple<Ts...>&& t) {
	using std::max;
	using return_type  = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return max({static_cast<return_type>(xs)...}); }, std::forward<std::tuple<Ts...>>(t));
}

/// Test if unary predicate is true for all
/**
 * Returns true if unary pred returns true for all the elements
 * in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns true for all the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
all_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return (p(std::forward<decltype(xs)>(xs)) && ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is true for all
/**
 * Returns true if binary pred returns true for all the elements
 * in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns true for all the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
all_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
	return xstd::all_of(result, [](auto&& val){return val;});
}

/// Test if unary predicate is true for any
/**
 * Returns true if unary pred returns true for any of the
 * elements in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns true for any of the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
any_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return (p(std::forward<decltype(xs)>(xs)) || ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is true for any
/**
 * Returns true if binary pred returns true for any of the
 * elements in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns true for any of the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
any_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
    return xstd::any_of(result, [](auto&& val){return val;});
}

/// Test if unary predicate is false for all
/**
 * Returns true if unary pred returns false for all of the
 * elements in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns false for all of the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
none_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return !(p(std::forward<decltype(xs)>(xs)) || ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is false for all
/**
 * Returns true if binary pred returns false for all of the
 * elements in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns false for all of the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
none_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
    return xstd::none_of(result, [](auto&& val){return val;});
}


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
template<typename Tuple, typename UnaryPredicate>
constexpr UnaryPredicate
for_each(Tuple&& t, UnaryPredicate p) noexcept {
	std::apply([&p](auto&& ...xs){(..., static_cast<void>(p(std::forward<decltype(xs)>(xs))));}, std::forward<Tuple>(t));
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
constexpr BinaryPredicate
for_each(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& f) {
	constexpr auto t1_size = std::tuple_size_v<std::remove_reference_t<Tuple1>>;
	constexpr auto t2_size = std::tuple_size_v<std::remove_reference_t<Tuple2>>;
	constexpr auto size = std::min(t1_size,t2_size);
    detail::for_each_impl(std::forward<Tuple1>(t1),
    		              std::forward<Tuple2>(t2),
						  std::forward<BinaryPredicate>(f),
                          std::make_index_sequence<size>{});
    return f;
}


/// Count if predicate evaluates to true
/**
 * Apply unary predicate to each index of tuple and count
 * if the predicate evaluates to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Number of predicates that evaluated to true
 */
template<typename Tuple, typename UnaryPredicate>
constexpr std::size_t
count_if(Tuple&& t, UnaryPredicate p) noexcept {
    std::size_t count = 0;
    ::xstd::for_each(t, [&](auto&& value){
            if( p(value) ) {
                ++count;
            }
        });
    return count;
}

/// Count if predicate evaluates to true
/**
 * Apply binary predicate to each index of tuple and count
 * if the predicate evaluates to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Number of predicates that evaluated to true
 */
template<typename T1, typename T2, typename BinaryPredicate>
constexpr std::size_t
count_if(T1&& t1, T2&& t2, BinaryPredicate p) noexcept {
	STATIC_ASSERT(std::tuple_size<std::remove_reference_t<T1>>::value ==
                  std::tuple_size<std::remove_reference_t<T2>>::value,
                  "Tuples must be same length");
    std::size_t count = 0;
    ::xstd::for_each(t1, t2, [&](auto&& v1, auto&& v2){
        if( p(v1,v2) ) {
            ++count;
        }
    });
    return count;
}

/// Return index where unary predicate returns true
/**
 * Apply unary predicate to each index of tuple and
 * return the index of the first entry that evaluates
 * to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Index of first predicate that evaluated to true
 */
template<typename Tuple, typename UnaryPredicate>
constexpr std::size_t
find_if(Tuple&& t, UnaryPredicate p) noexcept {
	std::size_t index = std::tuple_size<std::remove_reference_t<Tuple>>::value;
	std::size_t current_index = 0;
	bool found = false;
	::xstd::for_each(t, [&](auto&& value){
		if (!found && p(value)) {
			index = current_index;
			found = true;
		}
		++current_index;
	});
	return index;
}

/// Return index where binary predicate returns true
/**
 * Apply binary predicate to each index of tuple and
 * return the index of the first entry that evaluates
 * to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Index of first predicate that evaluated to true
 */
template<typename T1, typename T2, typename BinaryPredicate>
constexpr std::size_t
find_if(T1&& t1, T2&& t2, BinaryPredicate p) noexcept {
	STATIC_ASSERT(std::tuple_size<std::remove_reference_t<T1>>::value ==
			      std::tuple_size<std::remove_reference_t<T2>>::value,
				  "Tuples must be same length");
	std::size_t index = std::tuple_size<std::remove_reference_t<T1>>::value;
	std::size_t current_index = 0;
	bool found = false;
	::xstd::for_each(t1, t2, [&](auto&& v1, auto&& v2){
		if (!found && p(v1,v2)) {
			index = current_index;
			found = true;
		}
		++current_index;
	});
	return index;
}

/// Transform tuple using the provided function
/**
 * Transforms the provided tuple using the provided
 * function returning a new tuple of same element type
 * and size
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Transformed tuple
 */
template<typename... Ts, typename Function>
constexpr auto
transform(std::tuple<Ts...> const& inputs, Function function){
    return detail::transform_impl(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
}

/// Transform tuples using the provided function
/**
 * Transforms the provided tuples using the provided
 * function returning a new tuple of same element type
 * and size
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Transformed tuple
 */
template<typename... Ts1, typename... Ts2, typename Function>
constexpr auto
transform(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, Function function){
    return detail::transform_impl(t1,t2, function, std::make_index_sequence<sizeof...(Ts1)>{});
}

/// Perform action on single index of tuple
/**
 * Perform the provided action on the index
 * specified.
 *
 * \param t[in] Tuple 1 to move in and evaluate
 * \param index[in] Index to perform action on
 * \param action[in] Action to perform
 */
template<typename Tuple, typename Action>
constexpr void
perform(Tuple&& t, const std::size_t index, Action action){
	std::size_t current_index = 0;
	for_each(t, [action = std::move(action), index, &current_index](auto&& value){
		if (index == current_index){
			action(std::forward<decltype(value)>(value));
		}
		++current_index;
	});
}


} /* namespace xstd */



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
