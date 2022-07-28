#include "Box.h"
#include "../BindableBase.h"

using namespace D3D11;

Box::Box(D3D11Core& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	DirectX::XMFLOAT3 material)
	:
	BaseGameObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!IsStaticInitialized())
	{
		auto geo = m_GeoGen.CreateBox(1.5, 1.5, 1.5, 1);
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, geo.Vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		auto ib = std::make_unique<IndexBuffer>(gfx, geo.GetIndices16());
		AddStaticIndexBuffer(std::move(ib));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TANGENTU",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<Transform>(gfx, *this));

	MaterialConstants.color = material;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, MaterialConstants, 1u));
}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_Transform) * BaseGameObject::GetTransform();
}