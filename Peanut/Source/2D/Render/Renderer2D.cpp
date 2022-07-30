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

static constexpr size_t MAX_RECTANGLES_PER_BATCH = 100000;
static constexpr size_t MAX_VERTICES_PER_BATCH = 4 * MAX_RECTANGLES_PER_BATCH;
static constexpr size_t MAX_INDICES_PER_BATCH = 6 * MAX_RECTANGLES_PER_BATCH;

static constexpr size_t MAX_TEXTURE_SLOTS = 16;

struct PerVertexData
{
    glm::vec2 Position;
    glm::vec2 TexCoord;

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
    std::shared_ptr<VertexBuffer> RectanglePerVertexVBO;
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

    for (uint32_t i = 0; i < s_data->NumTextures; i++) {
        s_data->Textures[i]->BindToSlot(i);
    }

    RenderCommand::DrawIndexed(s_data->RectangleVAO, s_data->NumRectInstances * 6);
}

static void StartBatch()
{
    PN_PROFILE_FUNCTION();

    s_data->RectanglePerVertexData = reinterpret_cast<PerVertexData*>(s_data->RectanglePerVertexVBO->Map());
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
        { 0, pn::BufferLayoutElementType::Float, 2, "position" },
        { 1, pn::BufferLayoutElementType::Float, 2, "texCoord" },
        { 2, pn::BufferLayoutElementType::Float, 4, "color" },
        { 3, pn::BufferLayoutElementType::Int32, 1, "texIndex" },
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerVertexVBO, pn::BufferAttributeUsage::PerVertex);

    std::vector<uint32_t> rectIndices;
    rectIndices.reserve(MAX_INDICES_PER_BATCH);
    std::vector<uint32_t> singleRectIndices = { 0, 1, 2, 2, 3, 0 };
    for (uint32_t i = 0, base = 0; i < MAX_RECTANGLES_PER_BATCH; i++, base += 4) { 
        for (uint32_t quadIndex : singleRectIndices) {
            rectIndices.push_back(base + quadIndex);
        }
    }

    auto rectangleIBO = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint32, BufferMapAccess::NoAccess,
                                                MAX_INDICES_PER_BATCH * sizeof(rectIndices[0]), &rectIndices[0]);
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

    static glm::vec4 positions[] = {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f }
    };

    static glm::vec2 texCoords[] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };

    int32_t textureIndex = -1;
    if (rectangle.HasTexture()) {
        textureIndex = AddTextureToList(rectangle.GetTexture());
    }

    const glm::mat4& rectTransform = rectangle.GetTransformMatrix();

    for (size_t i = 0, base = s_data->NumRectInstances * 4; i < 4; i++) {
        s_data->RectanglePerVertexData[base + i].Position = rectTransform * positions[i];
        s_data->RectanglePerVertexData[base + i].TexCoord = texCoords[i];
        s_data->RectanglePerVertexData[base + i].Color = rectangle.GetColor();
        s_data->RectanglePerVertexData[base + i].TexIndex = textureIndex;
    }

    s_data->NumRectInstances++;
}

}
