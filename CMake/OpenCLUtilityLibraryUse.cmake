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

#------------------------------------------------------------------------------
# OpenCLUtilityLibrary_LIBRARIES variable
#------------------------------------------------------------------------------
find_library (OpenCLUtilityLibrary_LIBRARY NAMES OpenCLUtilityLibrary HINTS ${OpenCLUtilityLibrary_LIBRARY_DIRS})
set (OpenCLUtilityLibrary_LIBRARIES ${OpenCLUtilityLibrary_LIBRARY})
find_library (TestOpenCLUtilityLibrary_LIBRARY NAMES TestOpenCLUtilityLibrary HINTS ${OpenCLUtilityLibrary_LIBRARY_DIRS})
set (OpenCLUtilityLibrary_LIBRARIES ${OpenCLUtilityLibrary_LIBRARIES} ${TestOpenCLUtilityLibrary_LIBRARY})