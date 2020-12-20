find_package(glog)

add_library(sw_patterns STATIC
    pattern_detector.cc
    high_tight_flag.cc
)

target_link_libraries(sw_patterns
    sw_protos
    glog::glog
)

target_include_directories(sw_patterns PUBLIC
    include
)