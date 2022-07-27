#include "PointLight.h"
#include "../imgui/imgui.h"

PointLight::PointLight(D3D11::D3D11Core& gfx, float radius) :
	m_Mesh(gfx, radius),
	m_CBuffer(gfx)
{
}

void PointLight::Draw(D3D11::D3D11Core& gfx) const noexcept(!_DEBUG)
{
	m_Mesh.SetPosition(m_Position);
	m_Mesh.Draw(gfx);
}

void PointLight::Bind(D3D11::D3D11Core& gfx) const noexcept
{
	m_CBuffer.Update(gfx, PointLightCB{ m_Position });
	m_CBuffer.Bind(gfx);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_Position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_Position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_Position.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	m_Position = { 0.0f,0.0f,0.0f };
}
