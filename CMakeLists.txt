cmake_minimum_required(VERSION 2.8)

project(OpenCLUtilityLibrary)

option(BUILD_TESTS "Build tests." ON)

## Kernel binary path
set(OPENCL_KERNEL_BINARY_PATH "" CACHE STRING "Directory to put kernel binaries in. Default is source dir.")
# Remove any existing "
string(REPLACE "\"" "" OPENCL_KERNEL_BINARY_PATH ${OPENCL_KERNEL_BINARY_PATH})
message("-- OpenCL Kernel binary path set to: ${OPENCL_KERNEL_BINARY_PATH}")
add_definitions("-DOUL_OPENCL_KERNEL_BINARY_PATH=\"${OPENCL_KERNEL_BINARY_PATH}\"")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake)
find_package( Boost COMPONENTS system filesystem chrono date_time thread REQUIRED )
find_package( OpenCL REQUIRED )
find_package( OpenGL REQUIRED )

set(OpenCLUtilityLibrary_INCLUDE_DIRS
    ${OpenCLUtilityLibrary_SOURCE_DIR}
    ${OpenCLUtilityLibrary_SOURCE_DIR}/CL
    ${Boost_INCLUDE_DIRS}
    ${OPENCL_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${OpenCLUtilityLibrary_BINARY_DIR}
)

include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    CL/cl.hpp
    CL/OpenCL.hpp
    Reporter.hpp
    Reporter.cpp
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
    RuntimeMeasurement.hpp
    RuntimeMeasurement.cpp
    RuntimeMeasurementManager.hpp
    RuntimeMeasurementManager.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES
    ${Boost_LIBRARIES}
    ${OPENCL_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

message("-boost libs ${Boost_LIBRARIES}")

add_library (OpenCLUtilityLibrary ${SOURCE_FILES})
target_link_libraries(OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(testing)
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(OUL_TEST_SOURCE_FILES
            ${OUL_TEST_SOURCE_FILES}
            PARENT_SCOPE
        )
    endif()
endif(BUILD_TESTS)

#------------------------------------------------------------------------------
# Configure file for find_package module 
#------------------------------------------------------------------------------

set(OUL_DIR ${PROJECT_SOURCE_DIR})

set(OpenCLUtilityLibrary_LIBRARY
    OpenCLUtilityLibrary
   )

set(OpenCLUtilityLibrary_LIBRARY_DIRS
    ${OpenCLUtilityLibrary_BINARY_DIR}
    ${OpenCLUtilityLibrary_BINARY_DIR}/testing
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
