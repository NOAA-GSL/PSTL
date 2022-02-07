#
# Tests Directories
#

#
# Function to add a test
#
function(add_pstl_test test_name)
	set(test_target test_${test_name})  # Target becomes test_xxxx
	set(test_source ${test_name}.cpp)   # Source becomes xxxx.cpp
	add_executable(${test_target} ${test_source})
	target_include_directories(${test_target} 
		PRIVATE 
			$<BUILD_INTERFACE:${MY_PROJECT_INCDIR}> # Pick up unisntalled library
    		$<BUILD_INTERFACE:${MY_INSTALL_INCDIR}> # Pick up processed configure.hpp
    )
   	target_link_libraries(${test_target}
		PRIVATE
            XSTD
			"$<$<BOOL:${TBB_FOUND}>:tbb>"
	)
	set_target_properties(${test_target}
		PROPERTIES
    		CXX_STANDARD 17
    		CXX_STANDARD_REQUIRED YES
    		CXX_EXTENSIONS NO
	)
	target_compile_options(${test_target}
	 	PRIVATE
			"$<$<AND:$<BOOL:${NVHPC_FOUND}>,$<BOOL:${NVHPC_GPU_WORKS}>>:-stdpar=gpu>"
			"$<$<AND:$<BOOL:${NVHPC_FOUND}>,$<BOOL:${NVHPC_MULTICORE_WORKS}>>:-stdpar=multicore>"
	)
	add_test(${test_name} ${test_target})
endfunction()

# Install runall.sh script
install(
	FILES "runall.sh" 
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	DESTINATION "${MY_INSTALL_TSTDIR}"
)

#
# List files to compile/test
#
add_pstl_test(strided_range)
add_pstl_test(strided_stride)
add_pstl_test(stl_sort)
add_pstl_test(stl_vector)
add_pstl_test(web_example)