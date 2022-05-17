#include "DX11GLFWRenderContext.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>
#include <Peanut/Core/Core.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace pn
{


// Static members
DX11GLFWRenderContext* DX11GLFWRenderContext::s_currentContext = nullptr;


IDXGIAdapter* GetPrimaryAdapter()
{
    IDXGIFactory1* factory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory);

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


static DXGI_SWAP_CHAIN_DESC GetSwapChainDesc(HWND window, int windowWidth, int windowHeight)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    memset(&swapChainDesc, 0, sizeof(swapChainDesc));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = windowWidth;
    swapChainDesc.BufferDesc.Height = windowHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = window;

	// Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

    // TODO: handle full screen mode
	swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

    return swapChainDesc;
}


static ID3D11RenderTargetView* CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain)
{
    ID3D11Texture2D* backBufferPtr;
    HRESULT hr = swapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**)&backBufferPtr);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "IDXGISwapChain::GetBuffer() failed");
    }

    ID3D11RenderTargetView* renderTargetView;
    hr = device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "ID3D11Device::CreateRenderTargetView() failed");
    }

    backBufferPtr->Release();
    return renderTargetView;
}


static ID3D11Texture2D* CreateDepthStencilBufferTexture(ID3D11Device* device, int windowWidth, int windowHeight)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    memset(&textureDesc, 0, sizeof(textureDesc));
	textureDesc.Width = windowWidth;
	textureDesc.Height = windowHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

    ID3D11Texture2D* texture;
    HRESULT result = device->CreateTexture2D(&textureDesc, nullptr, &texture);
    if(FAILED(result)) {
        PN_CORE_ASSERT(false, "Failed to create depth buffer texture");
    }

    return texture;
}


static ID3D11DepthStencilState* CreateDepthStencilState(ID3D11Device* device)
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = false;
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

    ID3D11DepthStencilState* state;
    HRESULT result = device->CreateDepthStencilState(&depthStencilDesc, &state);
    if(FAILED(result)) {
        PN_CORE_ASSERT(false, "Failed to create depth stencil state");
    }

    return state;
}


static ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* device, ID3D11Texture2D* depthStencilTexture)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

    ID3D11DepthStencilView* view;
    HRESULT result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &view);
    if (FAILED(result)) {
        PN_CORE_ASSERT(false, "Failed to create depth stencil view");
    }

    return view;
}


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

void DX11GLFWRenderContext::PostWindowSetup(Window& window)
{
    bool useDebugContext = false;
#ifdef PN_DEBUG
    useDebugContext = true;
#endif

    GLFWwindow* glfwWindow = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    HWND win32Window = glfwGetWin32Window(glfwWindow);

    CreateDeviceAndSwapChain(win32Window, window.GetWidth(), window.GetHeight(), useDebugContext);

    m_renderTargetView = CreateRenderTargetView(m_device, m_swapChain);
    InitDepthBuffer(window.GetWidth(), window.GetHeight());

    SetCurrentContext(window);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}


void DX11GLFWRenderContext::CreateDeviceAndSwapChain(HWND window, int windowWidth, int windowHeight, bool createDebugContext)
{
    IDXGIAdapter* adapter = GetPrimaryAdapter();
    PrintAdapterInfo(adapter);
    
    uint32_t contextFlags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
    if (createDebugContext) {
        contextFlags |= D3D11_CREATE_DEVICE_DEBUG;
        PN_CORE_INFO("Requesting DirectX 11.1 with debug layer");
    } else {
        PN_CORE_INFO("Requesting DirectX 11.1");
    }

    D3D_FEATURE_LEVEL dxVersion = D3D_FEATURE_LEVEL_11_1;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = GetSwapChainDesc(window, windowWidth, windowHeight);

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        adapter, D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        contextFlags,
        &dxVersion, 1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_deviceContext
    );

    if (result != S_OK) {
        PN_CORE_ASSERT(false, "Unable to create swap chain!");
    }
}


void DX11GLFWRenderContext::InitDepthBuffer(int windowWidth, int windowHeight)
{
    m_depthStencilState = CreateDepthStencilState(m_device);
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

    RecreateDepthBuffer(windowWidth, windowHeight);
}

void DX11GLFWRenderContext::RecreateDepthBuffer(int windowWidth, int windowHeight)
{
    if (m_depthStencilView) {
        m_depthStencilView->Release();
    }

    if (m_depthStencilBufferTexture) {
        m_depthStencilBufferTexture->Release();
    }
    
    m_depthStencilBufferTexture = CreateDepthStencilBufferTexture(m_device, windowWidth, windowHeight);
    m_depthStencilView = CreateDepthStencilView(m_device, m_depthStencilBufferTexture);
}

void DX11GLFWRenderContext::OnWindowResize(Window& window)
{
    m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    {
        m_renderTargetView->Release();

        HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(hr)) {
            PN_CORE_ASSERT(false, "IDXGISwapChain::ResizeBuffers() failed");
        }

        m_renderTargetView = CreateRenderTargetView(m_device, m_swapChain);    
        RecreateDepthBuffer(window.GetWidth(), window.GetHeight());
    }

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