find_package(glog)

add_library(sw STATIC
    stock.cc
    stock_watch.cc
    finnhub.cc
    pattern_detector.cc
)

target_link_libraries(sw
    curl
    glog::glog
    -lpthread
)

target_include_directories(sw PUBLIC
    ${CURL_INCLUDE_DIRS}
    include
)