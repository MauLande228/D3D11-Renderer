#pragma once

#include "WinUtils.h"
#include "D3DUtil.h"

namespace D3D11
{
	class D3D11Core
	{
		friend class Bindable;

#ifdef NDEBUG
		const bool debugLayers = false;
#else
		const bool debugLayers = true;
#endif // NDEBUG

	public:
		D3D11Core(HWND hWnd, int width, int height);
		D3D11Core(const D3D11Core&) = delete;
		D3D11Core& operator=(const D3D11Core&) = delete;
		~D3D11Core();

		void EndFrame();
		void BeginFrame(DirectX::XMVECTORF32 color) noexcept;
		void DrawIndexed(uint32_t count);

		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		void SetCamera(DirectX::FXMMATRIX view) noexcept;

		DirectX::XMMATRIX GetProjection() const noexcept;
		DirectX::XMMATRIX GetView() const noexcept;

		void EnableImgui() noexcept;
		void DisableImgui() noexcept;
		bool IsImguiEnabled() const noexcept;

	private:
		void CreateDeviceAndSwapChain(HWND hWnd, int width, int height);
		void OnResize(int width, int height);

	private:
		bool m_bImguiEnabled = true;

		UINT m_DebugFlags;

		DirectX::XMMATRIX m_Projection;
		DirectX::XMMATRIX m_View;

		Microsoft::WRL::ComPtr<IDXGISwapChain>		m_Swapchain;
		Microsoft::WRL::ComPtr<ID3D11Device>		m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>	m_DeviceContext;

		Microsoft::WRL::ComPtr<ID3D11Resource>			m_BackBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_DSState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_DepthStencil;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;
	};
}

