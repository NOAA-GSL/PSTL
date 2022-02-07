#---------------------------------------------------------------------
# Build System for xstd
#---------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)
project(pstl VERSION 1.0.0 LANGUAGES CXX)

#---------------------------------------------------------------------
# User Configure Build Process
#---------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_COLOR_MAKEFILE TRUE)
set(CMAKE_BUILD_TYPE "Release")   # Hard Code

#---------------------------------------------------------------------
# Set location of *.cmake modules
#---------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

#---------------------------------------------------------------------
# Force build in seperate directory
#---------------------------------------------------------------------
include(PreventInSourceBuilds)

#---------------------------------------------------------------------
# Locations of Installation (CMAKE_INSTALL_PREFIX)
#---------------------------------------------------------------------
#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

#---------------------------------------------------------------------
# Directory Report
#---------------------------------------------------------------------
message(VERBOSE "")
message(VERBOSE "--------------------- Directory Report -----------------------")
message(VERBOSE " ") # CMAKE directories are from top level CMakeLists.txt
message(VERBOSE "Top Level Directories:")
message(VERBOSE "CMAKE_SOURCE_DIR     = ${CMAKE_SOURCE_DIR}")
message(VERBOSE "CMAKE_BINARY_DIR     = ${CMAKE_BINARY_DIR}")
message(VERBOSE " ") # PROJECT directories are for recent project call
message(VERBOSE "Project Level Directories:")
message(VERBOSE "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message(VERBOSE "PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR}")
message(VERBOSE " ") # My installation directories
message(VERBOSE "Installation Directories:")
message(VERBOSE "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


#---------------------------------------------------------------------
# Locations of Installation & Report
#---------------------------------------------------------------------

# Source locations in source tree
set(MY_PROJECT_PREFIX ${PROJECT_SOURCE_DIR})
set(MY_PROJECT_INCDIR ${MY_PROJECT_PREFIX}/include)
set(MY_PROJECT_TSTDIR ${MY_PROJECT_PREFIX}/test)

# Where to place all libraries
set(MY_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(MY_INSTALL_INCDIR ${MY_INSTALL_PREFIX}/include)
set(MY_INSTALL_TSTDIR ${MY_INSTALL_PREFIX}/test)

message(STATUS " ")
message(STATUS "Using Directories:")
message(STATUS "--- Original Locations ---")
message(STATUS "MY_PROJECT_PREFIX = ${MY_PROJECT_PREFIX}")
message(STATUS "MY_PROJECT_INCDIR = ${MY_PROJECT_INCDIR}")
message(STATUS "MY_PROJECT_TSTDIR = ${MY_PROJECT_TSTDIR}")
message(STATUS " ")
message(STATUS "--- Installation Locations ---")
message(STATUS "MY_INSTALL_PREFIX = ${MY_INSTALL_PREFIX}")
message(STATUS "MY_INSTALL_INCDIR = ${MY_INSTALL_INCDIR}")
message(STATUS "MY_INSTALL_TSTDIR = ${MY_INSTALL_TSTDIR}")


#---------------------------------------------------------------------
# Detect Library includes, flags, etc.
#---------------------------------------------------------------------
message(VERBOSE "")
message(VERBOSE "================================================================")
message(VERBOSE "                    Searching for Libraries                     ")
message(VERBOSE "================================================================")
if(${CMAKE_VERSION} VERSION_GREATER "3.11.999") 
	cmake_policy(SET CMP0074 NEW) # find_package search <name>_ROOT
endif()


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	find_package(TBB)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "NVHPC")
	set(NVHPC_FOUND TRUE)
	include(CheckGpuFlags)
	message(VERBOSE "NVHPC_GPU_WORKS       = ${NVHPC_GPU_WORKS}")
	message(VERBOSE "NVHPC_MULTICORE_WORKS = ${NVHPC_MULTICORE_WORKS}")
	if(NVHPC_GPU_WORKS)
		set(USE_NVHPC_GPU TRUE)
	elseif(NVHPC_MULTICORE_WORKS)
		set(USE_NVHPC_MULTICORE TRUE)
	endif()
else ()
	message(WARNING "Unknown Compiler ID ${CMAKE_CXX_COMPILER_ID}")
endif()

#
#---------------------------------------------------------------------
# Build Type Flag Configuration
#---------------------------------------------------------------------
	
#---------------------------------------------------------------------
# 
#---------------------------------------------------------------------

# Build Libraries & Applications
add_subdirectory(src)

# Build Tests for Libraries
enable_testing()
add_subdirectory(test)
