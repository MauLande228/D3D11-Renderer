#pragma once

#include "D3D11Core.h"
#include "Primitives/LightBulb.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(D3D11::D3D11Core& gfx, float radius = 0.5f);

	
	void Draw(D3D11::D3D11Core& gfx) const noexcept(!_DEBUG);
	void Bind(D3D11::D3D11Core& gfx) const noexcept;

	void SpawnControlWindow() noexcept;
	void Reset() noexcept;

private:
	struct PointLightCB
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};

private:
	DirectX::XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
	mutable LightBulb m_Mesh;
	mutable D3D11::PixelConstantBuffer<PointLightCB> m_CBuffer;
};