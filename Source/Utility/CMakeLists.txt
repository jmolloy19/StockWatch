cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_library(Utility STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CmdLineOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WriteReadFile.cpp
)

target_include_directories(Utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)