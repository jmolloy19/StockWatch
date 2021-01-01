find_package(glog)

add_compile_definitions(STOCKWATCH_MIGRATIONS_DIR="${PROJECT_SOURCE_DIR}/src/stockwatch/postgres/migrations/")
add_compile_definitions(STOCKWATCH_JSONS_DIR="${PROJECT_SOURCE_DIR}/build/jsons/")

add_executable(stockwatch
    main.cc
    stockwatch/stock_watch.cc
    stockwatch/stock.cc
    stockwatch/candle.cc
    stockwatch/config.cc
    stockwatch/finnhub/service.cc
    stockwatch/patterns/high_tight_flag.cc
    stockwatch/postgres/data_base.cc
    stockwatch/util/io/io.cc
    stockwatch/util/json/json.cc
    stockwatch/util/curl/curl.cc
    stockwatch/util/time/time.cc
)
    
target_link_libraries(stockwatch
    curl
    glog::glog
    -lpqxx
    -lpq
    -lstdc++fs 
    -lpthread
)
    
target_include_directories(stockwatch PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CURL_INCLUDE_DIRS}
)
    