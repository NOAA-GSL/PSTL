/**
 * \file       stl_sort.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */

#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

#include "helpers.hpp"


/** Functor to Time
 */
template <typename T>
class SORT {
   public:
    /** Construct the functor
     */
    SORT(const std::vector<T>& x) : x_(x), answer_(x) {
        std::sort(answer_.begin(), answer_.end());
    }

    /** Reset for next timed run
     *
     * This function resets the functor for the running of
     * the next timed run.
     *
     * This should NOT be timed.
     */
    void reset() { temp_ = x_; }

    /** Perform timed calculation
     */
    template <typename Policy>
    void operator()(const Policy policy) {
        std::sort(policy, temp_.begin(), temp_.end());
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
    std::vector<T> x_;
    std::vector<T> temp_;
    std::vector<T> answer_;
};



//
// MAIN Function
//
int main() {
    using Real = double;
    constexpr std::size_t NCYLCE = 10;        // Number of time to repeat test
    constexpr std::size_t NSIZE  = 10000000;  // Length of Vectors

    // Data for problem
    std::vector<Real> x(NSIZE);
    std::vector<bool> correct;

    // Initialize Data
    random_fill(x);

    // Create Functor
    SORT<Real> op(x);
    
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
