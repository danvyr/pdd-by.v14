project(pddby-project)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11 -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations)

find_package(Boost 1.49 COMPONENTS filesystem system unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)

add_subdirectory(lib)
add_subdirectory(test)
