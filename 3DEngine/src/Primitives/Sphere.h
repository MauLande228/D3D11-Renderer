#pragma once

/*#include "../ActorBase.h"
#include "../Geometry.h"
#include "../BaseGameObject.h"
#include "../Math.h"

class Sphere : public BaseGameObject<Sphere>
{
public:
	Sphere(
		D3D11::D3D11Core& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material);

	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} MaterialConstants;

private:
	DirectX::XMFLOAT4X4 m_Transform = Math::Identity4x4();
	Geometry			m_GeoGen;
};*/