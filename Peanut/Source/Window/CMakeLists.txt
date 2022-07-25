target_sources(
    Peanut PRIVATE
    RenderContext.cpp
    Window.cpp
)

add_subdirectory(RenderContextImpl)
add_subdirectory(WindowImpl)
