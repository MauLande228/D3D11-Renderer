#pragma once

#include "D3D11Core.h"

class Camera
{
public:
	Camera() noexcept;

	DirectX::XMMATRIX GetMatrix() const noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPosition() const noexcept;

	void SpawnControlWindow() noexcept;
	void Reset() noexcept;


private:
	DirectX::XMFLOAT3 m_Pos;
	float m_Pitch;
	float m_Yaw;
	static constexpr float m_TravelSpeed = 18.0f;
	static constexpr float m_RotationSpeed = 0.004f;
};