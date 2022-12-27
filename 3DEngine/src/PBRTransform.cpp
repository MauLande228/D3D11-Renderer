#include "PBRTransform.h"

D3D11::PBRTransform::PBRTransform(D3D11Core& gfx, const Actor& parent, Camera& camera, UINT slot) :
	m_Parent(parent),
	m_Camera(camera)
{
	if (!m_ConstantBuffer)
	{
		m_ConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void D3D11::PBRTransform::Bind(D3D11Core& gfx) noexcept
{
	UpdateBind(gfx, GetTransforms(gfx));
}

void D3D11::PBRTransform::UpdateBind(D3D11Core& gfx, const Transforms& tf) noexcept
{
	m_ConstantBuffer->Update(gfx, tf);
	m_ConstantBuffer->Bind(gfx);
}


D3D11::PBRTransform::Transforms D3D11::PBRTransform::GetTransforms(D3D11Core& gfx) noexcept
{
	const auto modelView = m_Parent.GetTransform() * gfx.GetView();
	auto CamPos = m_Camera.GetPosition();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		),
		CamPos
	};

	return tf;
}

std::unique_ptr<D3D11::VertexConstantBuffer<D3D11::PBRTransform::Transforms>> D3D11::PBRTransform::m_ConstantBuffer;