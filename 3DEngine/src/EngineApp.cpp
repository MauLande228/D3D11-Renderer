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
		m_CBuffer(m_Window.Gfx())
	{
		m_Window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 500.0f));
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
				Draw(m_Timer.DeltaTime());
				ProcessInput(m_Timer.DeltaTime());
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

	void EngineApp::Draw(float dt)
	{
		m_Window.Gfx().BeginFrame(DirectX::Colors::Black);
		m_Window.Gfx().SetCamera(m_Camera.GetMatrix());
		BindSceneLights();

		//nano.Draw(m_Window.Gfx());
		//gobber.Draw(m_Window.Gfx());
		//Sponza.Draw(m_Window.Gfx());
		sphere.Draw(m_Window.Gfx());

		for (unsigned int i = 0; i < 2; i++)
		{
			m_Lights[i].Draw(m_Window.Gfx());
		}

		//nano.ShowWindow("Nano suit");
		//gobber.ShowWindow("Gobber");
		//Sponza.ShowWindow("Sponza");
		m_Camera.SpawnControlWindow();
		
		m_Lights[0].SpawnControlWindow("Light 1");
		m_Lights[1].SpawnControlWindow("Light 2");

		m_Window.Gfx().EndFrame();
	}

	void EngineApp::ProcessInput(float dt)
	{
		while (const auto e = m_Window.m_KeyBoard.ReadKey())
		{
			if (e->IsPress() && e->GetCode() == VK_ESCAPE)
			{
				if (m_Window.CursorEnabled())
				{
					m_Window.DisableCursor();
					m_Window.m_Mouse.EnableRaw();
				}
				else
				{
					m_Window.EnableCursor();
					m_Window.m_Mouse.DisableRaw();
				}
			}
		}
		if (!m_Window.CursorEnabled())
		{
			if (m_Window.m_KeyBoard.KeyIsPressed('W'))
			{
				m_Camera.Translate({ 0.0f, 0.0f, dt });
			}
			if (m_Window.m_KeyBoard.KeyIsPressed('A'))
			{
				m_Camera.Translate({ -dt, 0.0f, 0.0f });
			}
			if (m_Window.m_KeyBoard.KeyIsPressed('S'))
			{
				m_Camera.Translate({ 0.0f, 0.0f, -dt });
			}
			if (m_Window.m_KeyBoard.KeyIsPressed('D'))
			{
				m_Camera.Translate({ dt, 0.0f, 0.0f });
			}
			if (m_Window.m_KeyBoard.KeyIsPressed('E'))
			{
				m_Camera.Translate({ 0.0f, dt, 0.0f });
			}
			if (m_Window.m_KeyBoard.KeyIsPressed('Q'))
			{
				m_Camera.Translate({ 0.0f, -dt, 0.0f });
			}
		}

		while (const auto delta = m_Window.m_Mouse.ReadRawDelta())
		{
			if (!m_Window.CursorEnabled())
			{
				m_Camera.Rotate((float)delta->x, (float)delta->y);
			}
		}
	}

	void EngineApp::BindSceneLights()
	{
		for (unsigned int i = 0; i < 2; i++)
		{
			auto data = m_Lights[i].GetData();
			auto poscopy = m_Lights[i].GetData().Pos;
			const auto pos = DirectX::XMLoadFloat3(&poscopy);
			DirectX::XMStoreFloat3(&data.Pos, DirectX::XMVector3Transform(pos, m_Camera.GetMatrix()));
			PL.lights[i] = data;
		}

		m_CBuffer.Update(m_Window.Gfx(), PL);
		m_CBuffer.Bind(m_Window.Gfx());
	}
}
