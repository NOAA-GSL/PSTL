/**
 * \file       web_example.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */

//
// Example copied from NVidia Website
// https://developer.nvidia.com/blog/accelerating-standard-c-with-gpus-using-stdpar/
//

#include "helpers.hpp"

#include <algorithm>
#include <execution>
#include <iomanip>
#include <iostream>
#include <vector>

//
// MAIN Function
//
int main() {
    using Real = double;
    constexpr std::size_t NSIZE  = 100000;  // Length of Vectors

    // Data for problem
    std::vector<Real> x(NSIZE);
    std::vector<Real> y;

    // Initialize Data
    random_fill(x);
    y = x;

    // Calculate answer on the Device (GPU)
    std::sort(std::execution::par, x.begin(), x.end());

    // Calculate the correct answer on the CPU
    std::sort(y.begin(), y.end());
    
    // Simple compare on CPU
    bool correct = std::equal(x.begin(), x.end(), y.begin());
    std::cout << "Correct = " << std::boolalpha << correct << std::endl;

    return (not correct);
}
