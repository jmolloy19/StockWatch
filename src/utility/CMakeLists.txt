cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_library(utility STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/parse_cmd_line_options.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/read_write_files.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/create_directory.cpp
)

target_include_directories(utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)