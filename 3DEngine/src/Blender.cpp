#include "Blender.h"
#include "ResourcePool.h"

D3D11::Blender::Blender(D3D11Core& gfx, bool blending, std::optional<float> factors) :
	m_bBlending(blending)
{
	if (factors)
	{
		m_Factors.emplace();
		m_Factors->fill(*factors);
	}

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	auto& brt = blendDesc.RenderTarget[0];
	if (m_bBlending)
	{
		brt.BlendEnable = TRUE;
		if (factors)
		{
			brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
			brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
		}
		else
		{
			brt.SrcBlend = D3D11_BLEND_SRC1_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
	}
	GFX_EXC(GetDevice(gfx)->CreateBlendState(&blendDesc, &m_BlendState));
}

void D3D11::Blender::Bind(D3D11Core& gfx) noexcept
{
	const float* data = m_Factors ? m_Factors->data() : nullptr;
	GetContext(gfx)->OMSetBlendState(m_BlendState.Get(), data, 0xFFFFFFFF);
}

void D3D11::Blender::SetFactor(float factor) NOXND
{
	assert(m_Factors);
	return m_Factors->fill(factor);
}

float D3D11::Blender::GetFactor() const NOXND
{
	assert(m_Factors);
	return m_Factors->front();
}

std::shared_ptr<D3D11::Blender> D3D11::Blender::Resolve(D3D11Core& gfx, bool blending, std::optional<float> factor)
{
	return Pool::Resolve<D3D11::Blender>(gfx, blending, factor);
}

std::string D3D11::Blender::GenerateUID(bool blending, std::optional<float> factor)
{
	using namespace std::string_literals;
	return typeid(D3D11::Blender).name() + "#"s + (blending ? "b"s : "n"s) + (factor ? "#f"s + std::to_string(*factor) : "");
}

std::string D3D11::Blender::GetUID() const noexcept
{
	return GenerateUID(m_bBlending, m_Factors ? m_Factors->front() : std::optional<float>{});
}
