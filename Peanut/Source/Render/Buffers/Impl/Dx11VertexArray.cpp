#include "Dx11VertexArray.hpp"
#include "Dx11VertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Window/RenderContextImpl/Dx11RenderContext.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>
#include "Dx11EnumConversions.hpp"
#include "Dx11VertexBuffer.hpp"
#include "Dx11IndexBuffer.hpp"
#include "Dx11ConstantBuffer.hpp"

#include <unordered_set>
#include <algorithm>

namespace pn
{

Dx11VertexArray::Dx11VertexArray(const VertexArrayDescription& description)
    : m_vertexBuffers(description.GetVertexBuffers())
    , m_indexBuffer(description.GetIndexBuffer())
    , m_inputLayout(description.GetShaderInputLayout())
{
    if (!m_inputLayout) {
        m_inputLayout = ShaderInputLayout::Create(m_vertexBuffers, description.GetShader());
    }
    
    for (const auto& vertexBuffer : m_vertexBuffers) {
        if (vertexBuffer->GetDataUsage() == VertexBufferDataUsage::PerInstance) {
            UpdateInstanceCount(*vertexBuffer);
        }
    }
}

void Dx11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

uint32_t Dx11VertexArray::GetVertexCount() const
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to pipeline state, can't get vertex count");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t Dx11VertexArray::GetIndexCount() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetIndexCount();
}

uint32_t Dx11VertexArray::GetInstanceCount() const
{
    // TODO: fixme
    return 0;
}

IndexBufferDataFormat Dx11VertexArray::GetIndexDataFormat() const
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set, unable to get its data format");
    return m_indexBuffer->GetDataFormat();
}

void Dx11VertexArray::UpdateInstanceCount(const VertexBuffer& vertexBuffer)
{
    if (m_instanceCount == 0) {
        m_instanceCount = vertexBuffer.GetVertexCount();
    } else {
        m_instanceCount = std::min<uint32_t>(m_instanceCount, vertexBuffer.GetVertexCount());
    }
}

}