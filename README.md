# Parallel Standard Template Library 
Simple tests for the Parallel Standard Template Library (PSTL)


## Status of Tests
| Symbol | Meaning        |
| ------ |:--------------:|
| N      | Not Working    |
| Y      | Works          |
| -      | To Be Done     |

### Current Status
| Compiler      |     stl_sort     |    stl_vector      |  strided_vector  | strided_stride  |
| ------------- |:----------------:|:------------------:|:----------------:|:---------------:|
| GCC 11.1      |
| GCC 11.1 + TBB 2021.3.0 |    
| NVHPC         |
| NVHPC + A100  |

### Instructions to Compile

1. Download the repository:
```console
git clone https://github.com/NOAA-GSD/PSTL.git
```

2. Create a build directory (for instance inside the source tree): 
```console
cd PSTL
mkdir build
cd build
```

3. Configure your system to have the proper libraries visable to the CMake build system:  
**Note:** These rules and variables used by CMake and not part of the PSTL application
    - CXX 
        - CMake will check CXX to detect the current compiler for C++
    ```console
	export CXX=/my/dir/to/compiler/g++
	```
	- TBB_ROOT 
	    - CMake will check for Intel Thread Building Blocks if using the GCC compiler
	```console 
    export TBB_ROOT=/my/path/to/tbb               # MPI Wrapped C Compiler
    ```


4. Run cmake to generate the Makefile:
```console
cmake ..
```
CMake tries to determine the platform you use, and will look for the required tools. It will report if something is missing.

## Instructions to Run

1. Run test cases.
```console
make test
```

2. Detailed test case information
The test cases print out more detailed information as they run but this information is not displayed using the 
cmake test environment.  To view the output each test can be manualy run from within the testing directory.
```console
cd test
./test_<case_name>
```







