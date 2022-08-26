#pragma once

#include <Peanut/Render/Shaders/Shader.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

    class Dx11Shader final : public Shader
    {
    public:
        Dx11Shader(const ShaderPaths& paths, std::string name = "");
        Dx11Shader(const ShaderSources& sources, std::string name = "");
        ~Dx11Shader() override;

        const std::string& GetName() const override { return m_name; }

        ID3DBlob* GetVertexShaderBlob() const { return m_vertexShaderBlob.Get(); }
        ID3D11VertexShader* GetNativeVertexShaderObj() const { return m_vertexShader.Get(); }
        ID3D11PixelShader* GetNativeFragmentShaderObj() const { return m_fragmentShader.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3DBlob> m_vertexShaderBlob;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_fragmentShader;
        std::string m_name;

    private:
        void InitShaders(const ShaderSources& sources);
    };

}
