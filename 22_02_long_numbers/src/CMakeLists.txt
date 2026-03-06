add_library(long_number_lib STATIC
    long_number.cpp
)


target_include_directories(long_number_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(long_number_lib PROPERTIES
    OUTPUT_NAME "longnumber"
    CLEAN_DIRECT_OUTPUT 1
)
