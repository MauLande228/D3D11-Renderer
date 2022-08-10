#include "Rasterizer.h"
#include "ResourcePool.h"

D3D11::Rasterizer::Rasterizer(D3D11Core& gfx, bool twoSided) :
	m_bTwoSided(twoSided)
{
	D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterDesc.CullMode = m_bTwoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

	GFX_EXC(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &m_Rasterizer));
}

void D3D11::Rasterizer::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->RSSetState(m_Rasterizer.Get());
}

std::shared_ptr<D3D11::Rasterizer> D3D11::Rasterizer::Resolve(D3D11Core& gfx, bool twosided)
{
	return Pool::Resolve<D3D11::Rasterizer>(gfx, twosided);
}

std::string D3D11::Rasterizer::GenerateUID(bool twoSided)
{
	using namespace std::string_literals;
	return typeid(D3D11::Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
}

std::string D3D11::Rasterizer::GetUID() const noexcept
{
	return GenerateUID(m_bTwoSided);
}
