#include "OpenGLRenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Render/Buffers/Impl/OpenGLIndexBuffer.hpp>
#include <Render/Textures/Impl/OpenGLTexture.hpp>
#include <Render/Textures/Impl/OpenGLTextureSampler.hpp>
#include <Render/Buffers/Impl/OpenGLVertexArray.hpp>
#include <Render/Buffers/Impl/OpenGLConstantBuffer.hpp>
#include <Render/Shaders/Impl/OpenGLShader.hpp>

#include <glad/glad.h>

namespace pn {

    void OpenGLRenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderCommand::SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height, uint32_t totalHeight)
    {
        (void)totalHeight;
        glViewport(leftX, bottomY, width, height);
    }

    void OpenGLRenderCommand::Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        PN_PROFILE_FUNCTION();

        if (count == 0) {
            count = vertexArray->GetVertexCount();
        }

        BindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
    }

    void OpenGLRenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        PN_PROFILE_FUNCTION();

        if (count == 0) {
            count = vertexArray->GetIndexCount();
        }

        uint32_t dataType = GetGLDataType(vertexArray->GetIndexDataFormat());

        BindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, count, dataType, nullptr);
    }

    void OpenGLRenderCommand::DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        PN_PROFILE_FUNCTION();

        if (count == 0) {
            count = vertexArray->GetVertexCount();
        }

        if (instanceCount == 0) {
            instanceCount = vertexArray->GetInstanceCount();
        }

        BindVertexArray(vertexArray);
        glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(count), instanceCount);
    }

    void OpenGLRenderCommand::DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        PN_PROFILE_FUNCTION();

        if (count == 0) {
            count = vertexArray->GetIndexCount();
        }

        if (instanceCount == 0) {
            instanceCount = vertexArray->GetInstanceCount();
        }

        uint32_t dataType = GetGLDataType(vertexArray->GetIndexDataFormat());

        BindVertexArray(vertexArray);
        glDrawElementsInstanced(GL_TRIANGLES, count, dataType, nullptr, instanceCount);
    }

    void OpenGLRenderCommand::BindVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
    {
        const auto& glVertexArray = static_cast<const OpenGLVertexArray&>(*vertexArray);
        glBindVertexArray(glVertexArray.GetOpenGLHandle());
    }

    void OpenGLRenderCommand::BindShader(const std::shared_ptr<Shader>& shader)
    {
        const auto& glShader = static_cast<const OpenGLShader&>(*shader);
        glUseProgram(glShader.GetOpenGLHandle());
    }

    void OpenGLRenderCommand::BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot)
    {
        PN_CORE_ASSERT(amount > 0 && amount <= 16, "Can't bind amount of constant buffers that is not it range [1, 16]");
    
        static std::vector<uint32_t> bufferHandles(16);

        for (uint32_t i = 0; i < amount; i++) {
            bufferHandles[i] = static_cast<const OpenGLConstantBuffer&>(*constantBuffers[i]).GetOpenGLHandle();
        }

        glBindBuffersBase(GL_UNIFORM_BUFFER, startSlot, static_cast<GLsizei>(amount), &bufferHandles[0]);
    }

    void OpenGLRenderCommand::BindTextures(const std::shared_ptr<Texture>* textures, size_t amount, uint32_t startSlot)
    {
        PN_CORE_ASSERT(amount > 0 && amount <= 16, "Can't bind amount of textures that is not it range [1, 16]");

        static std::vector<uint32_t> textureHandles(16);
        static std::vector<uint32_t> textureSamplers(16);

        for (uint32_t i = 0; i < amount; i++) {
            textureHandles[i] = dynamic_cast<const OpenGLTexture&>(*textures[i]).GetOpenGLHandle();
            textureSamplers[i] = static_cast<const OpenGLTextureSampler&>(*textures[i]->GetSampler()).GetOpenGLHandle();
        }

        glBindTextures(startSlot, static_cast<GLsizei>(amount), &textureHandles[0]);
        glBindSamplers(startSlot, static_cast<GLsizei>(amount), &textureSamplers[0]);
    }

    uint32_t OpenGLRenderCommand::GetGLDataType(IndexBufferDataFormat format)
    {
        switch (format) {
            case IndexBufferDataFormat::Uint16: return GL_UNSIGNED_SHORT;
            case IndexBufferDataFormat::Uint32: return GL_UNSIGNED_INT;
            default: break;
        }

        PN_CORE_ASSERT(false, "Unknown index buffer data format: {}", static_cast<uint32_t>(format));
        return 0u;
    }

}
