/**
 * \file       zip_iterator.hpp
 * \author     Bryan Flynt
 * \date       Feb 7, 2022
 */

#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

#include "helpers.hpp"
#include "xstd/zip.hpp"

/** Functor to Time
 */
template <typename T>
class SAXPY {
   public:
    /** Construct the functor
     */
    SAXPY(const T a, const std::vector<T>& x, const std::vector<T>& y) : a_(a), x_(x), y_(y), answer_(y) {
        for (std::size_t i = 0; i < x_.size(); ++i) {
            answer_[i] += (a_ * x_[i]);
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
        auto zit = xstd::zip(x_,temp_);
        std::for_each(policy, zit.begin(), zit.end(), [a = this->a_](auto vp) { 
            std::get<1>(vp) += a * std::get<0>(vp); 
        });
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
    using Real = double;
    constexpr std::size_t NCYLCE = 10;         // Number of time to repeat test
    constexpr std::size_t NSIZE  = 100000000;  // Length of Vectors

    // Data for problem
    const Real a(5);
    std::vector<Real> x(NSIZE);
    std::vector<Real> y(NSIZE);
    std::vector<bool> correct;

    // Initialize Data
    random_fill(x);
    random_fill(y);

    // Create Functor
    SAXPY<Real> op(a, x, y);

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
