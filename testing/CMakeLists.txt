
include_directories(${OpenCLUtilityLibrary_INCLUDE_DIRS})

set(SOURCE_FILES
    OulTestFixture.hpp
    OulTestFixture.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES
    ${OpenCLUtilityLibrary}
)


add_library (OpenCLUtilityLibraryTesting ${SOURCE_FILES})
target_link_libraries(OpenCLUtilityLibraryTesting OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})