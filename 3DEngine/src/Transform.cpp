#include "Transform.h"

D3D11::Transform::Transform(D3D11Core& gfx, const Actor& parent, UINT slot) :
	m_Parent(parent)
{
	if (!m_ConstantBuffer)
	{
		m_ConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void D3D11::Transform::Bind(D3D11Core& gfx) noexcept
{
	const auto modelView = m_Parent.GetTransform() * gfx.GetView();

	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};

	m_ConstantBuffer->Update(gfx, tf);
	m_ConstantBuffer->Bind(gfx);
}

std::unique_ptr<D3D11::VertexConstantBuffer<D3D11::Transform::Transforms>> D3D11::Transform::m_ConstantBuffer;