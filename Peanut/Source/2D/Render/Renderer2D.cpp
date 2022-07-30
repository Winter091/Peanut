#include <Peanut/2D/Render/Renderer2D.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <Core/StoragePath.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

namespace pn {

static constexpr size_t MAX_RECTANGLES_PER_BATCH = 25000;
static constexpr size_t MAX_VERTICES_PER_BATCH = 4 * MAX_RECTANGLES_PER_BATCH;
static constexpr size_t MAX_INDICES_PER_BATCH = 6 * MAX_RECTANGLES_PER_BATCH;

static constexpr size_t MAX_TEXTURE_SLOTS = 16;

struct PerVertexData
{
    uint8_t VertexIndex;
};

struct PerInstanceData
{
    glm::mat4 ModelMatrix;
    glm::vec4 Color;
    int32_t TexIndex;
};

struct CameraShaderData
{
    glm::mat4 ViewProjMatrix;
};

struct Renderer2DData
{
    PerVertexData* RectanglePerVertexData;
    PerInstanceData* RectanglePerInstanceData;
    std::shared_ptr<VertexBuffer> RectanglePerVertexVBO;
    std::shared_ptr<VertexBuffer> RectanglePerInstanceVBO;
    std::shared_ptr<VertexArray> RectangleVAO;
    std::shared_ptr<Shader> RectangleShader;

    uint32_t NumRectInstances = 0;

    std::array<std::shared_ptr<Texture2D>, MAX_TEXTURE_SLOTS> Textures;
    uint32_t NumTextures = 0;

    std::shared_ptr<ConstantBuffer> CameraConstantBuffer;
};

static std::unique_ptr<Renderer2DData> s_data = nullptr;
static bool s_isInitialized = false;

static void Flush()
{
    PN_PROFILE_FUNCTION();

    s_data->RectanglePerVertexVBO->Unmap();
    s_data->RectanglePerInstanceVBO->Unmap();

    for (uint32_t i = 0; i < s_data->NumTextures; i++) {
        s_data->Textures[i]->BindToSlot(i);
    }

    RenderCommand::DrawIndexedInstanced(s_data->RectangleVAO, 6, s_data->NumRectInstances);
}

static void StartBatch()
{
    PN_PROFILE_FUNCTION();

    s_data->RectanglePerVertexData = reinterpret_cast<PerVertexData*>(s_data->RectanglePerVertexVBO->Map());
    s_data->RectanglePerInstanceData = reinterpret_cast<PerInstanceData*>(s_data->RectanglePerInstanceVBO->Map());
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
        if (s_data->NumTextures + 1 >= MAX_TEXTURE_SLOTS) {
            Flush();
            StartBatch();
        }
        index = s_data->NumTextures;
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

    s_data->RectangleVAO = pn::VertexArray::Create();

    s_data->RectanglePerVertexVBO = pn::VertexBuffer::Create(BufferMapAccess::WriteOnly,
                                                             sizeof(PerVertexData) * MAX_VERTICES_PER_BATCH);
    s_data->RectanglePerVertexVBO->SetLayout(pn::BufferLayout::Create({
        { 0, pn::BufferLayoutElementType::Uint8, 1, "vertexIndex" },
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerVertexVBO, pn::BufferAttributeUsage::PerVertex);

    s_data->RectanglePerInstanceVBO = pn::VertexBuffer::Create(BufferMapAccess::WriteOnly,
                                                             sizeof(PerInstanceData) * MAX_VERTICES_PER_BATCH);
    s_data->RectanglePerInstanceVBO->SetLayout(pn::BufferLayout::Create({
        { 1, pn::BufferLayoutElementType::Mat4,  1, "modelMatrix" },
        { 5, pn::BufferLayoutElementType::Float, 4, "color" },
        { 6, pn::BufferLayoutElementType::Int32, 1, "texIndex" },
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerInstanceVBO, pn::BufferAttributeUsage::PerInstance);

    std::vector<uint32_t> rectIndices = { 0, 1, 2, 2, 3, 0 };
    auto rectangleIBO = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint32, BufferMapAccess::NoAccess,
                                                rectIndices.size() * sizeof(rectIndices[0]), &rectIndices[0]);
    s_data->RectangleVAO->SetIndexBuffer(rectangleIBO);

    s_data->RectangleShader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath(StoragePath::GetAssetsPath() + "/Shaders/Renderer2D/Rect.vert")
        .SetFragmentPath(StoragePath::GetAssetsPath() + "/Shaders/Renderer2D/Rect.frag"),
        "Renderer2D Rectangle Shader");

    s_data->CameraConstantBuffer = pn::ConstantBuffer::Create(pn::BufferMapAccess::WriteOnly, sizeof(CameraShaderData));

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
    PN_PROFILE_FUNCTION();

    s_data->RectangleShader->Bind();
    
    s_data->CameraConstantBuffer->BindToBindingIndex(0);
    auto* data = reinterpret_cast<CameraShaderData*>(s_data->CameraConstantBuffer->Map()); 
    {
        data->ViewProjMatrix = camera.GetViewProjectionMatrix();
    }
    s_data->CameraConstantBuffer->Unmap();

    StartBatch();
}

void Renderer2D::EndScene()
{
    PN_PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::DrawRectangle(const Rectangle& rectangle)
{
    if (s_data->NumRectInstances + 1 > MAX_RECTANGLES_PER_BATCH) {
        Flush();
        StartBatch();
    }

    int32_t textureIndex = -1;
    if (rectangle.HasTexture()) {
        textureIndex = AddTextureToList(rectangle.GetTexture());
    }

    for (size_t i = 0; i < 4; i++) {
        s_data->RectanglePerVertexData[s_data->NumRectInstances * 4 + i].VertexIndex = static_cast<uint8_t>(i);
    }

    s_data->RectanglePerInstanceData[s_data->NumRectInstances].ModelMatrix = rectangle.GetTransformMatrix();
    s_data->RectanglePerInstanceData[s_data->NumRectInstances].Color = rectangle.GetColor();
    s_data->RectanglePerInstanceData[s_data->NumRectInstances].TexIndex = textureIndex;

    s_data->NumRectInstances++;
}

}
