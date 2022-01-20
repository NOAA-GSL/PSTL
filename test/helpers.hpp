/**
 * \file       helpers.hpp
 * \author     Bryan Flynt
 * \date       Jan 19, 2022
 */
#pragma once


#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <type_traits>

#include "xstd/stop_watch.hpp"


/** Function to fill already sized std::vector
 *
 * Function to fill already sized std::vector
 * with random values between 0 and 1.  This function
 * is not intended to be timed and no effort has been
 * made to parallelize it.
 */
template<typename T>
void random_fill(std::vector<T> &vec) {
    static_assert(std::is_floating_point_v<T>);
    std::random_device rd;
    std::mt19937_64 mre(rd());
    std::uniform_real_distribution<T> urd(0.0, 1.0);
    std::for_each(std::begin(vec), std::end(vec), [&](auto& val){
        val = urd(mre);
    });
    // for (auto &val : vec) {
    //     val = urd(mre);
    // }
}

/** Check if two std::vectors are the same
 *
 * This function is not intended to be timed and
 * no effort has been made to parallelize it.
 */
template <typename T>
bool check_same(const std::vector<T> &x, const std::vector<T> &y) {
    return std::equal(x.begin(), x.end(), y.begin());
}


/** Class to perform timed running of provided Functor
 * 
 * 
 */
struct Runner {

    template <std::size_t N, typename P, typename T>
    static bool execute(const P policy, T& functor) {
        xstd::StopWatch watch;

        std::vector<bool>   correct;
        std::vector<double> times;
        for (auto i = 0; i < N; i++) {

            functor.reset();

            // Time Algorithm
            watch.restart();
            functor(policy);
            watch.stop();

            // Check Results
            correct.push_back(functor.check());

            // Capture time for algorithm
            times.push_back(watch.elapsed_seconds());

            // Display some info
            std::cout << "  Time (sec) = " << std::scientific << times.back();
            std::cout << "  Correct = " << std::boolalpha << correct.back() << std::endl;
        }
        double average = std::accumulate(times.begin(), times.end(), double(0)) / times.size();
        std::cout << "  Average (sec) = " << std::scientific << average << std::endl;

        return std::all_of(correct.begin(), correct.end(), [](auto val){return val;});
    }

};
