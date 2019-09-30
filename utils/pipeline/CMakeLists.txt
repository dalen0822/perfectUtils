cmake_minimum_required(VERSION 2.8)

project(pipeline)
include_directories(inc)
add_subdirectory(src)
add_executable(demo_pipeline "./demo/demo.c")
target_link_libraries(demo_pipeline src)
target_link_libraries(demo_pipeline pthread)
