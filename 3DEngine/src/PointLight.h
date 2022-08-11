#pragma once

#include "D3D11Core.h"
#include "Primitives/LightBulb.h"
#include "ConstantBuffers.h"

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

class PointLight
{
public:
	PointLight(D3D11::D3D11Core& gfx, float radius = 0.5f);

	
	void Draw(D3D11::D3D11Core& gfx) const NOXND;
	void Bind(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX view) const noexcept;

	PointLightCB GetData() const noexcept;

	void SpawnControlWindow(const char* wndName) noexcept;
	void Reset() noexcept;

private:
	PointLightCB m_Data;
	mutable LightBulb m_Mesh;
	mutable D3D11::PixelConstantBuffer<PointLightCB> m_CBuffer;
};