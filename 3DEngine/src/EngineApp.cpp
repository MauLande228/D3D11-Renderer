#include "EngineApp.h"

namespace Engine
{
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
		//m_Window.Gfx().Render();
	}
}
