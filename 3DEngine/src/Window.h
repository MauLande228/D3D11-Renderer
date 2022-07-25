#pragma once

#include "WinUtils.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace D3D11
{
	class D3D11Core;
}

struct WindowExtent
{
	UINT wndWidth;
	UINT wndHeight;
};

class Window
{
private:
	/** Nested class for acces to the private functions */
	class WindowClass
	{
	public:
		WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass(const WindowClass&&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&&) = delete;
		~WindowClass();

	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		static constexpr const wchar_t* m_WndClassName = L"System window";
		static WindowClass				m_WndClass;
		HINSTANCE						m_HInstance;
	};

public:
	Window(const int width, const int height, const wchar_t* wndName);
	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(const Window&&) = delete;
	~Window();

	void SetTitle(const std::wstring& wndTitle);
	std::wstring GetWindowTitle() const noexcept;

	WindowExtent GetWindowExtent() const;
	HWND GetHwnd()const { return m_HWnd; }

	void SetFullScreen(bool fullScreen);

	static std::optional<int> ProcessMessages() noexcept;
	D3D11::D3D11Core& Gfx();

private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int				m_Width;
	int				m_Height;
	HWND			m_HWnd;
	std::wstring	m_WndTitle;
	RECT			m_WindowRect;
	bool			m_Fullscreen;

public:
	std::unique_ptr<D3D11::D3D11Core> m_D3D11App;

	Keyboard	m_KeyBoard;
	Mouse		m_Mouse;
};
