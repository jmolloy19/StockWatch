cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(StockWatch)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/cmake)

add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(src)

option(Test "Build Debugging Tests" OFF)
if(Test)
    add_subdirectory(test)
endif()
