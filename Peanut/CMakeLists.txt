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

# Build GLFW alongside with Peanut
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Deps/glfw)

add_executable(Peanut)
add_subdirectory(Deps)
add_subdirectory(Source)

target_include_directories(
    Peanut PRIVATE 
    Deps/glad/include
    Deps/glfw/include
)

target_link_libraries(
    Peanut PRIVATE
    glfw
)
