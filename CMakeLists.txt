cmake_minimum_required(VERSION 3.13)

project(SandboxSolution)

# Force c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type was specified, building Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory(Peanut)

add_executable(SandboxSolution)
add_subdirectory(SandboxApp)

if(MSVC)
    target_compile_options(
        SandboxSolution PRIVATE 
        /WX /Zc:preprocessor
    )
else()
    target_compile_options(
        SandboxSolution PRIVATE 
        -Wall -Wextra -Wnon-virtual-dtor -Wsign-promo 
        -Wduplicated-branches -Wduplicated-cond -Wconversion
    )
endif()

target_link_libraries(
    SandboxSolution PRIVATE
    Peanut
)
