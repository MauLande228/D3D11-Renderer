#include "EngineApp.h"
#include "D3D11Core.h"

namespace Engine
{
	EngineApp::EngineApp()
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
		for (auto i = 0; i < 50; i++)
		{
			m_Boxes.push_back(std::make_unique<Box>(
				m_Window.Gfx(), rng, adist,
				ddist, odist, rdist
				));
		}
		m_Window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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

		for (auto& b : m_Boxes)
		{
			b->Update(m_Timer.DeltaTime());
			b->Draw(m_Window.Gfx());
		}

		m_Window.Gfx().EndFrame();
	}
}
