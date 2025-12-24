
add_executable(sort_tests
    test_merge_sort.cpp
)

target_link_libraries(sort_tests
    ${SORT_LIB_NAME} 
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(sort_tests)
