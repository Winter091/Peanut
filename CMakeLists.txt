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

# Useful Warnings
if(MSVC)
    target_compile_options(
        SandboxSolution PRIVATE 
        /W4 /WX
    )
else()
    target_compile_options(
        SandboxSolution PRIVATE 
        -Wall -Wextra -Wnon-virtual-dtor -Wsign-promo 
        -Wduplicated-branches -Wduplicated-cond -Wconversion
    )
endif()

get_target_property(PEANUT_INCLUDES Peanut INCLUDE_DIRECTORIES)

target_include_directories(
    SandboxSolution PRIVATE 
    ${PEANUT_INCLUDES}
)

target_link_libraries(
    SandboxSolution PRIVATE
    Peanut
)
