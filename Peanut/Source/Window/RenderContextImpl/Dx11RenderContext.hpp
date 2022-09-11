#pragma once

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Window/RenderContext.hpp>

#include <d3d11.h>

namespace pn {

	class Dx11RenderContext : public RenderContext
	{
	public:
		~Dx11RenderContext() override = default;

		ID3D11Device* GetDevice() 
		{ 
			PN_CORE_ASSERT(m_device, "Trying to get device, but it's nullptr");
			return m_device; 
		}
		ID3D11DeviceContext* GetDeviceContext() 
		{ 
			PN_CORE_ASSERT(m_deviceContext, "Trying to get device context, but it's nullptr");
			return m_deviceContext; 
		}
		
		ID3D11RenderTargetView* GetRenderTargetView() 
		{ 
			PN_CORE_ASSERT(m_renderTargetView, "Trying to get render target view, but it's nullptr");
			return m_renderTargetView; 
		}

		ID3D11DepthStencilView* GetDepthStencilView() 
		{ 
			PN_CORE_ASSERT(m_depthStencilView, "Trying to get depth stencil view, but it's nullptr");
			return m_depthStencilView; 
		}

		static Dx11RenderContext& GetCurrentContext() 
		{
			PN_CORE_ASSERT(s_currentContext, "Trying to get current context, but it's nullptr");
			return *s_currentContext; 
		}

	protected:
		void SetCurrentContext(Dx11RenderContext* context) { s_currentContext = context; }
		void SetDevice(ID3D11Device* device) { m_device = device; }
		void SetDeviceContext(ID3D11DeviceContext* deviceContext) { m_deviceContext = deviceContext; }
		void SetRenderTargetView(ID3D11RenderTargetView* renderTargetView) { m_renderTargetView = renderTargetView; }
		void SetDepthStencilView(ID3D11DepthStencilView* depthStencilView) { m_depthStencilView = depthStencilView; }

	private:
		static inline Dx11RenderContext* s_currentContext;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11DepthStencilView* m_depthStencilView;
	};

}
