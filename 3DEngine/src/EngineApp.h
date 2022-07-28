#include "Window.h"
#include "Timer.h"
#include "Primitives/Box.h"
#include "Primitives/Sphere.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "Actor.h"
#include "Camera.h"
#include "PointLight.h"

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

		void Draw();

	private:
		ImGuiManager m_ImGui{};

		Window	m_Window{ 800, 600, L"D3D12 Renderer" };
		Camera	m_Camera{};
		Timer	m_Timer{};

		PointLight m_PointLight;

		std::vector<std::unique_ptr<Actor>> m_Actors;
		static constexpr size_t nActors = 80;
	};
}