#include <Peanut/2D/Render/Renderer2D.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <Peanut/Core/StoragePath.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

namespace pn {

    static constexpr size_t MAX_RECTANGLES_PER_BATCH = 50000;
    static constexpr size_t MAX_VERTICES_PER_BATCH = 4 * MAX_RECTANGLES_PER_BATCH;
    static constexpr size_t MAX_INDICES_PER_BATCH = 6 * MAX_RECTANGLES_PER_BATCH;

    static constexpr size_t MAX_TEXTURE_SLOTS = 16;

    struct RectPerInstanceData
    {
        glm::mat4 ModelMatrix;
        uint32_t Color;
        int32_t TexIndex;
    };

    struct CameraShaderData
    {
        glm::mat4 ViewProjMatrix;
    };

    struct Renderer2DData
    {
        RectPerInstanceData* RectanglePerInstanceDataPtr = nullptr;
        std::shared_ptr<VertexBuffer> RectanglePerInstanceBuffer;
        std::shared_ptr<ConstantBuffer> CameraConstantBuffer;
        std::shared_ptr<VertexArray> RectangleVertexArray;
        std::shared_ptr<Shader> RectangleShader;

        uint32_t NumRectInstances = 0;

        std::array<std::shared_ptr<Texture>, MAX_TEXTURE_SLOTS> Textures;
        uint32_t NumTextures = 0;

    };

    static std::unique_ptr<Renderer2DData> s_data = nullptr;
    static bool s_isInitialized = false;

    static void Flush()
    {
        PN_PROFILE_FUNCTION();

        s_data->RectanglePerInstanceBuffer->Unmap();

        if (s_data->NumTextures) {
            RenderCommand::BindTextures(&s_data->Textures[0], s_data->NumTextures);
        }

        RenderCommand::DrawInstanced(s_data->RectangleVertexArray, 6, s_data->NumRectInstances);
    }

    static void StartBatch()
    {
        PN_PROFILE_FUNCTION();

        s_data->RectanglePerInstanceDataPtr = reinterpret_cast<RectPerInstanceData*>(s_data->RectanglePerInstanceBuffer->Map());
        s_data->NumRectInstances = 0;
        s_data->NumTextures = 0;
    }

    static int32_t FindTextureIndex(const Texture2D& texture)
    {
        for (uint32_t i = 0; i < s_data->NumTextures; i++) {
            if (texture == (*s_data->Textures[i])) {
                return i;
            }
        }
        return -1;
    }

    static int32_t AddTextureToList(const std::shared_ptr<Texture2D>& texture)
    {
        int32_t index = FindTextureIndex(*texture);
        if (index == -1) {
            if (s_data->NumTextures >= MAX_TEXTURE_SLOTS) {
                Flush();
                StartBatch();
            }
            index = static_cast<int32_t>(s_data->NumTextures);
            s_data->Textures[index] = texture;
            s_data->NumTextures++;
        }
        return index;
    }

    void Renderer2D::Init()
    {
        PN_PROFILE_FUNCTION();

        PN_CORE_ASSERT(!s_isInitialized, "Renderer2D can be initialized only once");
    
        s_data = std::make_unique<Renderer2DData>();

        uint8_t vertexIndices[6] = { 0, 1, 2, 2, 3, 0 };
        auto rectanglePerVertexBuffer = VertexBuffer::Create(
            BufferLayout::Create({
                { 0, "VertexIndex", BufferLayoutElementType::Uint8, 1 }}),
            BufferMapAccess::NoAccess,
            VertexBufferDataUsage::PerVertex,
            sizeof(vertexIndices),
            vertexIndices);

        s_data->RectanglePerInstanceBuffer = VertexBuffer::Create(
            BufferLayout::Create({
                { 1, "ModelMatrix", BufferLayoutElementType::Mat4, 1 },
                { 5, "Color",       BufferLayoutElementType::Uint32, 1 },
                { 6, "TexIndex",    BufferLayoutElementType::Int32, 1 }}),
            BufferMapAccess::WriteDiscard,
            VertexBufferDataUsage::PerInstance,
            sizeof(RectPerInstanceData) * MAX_VERTICES_PER_BATCH);

        s_data->CameraConstantBuffer = ConstantBuffer::Create(BufferMapAccess::WriteDiscard, sizeof(CameraShaderData));

        s_data->RectangleShader = Shader::Create(ShaderPaths()
            .SetVertexPath(StoragePath::GetAssetsPath() + "/Shaders/Renderer2D/Rect.vert")
            .SetFragmentPath(StoragePath::GetAssetsPath() + "/Shaders/Renderer2D/Rect.frag"),
            "Renderer2D Rectangle Shader");

        s_data->RectangleVertexArray = VertexArray::Create(VertexArrayDescription()
            .SetVertexBuffers({ rectanglePerVertexBuffer, s_data->RectanglePerInstanceBuffer })
            .SetShader(s_data->RectangleShader));

        s_isInitialized = true;
    }

    void Renderer2D::Shutdown()
    {
        PN_PROFILE_FUNCTION();

        PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");
        s_data.reset(nullptr);
        s_isInitialized = false;
    }

    void Renderer2D::BeginScene(const Camera& camera)
    {
        PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");
        PN_PROFILE_FUNCTION();

        s_data->CameraConstantBuffer->SetData(&camera.GetViewProjectionMatrix());

        RenderCommand::BindConstantBuffers(&s_data->CameraConstantBuffer, 1);
        RenderCommand::BindShader(s_data->RectangleShader);

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");
        PN_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::DrawRectangle(const Rectangle& rectangle)
    {
        PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");

        if (s_data->NumRectInstances + 1 > MAX_RECTANGLES_PER_BATCH) {
            Flush();
            StartBatch();
        }

        int32_t textureIndex = -1;
        if (rectangle.HasTexture()) {
            textureIndex = AddTextureToList(rectangle.GetTexture());
        }

        const glm::u8vec4& rectColor = rectangle.GetColor();
        uint32_t color = 0;
        color |= ((rectColor[0]) << 0);
        color |= ((rectColor[1]) << 8);
        color |= ((rectColor[2]) << 16);
        color |= ((rectColor[3]) << 24);

        s_data->RectanglePerInstanceDataPtr[s_data->NumRectInstances].ModelMatrix = rectangle.GetTransformMatrix();
        s_data->RectanglePerInstanceDataPtr[s_data->NumRectInstances].Color = color;
        s_data->RectanglePerInstanceDataPtr[s_data->NumRectInstances].TexIndex = textureIndex;

        s_data->NumRectInstances++;
    }

}
