cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")
project(StockWatch)

link_libraries(StockWatch)

add_executable(CreateSymbolsTest
    ${CMAKE_CURRENT_SOURCE_DIR}/TestCreateSymbolList.cpp
)

add_executable(FetchDataTest
    ${CMAKE_CURRENT_SOURCE_DIR}/TestFetchHistoricalData.cpp
)

add_executable(InputDataTest
    ${CMAKE_CURRENT_SOURCE_DIR}/TestInputData.cpp
)

add_executable(WriteTest
    ${CMAKE_CURRENT_SOURCE_DIR}/TestWriteFile.cpp
)

add_executable(ReadTest
    ${CMAKE_CURRENT_SOURCE_DIR}/TestReadFile.cpp
)
