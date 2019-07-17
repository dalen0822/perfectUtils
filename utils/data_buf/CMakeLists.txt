cmake_minimum_required(VERSION 2.8)

project(data_buf)
include_directories(inc)
add_subdirectory(src)
add_executable(demo_databuf "./demo/main.c")
target_link_libraries(demo_databuf src)