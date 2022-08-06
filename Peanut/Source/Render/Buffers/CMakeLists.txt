target_sources(
    Peanut PRIVATE
    BufferLayout.cpp
    ConstantBuffer.cpp
    IndexBuffer.cpp
    PipelineState.cpp
    VertexBuffer.cpp
)

add_subdirectory(Impl)
