add_subdirectory(stockwatch)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(stockwatch
    main.cc
)

target_link_libraries(stockwatch
    sw
)