cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

# Find and include CURL library
find_package(CURL REQUIRED)

# Add the executable
add_executable(untitled main.cpp)

# Link CURL library to your executable
target_link_libraries(untitled PRIVATE CURL::libcurl)