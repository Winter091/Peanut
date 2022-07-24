#include <Peanut/2D/Render/Renderer2D.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

namespace pn {

static constexpr size_t MAX_QUADS_PER_BATCH = 100000;
static constexpr size_t MAX_VERTICES_PER_BATCH = 4 * MAX_QUADS_PER_BATCH;
static constexpr size_t MAX_INDICES_PER_BATCH = 6 * MAX_QUADS_PER_BATCH;

struct Renderer2DPerVertexData
{
    glm::vec2 Position;
    glm::vec2 TexCoord;

    glm::vec4 Color;
    int32_t TexIndex;
};

struct Renderer2DPerInstanceData
{
    glm::vec4 Color;
    int32_t TexIndex;
};

struct Renderer2DData
{
    std::array<Renderer2DPerVertexData, MAX_VERTICES_PER_BATCH> RectanglePerVertexData;
    std::shared_ptr<VertexBuffer> RectanglePerVertexVBO;
    std::shared_ptr<VertexArray> RectangleVAO;
    std::shared_ptr<Shader> RectangleShader;

    uint32_t NumQuadInstances = 0;
};

static std::unique_ptr<Renderer2DData> s_data = nullptr;
static bool s_isInitialized = false;

void Renderer2D::Init()
{
    PN_CORE_ASSERT(!s_isInitialized, "Renderer2D can be initialized only once");
    
    s_data = std::make_unique<Renderer2DData>();

    s_data->RectangleVAO = pn::VertexArray::Create();

    s_data->RectanglePerVertexVBO = pn::VertexBuffer::Create(sizeof(Renderer2DPerVertexData) * MAX_VERTICES_PER_BATCH, nullptr);
    s_data->RectanglePerVertexVBO->SetLayout(pn::BufferLayout::Create({
        { 0, pn::BufferLayoutElementType::Float, 2, "position" },
        { 1, pn::BufferLayoutElementType::Float, 2, "texCoord" },
        { 2, pn::BufferLayoutElementType::Float, 4, "color" },
        { 3, pn::BufferLayoutElementType::Int32, 1, "texIndex" },
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerVertexVBO, pn::BufferDataUsage::PerVertex);

    std::vector<uint32_t> rectIndices;
    rectIndices.reserve(MAX_INDICES_PER_BATCH);
    std::vector<uint32_t> singleRectIndices = { 0, 1, 2, 2, 3, 0 };
    for (uint32_t i = 0, base = 0; i < MAX_QUADS_PER_BATCH; i++, base += 4) { 
        for (uint32_t quadIndex : singleRectIndices) {
            rectIndices.push_back(base + quadIndex);
        }
    }

    auto rectangleIBO = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint32, MAX_INDICES_PER_BATCH * sizeof(rectIndices[0]), &rectIndices[0]);
    s_data->RectangleVAO->SetIndexBuffer(rectangleIBO);

    s_data->RectangleShader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath("Peanut/Assets/Shaders/Renderer2D/Rect.vert")
        .SetFragmentPath("Peanut/Assets/Shaders/Renderer2D/Rect.frag"),
        "Renderer2D Rectangle Shader");

    s_isInitialized = true;
}

void Renderer2D::Shutdown()
{
    PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");
    s_data.reset(nullptr);
    s_isInitialized = false;
}

void Renderer2D::BeginScene(const Camera& camera)
{
    s_data->RectangleShader->Bind();
    s_data->RectangleShader->SetMat4("u_viewProjMatrix", camera.GetViewProjectionMatrix());
    ClearBuffers();
}

void Renderer2D::EndScene()
{
    Flush();
}

void Renderer2D::DrawShape(const Rectangle& rectangle)
{
    if (s_data->NumQuadInstances + 1 > MAX_QUADS_PER_BATCH) {
        Flush();
        ClearBuffers();
    }

    static glm::vec4 positions[] = {
        { -0.5f, -0.5f,  0.0f,  1.0f },
        { -0.5f,  0.5f,  0.0f,  1.0f },
        {  0.5f,  0.5f,  0.0f,  1.0f },
        {  0.5f, -0.5f,  0.0f,  1.0f }
    };

    static glm::vec2 texCoords[] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };

    const glm::mat4& rectTransform = rectangle.GetTransformMatrix();
    const glm::vec4& rectColor = rectangle.GetColor();
    size_t base = s_data->NumQuadInstances * 4;

    for (int i = 0; i < 4; i++) {
        s_data->RectanglePerVertexData[base + i].Position = rectTransform * positions[i];
        s_data->RectanglePerVertexData[base + i].TexCoord = texCoords[i];
        s_data->RectanglePerVertexData[base + i].Color = rectColor;
        s_data->RectanglePerVertexData[base + i].TexIndex = -1;
    }

    s_data->NumQuadInstances++;
}

void Renderer2D::Flush()
{
    int vertexHandle = s_data->RectanglePerVertexVBO->GetHandle();

    void* buf = nullptr;
    size_t bytes = 0;
    buf = glMapNamedBuffer(vertexHandle, GL_WRITE_ONLY);
    PN_CORE_ASSERT(buf, "nullptr");
    bytes = sizeof(Renderer2DPerVertexData) * s_data->RectanglePerVertexData.size();
    memcpy(buf, &s_data->RectanglePerVertexData[0], bytes);
    glUnmapNamedBuffer(vertexHandle);

    RenderCommand::DrawIndexed(s_data->RectangleVAO, s_data->NumQuadInstances * 6);
}

void Renderer2D::ClearBuffers()
{
    s_data->NumQuadInstances = 0;
}

}
