#include "Transform.h"

D3D11::Transform::Transform(D3D11Core& gfx, const Actor& parent) :
	m_ConstantBuffer(gfx),
	m_Parent(parent)
{
}

void D3D11::Transform::Bind(D3D11Core& gfx) noexcept
{
	m_ConstantBuffer.Update(
		gfx,
		DirectX::XMMatrixTranspose(
			m_Parent.GetTransform() * gfx.GetProjection()
		));

	m_ConstantBuffer.Bind(gfx);
}
