cmake_minimum_required(VERSION 3.10)
project(ok_api)

add_subdirectory(src/)
target_include_directories(ok_api_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_executable(ok_api main.cpp)
target_link_libraries(ok_api PRIVATE ok_api_lib)