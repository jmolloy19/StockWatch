find_package(glog)

add_executable(stockwatch
    main.cc
    stockwatch/stock_watch.cc
    stockwatch/stock.cc
    stockwatch/finnhub.cc
    stockwatch/pattern_detector.cc
    stockwatch/util/curl/curl.cc
    stockwatch/util/io/io.cc
)

target_link_libraries(stockwatch
    curl
    glog::glog
    -lpthread
)

target_include_directories(stockwatch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CURL_INCLUDE_DIRS}
)
