#include "Transform.h"

D3D11::Transform::Transform(D3D11Core& gfx, const Actor& parent) :
	m_Parent(parent)
{
	if (!m_ConstantBuffer)
	{
		m_ConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void D3D11::Transform::Bind(D3D11Core& gfx) noexcept
{
	m_ConstantBuffer->Update(
		gfx,
		DirectX::XMMatrixTranspose(
			m_Parent.GetTransform() * gfx.GetProjection()
		));

	m_ConstantBuffer->Bind(gfx);
}

std::unique_ptr<D3D11::VertexConstantBuffer<DirectX::XMMATRIX>> D3D11::Transform::m_ConstantBuffer;