#include "EngineApp.h"
#include "D3D11Core.h"
#include "stb_image.h"
#include "../imgui/imgui.h"

#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
	EngineApp::EngineApp() :
		m_PointLight(m_Window.Gfx())
	{
		m_Window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 50.0f));
	}

	EngineApp::~EngineApp()
	{
	}

	int EngineApp::Run()
	{
		m_Timer.Reset();
		while (true)
		{
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}

			m_Timer.Tick();
			if (!m_Timer.IsStopped())
			{
				CalculateFrameStats();
				Draw();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	void EngineApp::CalculateFrameStats()
	{
		static int g_FrameCount = 0;
		static float timeElapsed = 0.0f;

		g_FrameCount++;

		if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
		{
			const auto fps = (float)g_FrameCount;
			const float mspf = 1000.0f / fps;

			const std::wstring fpsStr = std::to_wstring(fps);
			const std::wstring mspfStr = std::to_wstring(mspf);

			const std::wstring windowText = m_Window.GetWindowTitle() +
				L"      fps: " + fpsStr +
				L"   ms: " + mspfStr;

			m_Window.SetTitle(windowText);

			g_FrameCount = 0;
			timeElapsed += 1.0f;
		}
	}

	void EngineApp::Draw()
	{
		m_Window.Gfx().BeginFrame(DirectX::Colors::Black);
		m_Window.Gfx().SetCamera(m_Camera.GetMatrix());
		m_PointLight.Bind(m_Window.Gfx(), m_Camera.GetMatrix());

		
		const auto transform = DirectX::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
			DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		nano.Draw(m_Window.Gfx(), transform);

		m_PointLight.Draw(m_Window.Gfx());

		m_Camera.SpawnControlWindow();
		m_PointLight.SpawnControlWindow();
		ShowModelWindow();

		m_Window.Gfx().EndFrame();
	}

	void EngineApp::ShowModelWindow()
	{
		if (ImGui::Begin("Model"))
		{
			using namespace std::string_literals;

			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
			ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
			ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
		}
		ImGui::End();
	}
}
