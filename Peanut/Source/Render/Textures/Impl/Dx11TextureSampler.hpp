#pragma once

#include <Peanut/Render/Textures/TextureSampler.hpp>

struct ID3D11SamplerState;

namespace pn {

	class Dx11TextureSampler final : public TextureSampler
	{
	public:
		Dx11TextureSampler(const TextureSamplerSettings& settings);
		~Dx11TextureSampler() override;

		ID3D11SamplerState* GetNativeObjectPtr() const { return m_handle; }

	private:
		ID3D11SamplerState* m_handle = 0;
	};

}

