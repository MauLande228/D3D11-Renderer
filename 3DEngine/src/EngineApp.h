#include "Window.h"
#include "Timer.h"
#include "Primitives/Box.h"
#include "Primitives/Sphere.h"
#include "ImGuiManager.h"
#include "Actor.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"

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
		void ShowModelWindow();

	private:
		ImGuiManager m_ImGui{};

		Window	m_Window{ 1280, 720, L"D3D12 Renderer" };
		Camera	m_Camera{};
		Timer	m_Timer{};

		PointLight m_PointLight;

		Model nano{ m_Window.Gfx(), "models/nano_textured/nanosuit.obj" };
		Model nano2{ m_Window.Gfx(), "models/nano_textured/nanosuit.obj" };

		struct
		{
			float roll = 0.0f;
			float pitch = 0.0f;
			float yaw = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		} pos;
	};
}