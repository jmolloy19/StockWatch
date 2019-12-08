cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/utility)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/stockwatch)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)


add_executable(StockWatch.exe
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(StockWatch.exe
    stockwatch
)

target_include_directories(StockWatch.exe PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)