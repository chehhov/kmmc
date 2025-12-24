add_library(${SORT_LIB_NAME} STATIC
    merge_sort.cpp
    print_array.cpp
)

target_include_directories(${SORT_LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(sort_app main.cpp)
target_link_libraries(sort_app ${SORT_LIB_NAME})
