
include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    catch/catch.hpp
    tests.cpp
    TestFixture.hpp
    TestFixture.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES
    ${OpenCLUtilityLibrary}
)


add_executable (Catch ${SOURCE_FILES})
target_link_libraries(Catch OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

