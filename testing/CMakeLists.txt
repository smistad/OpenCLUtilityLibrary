
set(OpenCLUtilityLibrary_INCLUDE_DIRS
    ${OpenCLUtilityLibrary_INCLUDE_DIRS}
    ./catch
)

include_directories(
    ${OpenCLUtilityLibrary_INCLUDE_DIRS}
    )


set(OUL_TEST_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/testing/TestFixture.hpp
    ${PROJECT_SOURCE_DIR}/testing/TestFixture.cpp
    ${PROJECT_SOURCE_DIR}/testing/histogramPyramidTests.cpp
    ${PROJECT_SOURCE_DIR}/testing/tests.cpp
)

set(EXECUTABLE_SOURCE_FILES
    catch/catch.hpp
    CatchMain.cpp
)

set(OpenCLUtilityLibrary_LINK_LIBRARIES
    ${OpenCLUtilityLibrary}
)

# What was this used for?
#add_library (TestOpenCLUtilityLibrary ${SOURCE_FILES})
#target_link_libraries(TestOpenCLUtilityLibrary OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

#set(TestOpenCLUtilityLibrary_LIBRARY
#    TestOpenCLUtilityLibrary
#    PARENT_SCOPE
#   )
set(OUL_TEST_SOURCE_FILES
    ${OUL_TEST_SOURCE_FILES}
    PARENT_SCOPE
)

add_executable (testOUL ${OUL_TEST_SOURCE_FILES} ${EXECUTABLE_SOURCE_FILES})
target_link_libraries(testOUL OpenCLUtilityLibrary ${OpenCLUtilityLibrary_LINK_LIBRARIES})

