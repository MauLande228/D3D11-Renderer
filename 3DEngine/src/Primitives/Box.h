#pragma once

#include "../ActorBase.h"
#include "../Geometry.h"

#include <random>

class Box : public ActorBase<Box>
{
public:
	Box(D3D11::D3D11Core& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material);
	
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	Geometry m_GeoGen;
};