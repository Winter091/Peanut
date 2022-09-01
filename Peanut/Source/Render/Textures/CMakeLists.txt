target_sources(
    Peanut PRIVATE
    DepthStencilTexture.cpp
    Texture2D.cpp
    TextureLoad.cpp
    TextureSampler.cpp
)

add_subdirectory(Impl)
