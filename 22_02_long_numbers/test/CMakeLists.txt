include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git  
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(long_number_test long_number_tests.cpp)
target_link_libraries(long_number_test PRIVATE long_number GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(long_number_test)
