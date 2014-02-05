
include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    TestFixture.hpp
    TestFixture.cpp
)

set(EXECUTABLE_SOURCE_FILES
    catch/catch.hpp
    CatchMain.cpp
    tests.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES
    ${OpenCLUtilityLibrary}
)


add_library (TestOpenCLUtilityLibrary ${SOURCE_FILES})
target_link_libraries(TestOpenCLUtilityLibrary OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

add_executable (Catch ${EXECUTABLE_SOURCE_FILES} ${SOURCE_FILES})
target_link_libraries(Catch OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

