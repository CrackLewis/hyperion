cmake_minimum_required(VERSION 3.22.0)


project(hyperion VERSION 0.1.0)

find_package(GTest CONFIG REQUIRED)

include_directories(.)
add_subdirectory(test)