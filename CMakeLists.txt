cmake_minimum_required(VERSION 3.3)
add_subdirectory(seasocks)

SET(SEASOCKS_EXAMPLE_APP OFF CACHE BOOL "" FORCE)
SET(DEFLATE_SUPPORT OFF CACHE BOOL "" FORCE)

add_executable(task main.cpp)
target_link_libraries(task seasocks)
