add_executable(long_number_tests
    long_number_tests.cpp
)

target_link_libraries(long_number_tests
    PRIVATE
    long_number_lib
)

target_include_directories(long_number_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_custom_target(run_tests
    COMMAND long_number_tests
    DEPENDS long_number_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

include(CTest)
add_test(NAME LongNumberTests COMMAND long_number_tests)
