#pragma once

#include "../Actor.h"
#include "../Geometry.h"
#include "../Camera.h"

struct CameraCB
{
	DirectX::XMFLOAT3 CamPos;
};

class Sphere : public Actor
{
public:
	Sphere(D3D11::D3D11Core& gfx, Camera& camera, float radius);

	void SetPosition(DirectX::XMFLOAT3 pos) noexcept;
	void UpdateCB(Camera& camera) noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
	DirectX::XMFLOAT3	m_Position = { 1.0f, 1.0f, 1.0f };
	Geometry			m_GeoGen;
	CameraCB			m_CameraCB;
};