include(protoGenMacro)
set(Protobuf_USE_STATIC_LIBS OFF)
find_package(Protobuf REQUIRED)

set(SW_PROTOS
    ticker_list.proto
    candle.proto)

generate_protos(SW_PROTO_SRCS SW_PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${SW_PROTOS})

set(SW_PROTO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_library(sw_protos STATIC
    ${SW_PROTO_SRCS})

target_include_directories(sw_protos PUBLIC
    ${SW_PROTO_INCLUDE_DIR})

target_link_libraries(sw_protos
        crypto
        ssl        
        z
        cares
        ${PROTOBUF_LIBRARIES}
        address_sorting)
    
include_directories(${SW_PROTO_INCLUDE_DIR})