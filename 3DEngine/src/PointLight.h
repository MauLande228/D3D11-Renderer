#pragma once

#include "D3D11Core.h"
#include "Primitives/LightBulb.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(D3D11::D3D11Core& gfx, float radius = 0.5f);

	
	void Draw(D3D11::D3D11Core& gfx) const noexcept(!_DEBUG);
	void Bind(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX view) const noexcept;

	void SpawnControlWindow() noexcept;
	void Reset() noexcept;

private:
	struct PointLightCB
	{
		alignas(16)DirectX::XMFLOAT3 Pos;
		alignas(16)DirectX::XMFLOAT3 Ambient;
		alignas(16)DirectX::XMFLOAT3 DiffuseColor;
		float DiffuseIntensity;
		float AttConst;
		float AttLin;
		float AttQuad;
	};

private:
	PointLightCB m_Data;
	mutable LightBulb m_Mesh;
	mutable D3D11::PixelConstantBuffer<PointLightCB> m_CBuffer;
};