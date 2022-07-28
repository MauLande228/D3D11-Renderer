#include "LightBulb.h"
#include "../BindableBase.h"

using namespace D3D11;

LightBulb::LightBulb(D3D11::D3D11Core& gfx, float radius)
{
	auto geo = m_GeoGen.CreateSphere(radius, 30, 30);
	//auto geo = m_GeoGen.CreateCylinder(radius, radius, 3, 20, 20);
	AddBind(std::make_unique<VertexBuffer>(gfx, geo.Vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"LightBulbVS.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"LightBulbPS.cso"));

	auto ib = std::make_unique<IndexBuffer>(gfx, geo.GetIndices16());
	//m_Count = ib->GetCount();
	AddIndexBuffer(std::move(ib));

	struct PSColorConstant
	{
		DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
		float padding;
	}ColorConst;

	AddBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, ColorConst));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TANGENTU",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<Transform>(gfx, *this));
}

void LightBulb::Update(float dt) noexcept
{
}

void LightBulb::SetPosition(DirectX::XMFLOAT3 pos) noexcept
{
	m_Position = pos;
}

DirectX::XMMATRIX LightBulb::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}
