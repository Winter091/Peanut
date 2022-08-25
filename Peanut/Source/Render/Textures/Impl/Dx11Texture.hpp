#pragma once

#include <Peanut/Render/Textures/Texture.hpp>

struct ID3D11ShaderResourceView;

namespace pn {

    class Dx11Texture : virtual public Texture
    {
    public:
        ~Dx11Texture() override = default;

        bool operator==(const Texture& other) const override { return m_handle == dynamic_cast<const Dx11Texture&>(other).GetNativeTexturePtr(); }

        void* GetNativeTexturePtr() const { return m_handle; }
        ID3D11ShaderResourceView* GetNativeViewPtr() const { return m_view; }

    protected:
        void SetNativeTexturePtr(void* nativeObject) { m_handle = nativeObject; }
        void SetNativeViewPtr(void* nativeObject) { m_view = reinterpret_cast<ID3D11ShaderResourceView*>(nativeObject); }

    private:
        void* m_handle = nullptr;
        ID3D11ShaderResourceView* m_view = nullptr;
    };

}
