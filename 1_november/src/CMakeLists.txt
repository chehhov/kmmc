add_library(sort_lib STATIC
    merge_sort.cpp
    print_array.cpp
)

target_include_directories(sort_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(sort_app main.cpp)
target_link_libraries(sort_app sort_lib)
