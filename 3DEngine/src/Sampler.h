#pragma once

#include "Bindable.h"

namespace D3D11
{
	class Sampler : public Bindable
	{
	public:
		Sampler(D3D11Core& gfx);

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}