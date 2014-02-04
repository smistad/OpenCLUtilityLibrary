cmake_minimum_required(VERSION 2.8)

project(OpenCLUtilityLibrary)

option(BUILD_TESTS "Build tests." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake)
find_package( OpenCL REQUIRED )

find_package( OpenGL REQUIRED )

set(OpenCLUtilityLibrary_INCLUDE_DIRS
    .
    /CL
    ${OPENCL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${OpenCLUtilityLibrary_BINARY_DIR}
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    Context.hpp
    Context.cpp
    DeviceCriteria.hpp
    DeviceCriteria.cpp
    Exceptions.hpp
    Exceptions.cpp
    GarbageCollector.hpp
    GarbageCollector.cpp
    HelperFunctions.hpp
    HelperFunctions.cpp
    HistogramPyramids.hpp
    HistogramPyramids.cpp
    OpenCLManager.hpp
    OpenCLManager.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES  
    ${OPENCL_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

add_library (OpenCLUtilityLibrary ${SOURCE_FILES})
target_link_libraries(OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

add_executable (example example.cpp)
target_link_libraries(example OpenCLUtilityLibrary)

if(BUILD_TESTS)
    add_subdirectory(testing)
endif(BUILD_TESTS)

#------------------------------------------------------------------------------
# Configure file for find_package module 
#------------------------------------------------------------------------------

set( OpenCLUtilityLibrary_INCLUDE_DIRS
    ${OpenCLUtilityLibrary_SOURCE_DIR}
    ) 

set(OpenCLUtilityLibrary_LIBRARY_DIRS
    ${OpenCLUtilityLibrary_BINARY_DIR}
    )

configure_file (
    "${PROJECT_SOURCE_DIR}/CMake/OpenCLUtilityLibraryConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/OpenCLUtilityLibraryConfig.cmake"
    )
    

#------------------------------------------------------------------------------
# Configure file for settings, filepaths, parameters ...
#------------------------------------------------------------------------------

set(TEST_DIR ${PROJECT_SOURCE_DIR}/testing)

configure_file(
 "${PROJECT_SOURCE_DIR}/CMake/OulConfig.hpp.in"
 "${PROJECT_BINARY_DIR}/OulConfig.hpp"
)