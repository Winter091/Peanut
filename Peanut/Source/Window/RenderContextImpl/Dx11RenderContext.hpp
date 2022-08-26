#pragma once

#include <Peanut/Window/RenderContext.hpp>

#include <d3d11.h>

namespace pn {

	class Dx11RenderContext : public RenderContext
	{
	public:
		~Dx11RenderContext() override = default;

		ID3D11Device* GetDevice() { return m_device; }
		ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
		ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
		ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }

		static Dx11RenderContext& GetCurrentContext() { return *s_currentContext; }

	protected:
		void SetCurrentContext(Dx11RenderContext* context) { s_currentContext = context; }
		void SetDevice(ID3D11Device* device) { m_device = device; }
		void SetDeviceContext(ID3D11DeviceContext* deviceContext) { m_deviceContext = deviceContext; }
		void SetRenderTargetView(ID3D11RenderTargetView* renderTargetView) { m_renderTargetView = renderTargetView;}
		void SetDepthStencilView(ID3D11DepthStencilView* depthStencilView) { m_depthStencilView = depthStencilView; }

	private:
		static inline Dx11RenderContext* s_currentContext;

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11DepthStencilView* m_depthStencilView;

	};

}