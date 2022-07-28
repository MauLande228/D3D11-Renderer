#pragma once

#include "ActorBase.h"
#include "Math.h"

#include <random>

template<class T>
class BaseGameObject : public ActorBase<T>
{
public:
	BaseGameObject(
		D3D11::D3D11Core& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist) :
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{}

	void Update(float dt) noexcept
	{
		roll = Math::WrapAngle(roll + droll * dt);
		pitch = Math::WrapAngle(pitch + dpitch * dt);
		yaw = Math::WrapAngle(yaw + dyaw * dt);
		theta = Math::WrapAngle(theta + dtheta * dt);
		phi = Math::WrapAngle(phi + dphi * dt);
		chi = Math::WrapAngle(chi + dchi * dt);
	}
	DirectX::XMMATRIX GetTransform() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}

protected:
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
};