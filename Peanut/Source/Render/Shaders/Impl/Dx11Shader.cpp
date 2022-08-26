#include "Dx11Shader.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Window/RenderContextImpl/Dx11RenderContext.hpp>

#include <fstream>
#include <sstream>
#include <vector>

#include <d3dcompiler.h>

namespace pn
{

static std::string ReadFile(const std::string& filePath)
{
	PN_PROFILE_FUNCTION();

	std::ifstream file(filePath);
	PN_CORE_ENSURE(file, "Unable to open shader source file: {}", filePath);

	std::stringstream ss;
	ss << file.rdbuf();

	return ss.str();
}

static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::string& source, LPCSTR profile)
{
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if PN_DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT res = D3DCompile(
		(void*)source.c_str(),
		source.size(),
		nullptr,
		nullptr,
		((ID3DInclude*)(UINT_PTR)1), // default include
		"main", 
		profile,
		flags,
		0,
		shaderBlob.GetAddressOf(),
		errorBlob.GetAddressOf()
	);
	
	PN_CORE_ASSERT(res == S_OK, "Unable to compile shader ({}): {}", profile, (const char*)errorBlob->GetBufferPointer());

	return shaderBlob;
}

Dx11Shader::Dx11Shader(const ShaderPaths& paths, std::string name)
    : m_name(std::move(name))
{
    PN_PROFILE_FUNCTION();

    ShaderSources sources;
    sources.VertexSource = ReadFile(paths.VertexPath);
    sources.FragmentSource = ReadFile(paths.FragmentPath);

    InitShaders(sources);
}



Dx11Shader::Dx11Shader(const ShaderSources& sources, std::string name)
    : m_name(std::move(name))
{
	InitShaders(sources);
}

void Dx11Shader::InitShaders(const ShaderSources& sources)
{
	auto* device = Dx11RenderContext::GetCurrentContext().GetDevice();

	m_vertexShaderBlob = CompileShader(sources.VertexSource, "vs_5_0");
	HRESULT res = device->CreateVertexShader(
		m_vertexShaderBlob->GetBufferPointer(),
		m_vertexShaderBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()
	);
	PN_CORE_ASSERT(res == S_OK, "Unable to create vertex shader from compiled source");

	auto fragmentShaderBlob = CompileShader(sources.FragmentSource, "ps_5_0");
	res = device->CreatePixelShader(
		fragmentShaderBlob->GetBufferPointer(),
		fragmentShaderBlob->GetBufferSize(),
		nullptr,
		m_fragmentShader.GetAddressOf()
	);
	PN_CORE_ASSERT(res == S_OK, "Unable to create fragment shader from compiled source");
}

Dx11Shader::~Dx11Shader()
{
}

}