#include "Sampler.h"

D3D11::Sampler::Sampler(D3D11Core& gfx)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_EXC(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_Sampler));
}

void D3D11::Sampler::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());
}
