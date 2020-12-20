find_package(glog)
find_package(CURL)

add_library(sw_finnhub STATIC
    finnhub.cc
    curl.cc
)

target_link_libraries(sw_finnhub
    sw_protos
    glog::glog
    curl
)

target_include_directories(sw_finnhub PUBLIC
    include
    ${CURL_INCLUDE_DIR}
)