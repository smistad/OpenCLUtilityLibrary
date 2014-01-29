cmake_minimum_required(VERSION 2.8)

project(OpenCLUtilityLibrary)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake)
find_package( OpenCL REQUIRED )

find_package( OpenGL REQUIRED )

set(OpenCLUtilityLibrary_INCLUDE_DIRS
    .
    /CL
    ${OPENCL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
)

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
target_link_libraries(example OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

add_subdirectory(testing)
