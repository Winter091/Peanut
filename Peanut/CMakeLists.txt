cmake_minimum_required(VERSION 3.13)

project(Peanut)

option(PN_USE_DX11_ON_WINDOWS "Use DirectX 11 rendering backend on windows instead of OpenGL" ON)

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

# Build GLFW alongside with Peanut
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Deps/glfw)

# Build spdlog alongside with Peanut
add_subdirectory(Deps/spdlog)

#Build stb alongside with Peanut
add_subdirectory(Deps/stb)

add_library(Peanut)

target_compile_definitions(Peanut PUBLIC SPDLOG_COMPILED_LIB)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Peanut PUBLIC PN_DEBUG)
endif()

target_link_libraries(
    Peanut PRIVATE 
    glfw
    stb
)

target_link_libraries(
    Peanut PUBLIC
    spdlog
)

target_include_directories(
    Peanut PUBLIC 
    Source
    Deps/glad/include
    Deps/glfw/include
    Deps/stb/include
)

target_include_directories(
    Peanut PUBLIC
    Include
    Deps/glm/include
    Deps/spdlog/include
)

target_compile_definitions(Peanut PUBLIC PN_PROFILE)

if (UNIX)
    target_compile_definitions(Peanut PUBLIC PN_PLATFORM_LINUX)
    target_compile_definitions(Peanut PUBLIC PN_WINDOW_GLFW)
    target_compile_definitions(Peanut PUBLIC PN_RENDERING_OPENGL)
    set(PN_RENDERING_OPENGL ON)
elseif (WIN32 OR MINGW OR CYGWIN)
    target_compile_definitions(Peanut PUBLIC PN_PLATFORM_WINDOWS)
    target_compile_definitions(Peanut PUBLIC PN_WINDOW_GLFW)

    if (PN_USE_DX11_ON_WINDOWS)
        target_compile_definitions(Peanut PUBLIC PN_RENDERING_DX11)
        set(PN_RENDERING_DX11 ON)
        target_link_libraries(Peanut PUBLIC d3d11 dxguid DXGI D3DCompiler)
    else()
        target_compile_definitions(Peanut PUBLIC PN_RENDERING_OPENGL)
        set(PN_RENDERING_OPENGL ON)
    endif()
else()
    message(FATAL_ERROR "Platform is not supported by Peanut")
endif()

if(MSVC)
    target_compile_options(
        Peanut PRIVATE
        /WX /Zc:preprocessor
    )
else()
    target_compile_options(
        Peanut PRIVATE
        -Wall -Wextra -Wduplicated-branches -Wduplicated-cond -Wconversion
        $<$<COMPILE_LANGUAGE:CXX>: -Wnon-virtual-dtor -Wsign-promo>
    )
endif()

add_subdirectory(Deps)
add_subdirectory(Source)
