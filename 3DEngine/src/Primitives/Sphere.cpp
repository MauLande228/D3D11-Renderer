#include "Sphere.h"
#include "SphereGen.h"
#include "GeosphereGen.h"
#include "../BindableBase.h"

using namespace D3D11;

Sphere::Sphere(D3D11::D3D11Core& gfx, Camera& camera, float radius)
{
	auto model = GeosphereGen::Make();
	//auto model = m_GeoGen.CreateSphere(radius, 10, 10);
	model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$spherePBR." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, "PBR_vs.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PBR_ps.cso"));

	AddBind(Texture::Resolve(gfx, "textures/PBR/rustediron2_basecolor.png"));
	AddBind(Texture::Resolve(gfx, "textures/PBR/rustediron2_metallic.png", 1));
	AddBind(Texture::Resolve(gfx, "textures/PBR/rustediron2_normal.png", 2));
	AddBind(Texture::Resolve(gfx, "textures/PBR/rustediron2_roughness.png", 3));

	AddBind(Sampler::Resolve(gfx));

	struct PSColorConstant
	{
		DirectX::XMFLOAT3 color = { 0.0f,1.0f,0.0f };
		float padding;
	} colorConst;
	//AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 1));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<PBRTransform>(gfx, *this, camera));
}

void Sphere::SetPosition(DirectX::XMFLOAT3 pos) noexcept
{
	m_Position = pos;
}

void Sphere::UpdateCB(Camera& camera) noexcept
{
}

DirectX::XMMATRIX Sphere::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}
