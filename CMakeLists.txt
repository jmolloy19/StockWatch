cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(StockWatch)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/cmake)

add_subdirectory(src/proto)
add_subdirectory(src/stockwatch)

option(Test "Build Debugging Tests" OFF)
if(Test)
    add_subdirectory(test)
endif()

add_executable(StockWatch
    src/main.cpp
)

target_link_libraries(StockWatch
    stockwatch
)

target_include_directories(StockWatch PUBLIC
    src
)
