#include "Window.h"
#include "Timer.h"
#include "Primitives/Box.h"
#include "Primitives/Sphere.h"
#include "ImGuiManager.h"
#include "Actor.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
#include "Primitives/Plane.h"

constexpr float PI = 3.14159265f;

namespace Engine
{
	class EngineApp
	{
		friend class Window;

	public:
		Window& GetWindow() { return m_Window; }
		int GetFrameCount() const { return 1; }

		int Run();
		void CalculateFrameStats();

	public:
		EngineApp();
		EngineApp(const EngineApp&) = delete;
		EngineApp(const EngineApp&&) = delete;
		EngineApp& operator=(const EngineApp&) = delete;
		EngineApp& operator=(const EngineApp&&) = delete;
		~EngineApp();

		void Draw(float dt);
		void ProcessInput(float dt);
		void BindSceneLights();

	private:
		ImGuiManager m_ImGui{};

		Window	m_Window{ 1280, 720, L"D3D11 Renderer" };
		Camera	m_Camera{};
		Timer	m_Timer{};
		
		PointLight m_Lights[2] = { PointLight{m_Window.Gfx()}, PointLight{m_Window.Gfx()} };

		//Model nano{ m_Window.Gfx(), "models/nano_textured/nanosuit.obj", 2.0f };
		//Model gobber{ m_Window.Gfx(), "models/gobber/GoblinX.obj", 6.0f };
		Model Sponza{ m_Window.Gfx(), "models/Sponza/sponza.obj", 1.0f / 20.0f };

		struct PointLights
		{
			PointLightCB lights[2];
		}PL;

		mutable D3D11::PixelConstantBuffer<PointLights> m_CBuffer;
	};
}