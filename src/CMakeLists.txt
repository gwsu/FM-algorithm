set(SRC main.cpp)
set(BW_PART bw_partitioner)
add_executable(${BW_PART} ${SRC})

target_link_libraries(${BW_PART}
    partition
)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

target_include_directories(${BW_PART}
    PUBLIC ${PARENT_DIR}
)

set_target_properties(${BW_PART} PROPERTIES
    # LINK_SEARCH_START_STATIC ON
    # LINK_SEARCH_END_STATIC ON
    CXX_STANDARD 14
)
