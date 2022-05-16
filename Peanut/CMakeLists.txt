cmake_minimum_required(VERSION 3.13)

project(Peanut)

# Force c11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Force c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type was specified, building Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# ===========================================================================

# ===========================================================================






# Build GLFW alongside with Peanut
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Deps/glfw)

# Build spdlog alongside with Peanut
add_subdirectory(Deps/spdlog)

add_library(Peanut)
add_subdirectory(Deps)
add_subdirectory(Peanut)

target_compile_definitions(Peanut PUBLIC SPDLOG_COMPILED_LIB)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Peanut PUBLIC PN_DEBUG)
endif()

# Useful Warnings
if(MSVC)
    target_compile_options(
        Peanut PRIVATE 
        /W4 /WX
    )
else()
    target_compile_options(
        Peanut PRIVATE
        -Wall -Wextra -Wduplicated-branches -Wduplicated-cond -Wconversion
        $<$<COMPILE_LANGUAGE:CXX>: -Wnon-virtual-dtor -Wsign-promo>
    )
endif()

target_include_directories(
    Peanut PRIVATE 
    .
    Deps/glad/include
    Deps/glfw/include
    Deps/glm/include
    Deps/spdlog/include
)

target_link_libraries(
    Peanut PRIVATE 
    glfw
    spdlog
    d3d11
)
