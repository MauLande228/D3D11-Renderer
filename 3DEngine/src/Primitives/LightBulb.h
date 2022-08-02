#pragma once

#include "../Actor.h"
#include "../Geometry.h"

class LightBulb : public Actor
{
public:
	LightBulb(D3D11::D3D11Core& gfx, float radius);

	void SetPosition(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
	DirectX::XMFLOAT3	m_Position = { 1.0f, 1.0f, 1.0f };
	Geometry			m_GeoGen;
};