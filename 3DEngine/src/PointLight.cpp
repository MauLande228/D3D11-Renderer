#include "PointLight.h"
#include "../imgui/imgui.h"

PointLight::PointLight(D3D11::D3D11Core& gfx, float radius) :
	m_Mesh(gfx, radius),
	m_CBuffer(gfx)
{
	Reset();
}

void PointLight::Draw(D3D11::D3D11Core& gfx) const NOXND
{
	m_Mesh.SetPosition(m_Data.Pos);
	m_Mesh.Draw(gfx);
}

void PointLight::Bind(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = m_Data;
	const auto pos = DirectX::XMLoadFloat3(&m_Data.Pos);
	DirectX::XMStoreFloat3(&dataCopy.Pos, DirectX::XMVector3Transform(pos, view));
	m_CBuffer.Update(gfx, dataCopy);
	m_CBuffer.Bind(gfx);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_Data.Pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_Data.Pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_Data.Pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &m_Data.DiffuseIntensity, 0.01f, 2.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		ImGui::ColorEdit3("Diffuse Color", &m_Data.DiffuseColor.x);
		ImGui::ColorEdit3("Ambient", &m_Data.Ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &m_Data.AttConst, 0.05f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat("Linear", &m_Data.AttLin, 0.0001f, 4.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat("Quadratic", &m_Data.AttQuad, 0.0000001f, 10.0f, "%.7f", ImGuiSliderFlags_Logarithmic);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	m_Data = {
		{ 0.0f,0.0f,0.0f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}
