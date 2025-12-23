
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(../lib/googletest googletest)

add_executable(sort_app 
    src/main.cpp
    src/merge_sort.cpp
    src/print_array.cpp
)

add_executable(sort_tests 
    tests/test_merge_sort.cpp
    src/merge_sort.cpp
    src/print_array.cpp
)
target_link_libraries(sort_tests gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(sort_tests)
