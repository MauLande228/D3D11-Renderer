#include "Sampler.h"
#include "ResourcePool.h"

D3D11::Sampler::Sampler(D3D11Core& gfx)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	GFX_EXC(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_Sampler));
}

void D3D11::Sampler::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());
}

std::shared_ptr<D3D11::Sampler> D3D11::Sampler::Resolve(D3D11Core& gfx)
{
	return Pool::Resolve<D3D11::Sampler>(gfx);
}

std::string D3D11::Sampler::GenerateUID()
{
	return typeid(D3D11::Sampler).name();
}

std::string D3D11::Sampler::GetUID() const noexcept
{
	return GenerateUID();
}
