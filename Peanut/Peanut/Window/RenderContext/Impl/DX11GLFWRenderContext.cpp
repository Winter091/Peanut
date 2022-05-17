#include "DX11GLFWRenderContext.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>
#include <Peanut/Core/Core.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <d3d11.h>

namespace pn
{


DX11GLFWRenderContext* DX11GLFWRenderContext::s_currentContext = nullptr;


DX11GLFWRenderContext::DX11GLFWRenderContext()
{
    PN_CORE_INFO("Using DX11-GLFW Render Context");
}

DX11GLFWRenderContext::~DX11GLFWRenderContext()
{
	m_swapChain->SetFullscreenState(false, nullptr);

	m_renderTargetView->Release();
	m_deviceContext->Release();
	m_device->Release();
	m_swapChain->Release();
}

void DX11GLFWRenderContext::PreWindowSetup()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

static DXGI_SWAP_CHAIN_DESC SetupSwapChainDesc(HWND window, int windowWidth, int windowHeight);

void DX11GLFWRenderContext::PostWindowSetup(Window& window)
{
    auto* adapter = GetPrimaryAdapter();
    
    uint32_t contextFlags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
#ifdef PN_DEBUG
    contextFlags |= D3D11_CREATE_DEVICE_DEBUG;
    PN_CORE_INFO("Requesting DirectX 11.1 with debug layer");
#else
    PN_CORE_INFO("Requesting DirectX 11.1");
#endif

    D3D_FEATURE_LEVEL dxVersion = D3D_FEATURE_LEVEL_11_1;

    auto glfwWindow = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    auto winWindow = glfwGetWin32Window(glfwWindow);
    auto swapChain = SetupSwapChainDesc(winWindow, window.GetWidth(), window.GetHeight());

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, // Use default hardware render adapter
        nullptr,
        contextFlags,
        &dxVersion, 1,
        D3D11_SDK_VERSION,
        &swapChain,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_deviceContext
    );

    if (result != S_OK) {
        PN_CORE_ASSERT(false, "Unable to create swap chain!");
    }

    SetupRenderTarget();
    SetupDepthStencilBuffer(window.GetWidth(), window.GetHeight());

    SetCurrentContext(window);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    PrintAdapterInfo(adapter);
}

static DXGI_SWAP_CHAIN_DESC SetupSwapChainDesc(HWND window, int windowWidth, int windowHeight)
{
    DXGI_SWAP_CHAIN_DESC swapChain;
    memset(&swapChain, 0, sizeof(swapChain));

    swapChain.BufferCount = 1;
    swapChain.BufferDesc.Width = windowWidth;
    swapChain.BufferDesc.Height = windowHeight;
    swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    swapChain.BufferDesc.RefreshRate.Numerator = 0;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;

	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChain.OutputWindow = window;

	// Turn multisampling off
	swapChain.SampleDesc.Count = 1;
	swapChain.SampleDesc.Quality = 0;

    // TODO: handle full screen mode
	swapChain.Windowed = true;

	swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChain.Flags = 0;

    return swapChain;
}

IDXGIAdapter* DX11GLFWRenderContext::GetPrimaryAdapter()
{
    IDXGIFactory* factory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

    IDXGIAdapter* adapter;
	factory->EnumAdapters(0, &adapter);

    return adapter;
}

void DX11GLFWRenderContext::PrintAdapterInfo(IDXGIAdapter* adapter)
{
    DXGI_ADAPTER_DESC adapterInfo;
	adapter->GetDesc(&adapterInfo);

    char buf[128] = {0};
    for (int i = 0; i < 128 && adapterInfo.Description[i]; i++) {
        buf[i] = static_cast<char>(adapterInfo.Description[i]);
    }

	uint32_t vramMb = (uint32_t)(adapterInfo.DedicatedVideoMemory / 1024 / 1024);

    PN_CORE_INFO("==========================================");
    PN_CORE_INFO("\tAdapter: {}", buf);
    PN_CORE_INFO("\tVRAM: {} MB", vramMb);
    PN_CORE_INFO("==========================================");
}

void DX11GLFWRenderContext::SetupRenderTarget()
{
    ID3D11Texture2D* backBufferPtr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**)&backBufferPtr);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "IDXGISwapChain::GetBuffer() failed");
    }

    hr = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "ID3D11Device::CreateRenderTargetView() failed");
    }

    backBufferPtr->Release();
}

void DX11GLFWRenderContext::SetupDepthStencilBuffer(int windowWidth, int windowHeight)
{
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    memset(&depthBufferDesc, 0, sizeof(depthBufferDesc));
	depthBufferDesc.Width = windowWidth;
	depthBufferDesc.Height = windowHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
    {
        HRESULT result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBufferTexture);
        if(FAILED(result)) {
            PN_CORE_ASSERT(false, "Failed to create depth buffer texture");
        }
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    {
        HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
        if(FAILED(result)) {
            PN_CORE_ASSERT(false, "Failed to create depth stencil state");
        }
        
        int stencilRef = 1;
        m_deviceContext->OMSetDepthStencilState(m_depthStencilState, stencilRef);
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
    {
        HRESULT result = m_device->CreateDepthStencilView(
            m_depthStencilBufferTexture, &depthStencilViewDesc, &m_depthStencilView);
        if (FAILED(result)) {
            PN_CORE_ASSERT(false, "Failed to create depth stencil view");
        }
    }
}

void DX11GLFWRenderContext::OnWindowResize(Window& /*window*/)
{
    m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView->Release();

    HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "IDXGISwapChain::ResizeBuffers() failed");
    }

    SetupRenderTarget();
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void DX11GLFWRenderContext::SetCurrentContext(Window& /*window*/)
{
    s_currentContext = this;
}

void DX11GLFWRenderContext::SetSwapInterval(int interval)
{
    m_swapInterval = interval;
}

void DX11GLFWRenderContext::SwapBuffers(Window& /*window*/)
{
    m_swapChain->Present(m_swapInterval, 0);
}

DX11GLFWRenderContext& DX11GLFWRenderContext::GetCurrentContext()
{
    PN_CORE_ASSERT(s_currentContext, "Trying to get context, but it's not set");
    return *s_currentContext;
}

}