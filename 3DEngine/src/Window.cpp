#include "Window.h"

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

	//m_D3D12App = std::make_unique<D3D12::D3D12Core>(m_HWnd, wr.right - wr.left, wr.bottom - wr.top);

	ShowWindow(m_HWnd, SW_SHOWDEFAULT);

}

Window::~Window()
{
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

void Window::SetFullScreen(bool fullscreen)
{
	if (m_Fullscreen != fullscreen)
	{
		m_Fullscreen = fullscreen;

		if (m_Fullscreen) // Switching to fullscreen.
		{
			// Store the current window dimensions so they can be restored 
			// when switching out of fullscreen state.
			::GetWindowRect(m_HWnd, &m_WindowRect);

			// Set the window style to a borderless window so the client area fills
			// the entire screen.
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

			::SetWindowLongW(m_HWnd, GWL_STYLE, windowStyle);

			// Query the name of the nearest display device for the window.
			// This is required to set the fullscreen dimensions of the window
			// when using a multi-monitor setup.
			HMONITOR hMonitor = ::MonitorFromWindow(m_HWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMonitor, &monitorInfo);

			::SetWindowPos(m_HWnd, HWND_TOPMOST,
				monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(m_HWnd, SW_MAXIMIZE);
		}
		else
		{
			// Restore all the window decorators.
			::SetWindowLong(m_HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			::SetWindowPos(m_HWnd, HWND_NOTOPMOST,
				m_WindowRect.left,
				m_WindowRect.top,
				m_WindowRect.right - m_WindowRect.left,
				m_WindowRect.bottom - m_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(m_HWnd, SW_NORMAL);
		}
	}
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

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS:
		m_KeyBoard.ClearState();
		break;

	case WM_SIZE:
	{
		RECT clientRect = {};
		::GetClientRect(m_HWnd, &clientRect);

		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		//m_D3D12App->Resize(width, height);
	}
	break;

	/*KEYBOARD MESSAGES*/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || m_KeyBoard.AutorepeatIsEnabled())
		{
			m_KeyBoard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_KeyBoard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:

		m_KeyBoard.OnChar(static_cast<unsigned char>(wParam));
		break;
		/*END KEYBOARD MESSAGES*/

		/*MOUSE MESSAGES*/
	case WM_MOUSEMOVE:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
		{
			m_Mouse.OnMouseMove(pt.x, pt.y);
			if (!m_Mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_Mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				m_Mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
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

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height)
		{
			ReleaseCapture();
			m_Mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height)
		{
			ReleaseCapture();
			m_Mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_Mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/*END MOUSE MESSAGES*/
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}