


add_subdirectory(../lib/googletest googletest-build)
set(SORT_LIB_NAME sort_lib)

enable_testing()


add_subdirectory(src)


include_directories(src)

add_subdirectory(tests)
