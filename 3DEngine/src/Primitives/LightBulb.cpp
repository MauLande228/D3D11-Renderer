#include "LightBulb.h"
#include "SphereGen.h"
#include "../BindableBase.h"

using namespace D3D11;

LightBulb::LightBulb(D3D11::D3D11Core& gfx, float radius)
{
	auto model = SphereGen::Make();
	model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, "LightBulbVS.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "LightBulbPS.cso"));

	struct PSColorConstant
	{
		DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;
	AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Transform>(gfx, *this));
}

void LightBulb::SetPosition(DirectX::XMFLOAT3 pos) noexcept
{
	m_Position = pos;
}

DirectX::XMMATRIX LightBulb::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}
