#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Render/Commands/Impl/OpenGLRenderCommand.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Render/Commands/Impl/Dx11RenderCommand.hpp>
#endif

namespace pn {

    RenderAPI s_renderAPI;
    bool s_isInitialized = false;
    std::unique_ptr<RenderCommandImpl> s_renderCommandImpl;

    void RenderCommand::Init()
    {
        PN_CORE_ASSERT(!s_isInitialized, "RenderCommand can be initialized only once");
        s_isInitialized = true;

    #if defined(PN_RENDERING_OPENGL)
        s_renderAPI = RenderAPI::OpenGL;
        s_renderCommandImpl = std::make_unique<OpenGLRenderCommand>();
        PN_CORE_INFO("Using OpenGL Renderer");
    #elif defined(PN_RENDERING_DX11)
        s_renderAPI = RenderAPI::Dx11;
        s_renderCommandImpl = std::make_unique<Dx11RenderCommand>();
        PN_CORE_INFO("Using DirectX 11 Renderer");
    #else
        PN_CORE_ASSERT(false, "RenderCommand: cannot determine rendering backend");
    #endif
    }

    void RenderCommand::Shutdown()
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand is not initialized, unable to shutdown");
        s_isInitialized = false;

        s_renderAPI = RenderAPI::None;
        s_renderCommandImpl.reset(nullptr);
    }

    RenderAPI RenderCommand::GetRenderAPI()
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        return s_renderAPI;
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        return s_renderCommandImpl->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        return s_renderCommandImpl->Clear();
    }

    void RenderCommand::SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height, uint32_t totalHeight)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->SetViewport(leftX, bottomY, width, height, totalHeight);
    }

    void RenderCommand::Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->Draw(vertexArray, count);
    }

    void RenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->DrawIndexed(vertexArray, count);
    }

    void RenderCommand::DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->DrawInstanced(vertexArray, count, instanceCount);
    }

    void RenderCommand::DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->DrawIndexedInstanced(vertexArray, count, instanceCount);
    }

    void RenderCommand::BindFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->BindFramebuffer(framebuffer);
    }

    void RenderCommand::BindShader(const std::shared_ptr<Shader>& shader)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->BindShader(shader);
    }

    void RenderCommand::BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->BindConstantBuffers(constantBuffers, amount, startSlot);
    }

    void RenderCommand::BindTextures(const std::shared_ptr<Texture>* textures, size_t amount, uint32_t startSlot)
    {
        PN_CORE_ASSERT(s_isInitialized, "RenderCommand has to be initialized before usage");
        s_renderCommandImpl->BindTextures(textures, amount, startSlot);
    }

}
