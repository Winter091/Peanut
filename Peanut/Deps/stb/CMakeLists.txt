cmake_minimum_required(VERSION 3.10)

project(stb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(stb src/stb_image.cpp)

target_include_directories(stb PRIVATE include)
