#include "Camera.h"
#include "Math.h"
#include "../imgui/imgui.h"

#include <algorithm>

Camera::Camera() noexcept
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	using namespace DirectX;
	
	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	const auto lookVector = XMVector3Transform(
		forwardBaseVector,
		XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f));

	const auto camPosition = XMLoadFloat3(&m_Pos);
	const auto camTarget = camPosition + lookVector;

	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::Rotate(float dx, float dy) noexcept
{
	m_Yaw = Math::WrapAngle(m_Yaw + dx * m_RotationSpeed);
	m_Pitch = std::clamp(m_Pitch + dy * m_RotationSpeed, 0.995f * -Math::PI / 2.0f, 0.995f * Math::PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	DirectX::XMStoreFloat3(
		&translation,
		DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&translation),
			DirectX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f) *
			DirectX::XMMatrixScaling(m_TravelSpeed, m_TravelSpeed, m_TravelSpeed)));

	m_Pos = {
		m_Pos.x + translation.x,
		m_Pos.y + translation.y,
		m_Pos.z + translation.z
	};
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_Pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_Pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_Pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &m_Pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderAngle("Yaw", &m_Yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	m_Pos = { 0.0f, 7.5f, -18.0f };
	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
}
