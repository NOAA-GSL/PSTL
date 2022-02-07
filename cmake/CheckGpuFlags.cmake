

include(CheckCXXSourceCompiles)

#
# Capture the Original Flags
#
set(ORIG_FLAGS ${CMAKE_REQUIRED_FLAGS})
set(ORIG_LINK_OPTIONS ${CMAKE_REQUIRED_LINK_OPTIONS})

# 
# Attempt to Compile for GPU
#
set(CMAKE_REQUIRED_FLAGS "-stdpar=gpu")
set(CMAKE_REQUIRED_LINK_OPTIONS "-stdpar=gpu")
check_cxx_source_compiles(
    "
    #include <algorithm>
    #include <execution>
    #include <vector>

    int main() {
        using Real = double;
        constexpr std::size_t NSIZE  = 100000;
        std::vector<Real> x(NSIZE, 0);
        std::sort(std::execution::par, x.begin(), x.end());
        return 0;
    }"
    NVHPC_GPU_WORKS
)

# 
# Attempt to Compile for MultiCore CPU
#
set(CMAKE_REQUIRED_FLAGS "-stdpar=multicore")
set(CMAKE_REQUIRED_LINK_OPTIONS "-stdpar=multicore")
check_cxx_source_compiles(
    "
    #include <algorithm>
    #include <execution>
    #include <vector>

    int main() {
        using Real = double;
        constexpr std::size_t NSIZE  = 100000;
        std::vector<Real> x(NSIZE, 0);
        std::sort(std::execution::par, x.begin(), x.end());
        return 0;
    }"
    NVHPC_MULTICORE_WORKS
)

#
# Set back to original
#
set(CMAKE_REQUIRED_FLAGS ${ORIG_FLAGS})
set(CMAKE_REQUIRED_LINK_OPTIONS ${ORIG_LINK_OPTIONS})