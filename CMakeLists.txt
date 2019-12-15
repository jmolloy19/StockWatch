cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/utility)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/stockwatch)

option(Test "Build Debugging Tests" OFF)
if(Test)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()

add_executable(StockWatch
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(StockWatch
    stockwatch
)

target_include_directories(StockWatch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
