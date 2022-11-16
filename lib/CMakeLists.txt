project (partition)

file(GLOB PART_SRC *.cpp)
add_library(${PROJECT_NAME} ${PART_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES
    # LINK_SEARCH_START_STATIC ON
    # LINK_SEARCH_END_STATIC ON
    CXX_STANDARD 14
)
