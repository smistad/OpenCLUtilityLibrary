#------------------------------------------------------------------------------
# External libraries
#------------------------------------------------------------------------------

# OpenCL
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OpenCLUtilityLibrary_SOURCE_DIR}/CMake)
find_package(OpenCL REQUIRED)

#------------------------------------------------------------------------------
# Where to look for includes and libraries
#------------------------------------------------------------------------------
include_directories( ${OpenCLUtilityLibrary_INCLUDE_DIRS} ${OPENCL_INCLUDE_DIR})
link_directories (${OpenCLUtilityLibrary_LIBRARY_DIRS})
