#include <Peanut/2D/Render/Renderer2D.hpp>

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glad/glad.h>

namespace pn {

static constexpr size_t MAX_QUADS_PER_BATCH = 50000;
static constexpr size_t MAX_VERTICES_PER_BATCH = 4 * MAX_QUADS_PER_BATCH;
static constexpr size_t MAX_INDICES_PER_BATCH = 6 * MAX_QUADS_PER_BATCH;

struct Renderer2DPerVertexData
{
    glm::vec2 Position;
    glm::vec2 TexCoord;
};

struct Renderer2DPerInstanceData
{
    glm::vec4 Color;
    int32_t TexIndex;
};

struct Renderer2DData
{
    std::shared_ptr<VertexBuffer> RectanglePerVertexVBO;
    std::shared_ptr<VertexBuffer> RectanglePerInstanceVBO;
    std::shared_ptr<VertexArray> RectangleVAO;
    std::shared_ptr<Shader> RectangleShader;

    std::vector<Renderer2DPerVertexData> RectanglePerVertexData;
    std::vector<Renderer2DPerInstanceData> RectanglePerInstanceData;

    int currNumIndices = 0;
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
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerVertexVBO, pn::BufferDataUsage::PerVertex);

    s_data->RectanglePerInstanceVBO = pn::VertexBuffer::Create(sizeof(Renderer2DPerInstanceData) * MAX_QUADS_PER_BATCH, nullptr);
    s_data->RectanglePerInstanceVBO->SetLayout(pn::BufferLayout::Create({
        { 2, pn::BufferLayoutElementType::Float, 4, "color" },
        { 3, pn::BufferLayoutElementType::Int32, 1, "texIndex" },
    }));
    s_data->RectangleVAO->AddVertexBuffer(s_data->RectanglePerInstanceVBO, pn::BufferDataUsage::PerInstance);

    std::vector<uint32_t> indices;
    indices.reserve(MAX_INDICES_PER_BATCH);
    for (int i = 0, base = 0; i < MAX_QUADS_PER_BATCH; i++, base += 4) { 
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    auto rectangleIBO = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint32, MAX_INDICES_PER_BATCH * sizeof(indices[0]), &indices[0]);
    s_data->RectangleVAO->SetIndexBuffer(rectangleIBO);

    s_data->RectangleShader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath("Peanut/Assets/Shaders/Renderer2D/Rect.vert")
        .SetFragmentPath("Peanut/Assets/Shaders/Renderer2D/Rect.frag"));

    s_isInitialized = true;
}

void Renderer2D::Shutdown()
{
    PN_CORE_ASSERT(s_isInitialized, "Renderer2D is not initialized");
    s_data = nullptr;
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
    if (s_data->RectanglePerInstanceData.size() + 1 > MAX_QUADS_PER_BATCH) {
        Flush();
        ClearBuffers();
        // PN_CORE_TRACE("Over size");
    }

    const glm::vec2& pos = rectangle.GetPos();
    const glm::vec2& size = rectangle.GetSize();

    s_data->RectanglePerVertexData.push_back({ {pos.x         , pos.y         }, {0.0f, 0.0f} });
    s_data->RectanglePerVertexData.push_back({ {pos.x         , pos.y + size.y}, {0.0f, 1.0f} });
    s_data->RectanglePerVertexData.push_back({ {pos.x + size.x, pos.y + size.y}, {1.0f, 1.0f} });
    s_data->RectanglePerVertexData.push_back({ {pos.x + size.x, pos.y         }, {1.0f, 0.0f} });

    s_data->RectanglePerInstanceData.push_back({ {0.0f, 1.0f, 1.0f, 0.0f}, 1 });

    s_data->currNumIndices += 6;
}

void Renderer2D::Flush()
{
    int vertexHandle = s_data->RectanglePerVertexVBO->GetHandle();
    int instanceHandle = s_data->RectanglePerInstanceVBO->GetHandle();

    void* buf = nullptr;
    int bytes = 0;
    buf = glMapNamedBuffer(vertexHandle, GL_WRITE_ONLY);
    PN_CORE_ASSERT(buf, "nullptr");
    bytes = sizeof(Renderer2DPerVertexData) * s_data->RectanglePerVertexData.size();
    memcpy(buf, &s_data->RectanglePerVertexData[0], bytes);
    glUnmapNamedBuffer(vertexHandle);

    buf = glMapNamedBuffer(instanceHandle, GL_WRITE_ONLY);
    PN_CORE_ASSERT(buf, "nullptr");
    bytes = sizeof(Renderer2DPerInstanceData) * s_data->RectanglePerInstanceData.size();
    memcpy(buf, &s_data->RectanglePerInstanceData[0], bytes);
    glUnmapNamedBuffer(instanceHandle);

    // PN_CORE_TRACE("Num indices: {}", s_data->currNumIndices);
    RenderCommand::DrawIndexed(s_data->RectangleVAO, s_data->currNumIndices);
}

void Renderer2D::ClearBuffers()
{
    s_data->RectanglePerVertexData.clear();
    s_data->RectanglePerInstanceData.clear();
    s_data->RectanglePerVertexData.reserve(MAX_VERTICES_PER_BATCH / 10.0f);
    s_data->RectanglePerInstanceData.reserve(MAX_QUADS_PER_BATCH / 10.0f);
    s_data->currNumIndices = 0;
}

}
