#pragma once

#include "BaseGameObject.h"
#include "ConstantBuffers.h"

class Model : public BaseGameObject<Model>
{
public:
	Model(
		D3D11::D3D11Core& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		float scale);
};