find_package(glog)

add_library(sw STATIC
    stock.cc
    stock_watch.cc
)

target_link_libraries(sw
    sw_protos
    sw_finnhub
    sw_patterns
    -lpthread
)

target_include_directories(sw PUBLIC
    include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(stockwatch
    main.cc
)

target_link_libraries(stockwatch
    sw
)