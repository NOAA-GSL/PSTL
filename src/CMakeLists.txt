#
################################################################
#             Configure and Install Library
################################################################
#
# We do this from inside the "src" directory so that the include
# installation is not corrupted with all the temporary cmake file
# which will now be placed inside "src"
#  
 
#
# Define the name of the xstd Library
#
set(xstd_library_name XSTD)

#
# Make a header only library target
#
add_library(${xstd_library_name} INTERFACE)

#
# List the include directories for tests and library users
#
target_include_directories(${xstd_library_name} 
	INTERFACE
	    "$<BUILD_INTERFACE:${MY_PROJECT_INCDIR}>"
    	"$<INSTALL_INTERFACE:${MY_INSTALL_INCDIR}>"
)

#
# List of all libraries that need linking
#
#target_link_libraries(${xstd_library_name} 
# 	INTERFACE
#)

set_target_properties(${xstd_library_name}
	PROPERTIES
        CXX_STANDARD 17
    	CXX_STANDARD_REQUIRED YES
    	CXX_EXTENSIONS NO
)

#
# Copy all header files to the installation folder
#
install(FILES "${MY_PROJECT_INCDIR}/xstd.hpp" DESTINATION ${MY_INSTALL_INCDIR})
install(DIRECTORY "${MY_PROJECT_INCDIR}/xstd" DESTINATION ${MY_INSTALL_INCDIR})

#
################################################################
#              Making XSTD Library Exportable
################################################################
#
# After all this is done all you need to use the XSTD Library
# is use the CMake "find_package" function and all 
# of the standard variables will be set by that function.
# - XSTD_FOUND
# - XSTD_INCLUDE_DIRS
# - XSTD_LIBRARIES
#
# For Example:
#
# project("My-XSTD-Example")
#
# find_package(XSTD CONFIG REQUIRED)
#
# add_executable(${PROJECT_NAME} src/main.cc)
# target_link_libraries(${PROJECT_NAME} XSTD::XSTD)
#
    

# install(
# 	TARGETS ${xstd_library_name}
# 	EXPORT ${xstd_library_name}_Targets
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

#[[

#
# Provided default system installation locations
# Note: Most won't be used since it is header only
#
install(
	TARGETS ${xstd_library_name}
	EXPORT ${xstd_library_name}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#
# Tell CMake to manage versions of this library
#
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${PROJECT_BINARY_DIR}/${xstd_library_name}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

#
# Tell CMake to generate a configuration files
#
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${xstd_library_name}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${xstd_library_name}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${xstd_library_name}/cmake)
  
#
# Copy the file containing the installation targets 
# to the installation folder
#  
install(EXPORT ${xstd_library_name}_Targets
        FILE ${xstd_library_name}Targets.cmake
        NAMESPACE ${xstd_library_name}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${xstd_library_name}/cmake)

#
# Copy the file containing the build configuration and informartion 
# about version compatibilty to the installation folder
#    
install(FILES "${PROJECT_BINARY_DIR}/${xstd_library_name}Config.cmake"
              "${PROJECT_BINARY_DIR}/${xstd_library_name}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${xstd_library_name}/cmake)
]]



