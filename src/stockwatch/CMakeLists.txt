find_package(CURL)
find_package(glog)

add_library(stockwatch STATIC
    finnhub.cc
    stock.cc
    stockwatch.cc
)

target_link_libraries(stockwatch
    sw_protos
    glog::glog
    curl
    -lpthread
)

target_include_directories(stockwatch PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${CURL_INCLUDE_DIR}
)