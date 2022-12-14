#include "Window.h"
#include "D3D11Core.h"
#include "../imgui/imgui_impl_win32.h"

#include <cassert>

Window::WindowClass Window::WindowClass::m_WndClass;

Window::WindowClass::WindowClass()
	:m_HInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(m_WndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return m_WndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return m_WndClass.m_HInstance;
}

Window::Window(const int width, const int height, const wchar_t* wndName)
	: m_Width(width), m_Height(height), m_HWnd(nullptr), m_WndTitle(wndName)
{
	RECT wr{};
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	m_WindowRect = wr;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw std::runtime_error("Could not adjust window rect!");
	}

	m_HWnd = ::CreateWindow(
		WindowClass::GetName(),
		wndName,
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		WindowClass::GetInstance(),
		this);


	ShowWindow(m_HWnd, SW_SHOWDEFAULT);
	ImGui_ImplWin32_Init(m_HWnd);
	m_D3D11App = std::make_unique<D3D11::D3D11Core>(m_HWnd, width, height);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; //Mouse page
	rid.usUsage = 0x02; // Mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1u, sizeof(rid));
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_HWnd);
}

void Window::SetTitle(const std::wstring& wndTitle)
{
	SetWindowText(m_HWnd, wndTitle.c_str());
}

std::wstring Window::GetWindowTitle() const noexcept
{
	return m_WndTitle;
}

WindowExtent Window::GetWindowExtent() const
{
	WindowExtent we{};
	we.wndWidth = m_Width;
	we.wndHeight = m_Height;

	return we;
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return 0;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

D3D11::D3D11Core& Window::Gfx()
{
	assert(m_D3D11App && "There must be a D3D11Core instance");

	return *m_D3D11App;
}

void Window::EnableCursor()
{
	m_CursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void Window::DisableCursor()
{
	m_CursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

bool Window::CursorEnabled() const noexcept
{
	return m_CursorEnabled;
}

void Window::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

void Window::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect(m_HWnd, &rect);
	MapWindowPoints(m_HWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

LRESULT __stdcall Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsg));
		return pWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT __stdcall Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	const auto io = ImGui::GetIO();

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!m_CursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;

		/******************KEY MESSAGES*******************/
	case WM_KILLFOCUS:
		m_KeyBoard.ClearState();
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (io.WantCaptureKeyboard)
		{
			break;
		}

		if (!(lParam & 0x40000000) || m_KeyBoard.AutorepeatIsEnabled())
		{
			m_KeyBoard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (io.WantCaptureKeyboard)
		{
			break;
		}
		m_KeyBoard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		if (io.WantCaptureKeyboard)
		{
			break;
		}
		m_KeyBoard.OnChar(static_cast<unsigned char>(wParam));
		break;

		/******************MOUSE MESSAGES*******************/
	case WM_MOUSEMOVE:
	{
		if (!m_CursorEnabled)
		{
			if (!m_Mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_Mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}

		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
		{
			m_Mouse.OnMouseMove(pt.x, pt.y);
			if (!m_Mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_Mouse.OnMouseEnter();
			}
		}

		else
		{	//Anding the wParam and the button defs we can know if either of them is down
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				m_Mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				m_Mouse.OnMouseLeave();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!m_CursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}

		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnMouseMove(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightPressed(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightReleased(pt.x, pt.y);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if (io.WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_Mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/*END MOUSE MESSAGES*/

	/******************* RAW MOUSE MESSAGES *******************/
	case WM_INPUT:
	{
		if (!m_Mouse.RawEnabled())
		{
			break;
		}
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}
		m_RawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			m_RawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			// bail msg processing if error
			break;
		}
		// process the raw input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			m_Mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
