cmake_minimum_required(VERSION 3.16)
project(LongNumber VERSION 1.0.0)

option(BUILD_TESTS "Сборка тестов" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

add_subdirectory(src)

if(BUILD_TESTS)
	    add_subdirectory(test)
    endif()
