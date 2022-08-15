#include "OpenGLVertexArray.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>
#include "OpenGLVertexBuffer.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLConstantBuffer.hpp"

#include <glad/glad.h>

#include <unordered_set>

namespace pn
{

OpenGLVertexArray::OpenGLVertexArray(const VertexArrayDescription& description)
    : m_constantBuffers(description.ConstantBuffers)
    , m_shader(description._Shader)
{
    glCreateVertexArrays(1, &m_vaoHandle);

    for (const auto& vertexBuffer : description.VertexBuffers) {
        AddVertexBuffer(vertexBuffer);
    }

    if (description._IndexBuffer) {
        SetIndexBuffer(description._IndexBuffer);
    }
}
    
OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_vaoHandle);
}

void OpenGLVertexArray::Bind()
{
    glBindVertexArray(m_vaoHandle);

    for (uint32_t i = 0; i < m_constantBuffers.size(); i++) {
        uint32_t handle = static_cast<OpenGLConstantBuffer&>(*m_constantBuffers[i]).GetOpenGLHandle();
        glBindBufferBase(GL_UNIFORM_BUFFER, i, handle);
    }

    m_shader->Bind();
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    PN_PROFILE_FUNCTION();

    glBindVertexArray(m_vaoHandle);

    uint32_t bindingIndex = static_cast<uint32_t>(m_vertexBuffers.size());
    uint32_t vertexBufferHandle = static_cast<OpenGLVertexBuffer&>(*vertexBuffer).GetOpenGLHandle();
    glBindVertexBuffer(bindingIndex, vertexBufferHandle, 0, vertexBuffer->GetVertexSize());

    if (vertexBuffer->GetDataUsage() == pn::VertexBufferDataUsage::PerVertex) {
        glVertexBindingDivisor(bindingIndex, 0);
    } else {
        glVertexBindingDivisor(bindingIndex, 1);
        UpdateInstanceCount(*vertexBuffer);
    }

    ProcessVertexBufferLayout(vertexBuffer.get(), bindingIndex);
    m_vertexBuffers.push_back(vertexBuffer);

#if defined(PN_DEBUG)
    AssertAllAttributeIndicesAreUnique();
#endif
}

void OpenGLVertexArray::UpdateInstanceCount(const VertexBuffer& vertexBuffer)
{
    if (m_instanceCount == 0) {
        m_instanceCount = vertexBuffer.GetVertexCount();
    } else {
        m_instanceCount = std::min(m_instanceCount, vertexBuffer.GetVertexCount());
    }
}

void OpenGLVertexArray::ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex)
{
    PN_PROFILE_FUNCTION();

    const auto& layout = vertexBuffer->GetLayout();
    const auto& layoutElements = layout->GetElements();

    for (const auto& elem : layoutElements) {
        if (elem.GetType() == BufferLayoutElementType::Mat4) {
            for (int i = 0; i < 4; i++) {
                glVertexAttribFormat(elem.GetIndex() + i, 4, GL_FLOAT, false, static_cast<GLuint>(elem.GetOffset() + i * (4 * sizeof(float))));
                glVertexAttribBinding(elem.GetIndex() + i, bindingIndex);
                glEnableVertexAttribArray(elem.GetIndex() + i);
            }
        } else {
            if (IsBufferLayoutElementTypeInt(elem.GetType())) {
                glVertexAttribIFormat(elem.GetIndex(), elem.GetCount(), BufferLayoutElementTypeToGlType(elem.GetType()), static_cast<GLuint>(elem.GetOffset()));
            } else {
                glVertexAttribFormat(elem.GetIndex(), elem.GetCount(), BufferLayoutElementTypeToGlType(elem.GetType()), false, static_cast<GLuint>(elem.GetOffset()));
            }

            glVertexAttribBinding(elem.GetIndex(), bindingIndex);
            glEnableVertexAttribArray(elem.GetIndex());
        }

    }
}

void OpenGLVertexArray::AssertAllAttributeIndicesAreUnique() const
{
    PN_PROFILE_FUNCTION();

    std::unordered_set<uint32_t> seenIndices;

    for (const auto& buffer : m_vertexBuffers) {
        const auto& bufferAttributes = buffer->GetLayout()->GetElements();

        for (const BufferLayoutElement& elem : bufferAttributes) {
            if (!seenIndices.insert(elem.GetIndex()).second) {
                PN_CORE_ASSERT(false, "At least 2 attributes in pipeline state have the same index = {}", elem.Index);
            }
        }
    }
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_indexBuffer = indexBuffer;

    uint32_t handle = indexBuffer ? static_cast<OpenGLIndexBuffer&>(*indexBuffer).GetOpenGLHandle() : 0;

    glBindVertexArray(m_vaoHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void OpenGLVertexArray::SetConstantBuffers(const std::vector<std::shared_ptr<ConstantBuffer>>& constantBuffers)
{
    m_constantBuffers = constantBuffers;
}

uint32_t OpenGLVertexArray::GetVertexCount() const
{
    PN_CORE_ASSERT(!m_vertexBuffers.empty(), "No vertex buffers are bound to pipeline state");
    return m_vertexBuffers.front()->GetVertexCount();
}

uint32_t OpenGLVertexArray::GetIndexCount() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetIndexCount();
}

uint32_t OpenGLVertexArray::GetInstanceCount() const 
{
    return m_instanceCount == 0 ? 1 : m_instanceCount;
}

IndexBufferDataFormat OpenGLVertexArray::GetIndexDataFormat() const 
{
    PN_CORE_ASSERT(m_indexBuffer, "Index buffer is not set");
    return m_indexBuffer->GetDataFormat();
}

}