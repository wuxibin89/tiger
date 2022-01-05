add_library(
    tree
    tree.c
    printtree.c
)
target_link_libraries(tree frame)
target_include_directories(tree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")