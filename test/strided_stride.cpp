/**
 * \file       strided_stride.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */

#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

#include "helpers.hpp"
#include "xstd/strided.hpp"

template <typename T>
class STRIDED_SAXPY {
   public:
    /** Construct the functor
     */
    STRIDED_SAXPY(const T a, const std::vector<T>& x, const std::ptrdiff_t incx, const std::vector<T>& y,
                  const std::ptrdiff_t incy)
        : a_(a), x_(x), incx_(incx), y_(y), answer_(y), incy_(incy) {
        const std::ptrdiff_t x_length = x.size() / incx;
        const std::ptrdiff_t y_length = y.size() / incy;
        const std::ptrdiff_t n        = std::min(x_length, y_length);
        for (std::ptrdiff_t i = 0; i < n; ++i) {
            answer_[i * incy_] += (a_ * x_[i * incx_]);
        }
    }

    /** Reset for next timed run
     *
     * This function resets the functor for the running of
     * the next timed run.
     *
     * This should NOT be timed.
     */
    void reset() { temp_ = y_; }

    /** Perform timed calculation
     */
    template <typename Policy>
    void operator()(const Policy policy) {
        auto x_iter = xstd::strided(x_, incx_);
        auto y_iter = xstd::strided(temp_, incy_);
        std::transform(policy, x_iter.begin(), x_iter.end(), y_iter.begin(), y_iter.begin(),
                       [a = this->a_](auto xval, auto yval) { return yval + (a * xval); });
    }

    /** Check for correct solution
     *
     * Checks the calculated results against the true solution
     * previously calculated during construction.
     *
     * This should NOT be timed.
     */
    bool check() { return std::equal(answer_.begin(), answer_.end(), temp_.begin()); }

   private:
    std::ptrdiff_t incx_;
    std::ptrdiff_t incy_;
    T a_;
    std::vector<T> x_;
    std::vector<T> y_;
    std::vector<T> temp_;
    std::vector<T> answer_;
};

//
// MAIN Function
//
int main() {
    using Real                    = double;
    constexpr std::size_t NCYLCE  = 10;        // Number of time to repeat test
    constexpr std::size_t NSIZE   = 10000000;  // Length of Vectors
    constexpr std::ptrdiff_t INCX = 2;
    constexpr std::ptrdiff_t INCY = 3;

    // Data for problem
    const Real a(5);
    std::vector<Real> x(NSIZE * INCX);
    std::vector<Real> y(NSIZE * INCY);
    std::vector<bool> correct;

    // Initialize Data
    random_fill(x);
    random_fill(y);

    // Create Functor
    STRIDED_SAXPY<Real> op(a, x, INCX, y, INCY);

    // Calculate Timings
    std::cout << "std::execution::seq\n";
    correct.push_back( Runner::execute<NCYLCE>(std::execution::seq, op) );

    std::cout << "std::execution::unseq\n";
    correct.push_back( Runner::execute<NCYLCE>(std::execution::unseq, op) );

    std::cout << "std::execution::par\n";
    correct.push_back( Runner::execute<NCYLCE>(std::execution::par, op) );

    std::cout << "std::execution::par_unseq\n";
    correct.push_back( Runner::execute<NCYLCE>(std::execution::par_unseq, op) );

    return not std::all_of(correct.begin(), correct.end(), [](auto val){return val;});
}
