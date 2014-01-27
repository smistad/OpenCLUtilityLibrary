cmake_minimum_required(VERSION 2.8)

project(OpenCLUtilityLibrary)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake)
find_package( OpenCL REQUIRED )

set(OpenCLUtilityLibrary_INCLUDE_DIRS
    .
    /CL
    ${OPENCL_INCLUDE_DIR}
)

include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    Context.hpp
    Context.cpp
    DeviceCriteria.hpp
    DeviceCriteria.cpp
    Exceptions.hpp
    Exceptions.cpp
    OpenCLManager.hpp
    OpenCLManager.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES  
    ${OPENCL_LIBRARIES}
)

add_library (OpenCLUtilityLibrary ${SOURCE_FILES})
target_link_libraries(OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

add_subdirectory(testing)