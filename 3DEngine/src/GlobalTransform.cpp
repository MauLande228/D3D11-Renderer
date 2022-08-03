#include "GlobalTransform.h"

D3D11::GlobalTransform::GlobalTransform(D3D11Core& gfx, const Actor& parent, UINT slotV, UINT slotP) :
	Transform(gfx, parent, slotV)
{
	if (!m_ConstantBufferPS)
	{
		m_ConstantBufferPS = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
	}
}

void D3D11::GlobalTransform::Bind(D3D11Core& gfx) noexcept
{
	const auto tf = GetTransforms(gfx);
	Transform::UpdateBind(gfx, tf);
	UpdateBind(gfx, tf);
}

void D3D11::GlobalTransform::UpdateBind(D3D11Core& gfx, const Transforms& tf) noexcept
{
	m_ConstantBufferPS->Update(gfx, tf);
	m_ConstantBufferPS->Bind(gfx);
}

std::unique_ptr<D3D11::PixelConstantBuffer<D3D11::Transform::Transforms>> D3D11::GlobalTransform::m_ConstantBufferPS;
