#pragma once

#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>

#include <d3d11.h>
#include <wrl/client.h>

namespace pn {

    class Dx11ShaderInputLayout final : public ShaderInputLayout
    {
    public:
        Dx11ShaderInputLayout(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader);
        ~Dx11ShaderInputLayout() override = default;

        ID3D11InputLayout* Get() { return m_layout.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
    };

}
