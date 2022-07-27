#include "D3D11Core.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

namespace D3D11
{
	D3D11Core::D3D11Core(HWND hWnd, int width, int height)
	{
		CreateDeviceAndSwapChain(hWnd, width, height);
		OnResize(width, height);
		ImGui_ImplDX11_Init(m_Device.Get(), m_DeviceContext.Get());
	}

	D3D11Core::~D3D11Core()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void D3D11Core::EndFrame()
	{
		if (m_bImguiEnabled)
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		GFX_EXC(m_Swapchain->Present(1u, 0u));
	}

	void D3D11Core::BeginFrame(DirectX::XMVECTORF32 color) noexcept
	{
		if (m_bImguiEnabled)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void D3D11Core::DrawIndexed(uint32_t count)
	{
		m_DeviceContext->DrawIndexed(count, 0u, 0u);
	}

	void D3D11Core::SetProjection(DirectX::FXMMATRIX proj) noexcept
	{
		m_Projection = proj;
	}

	void D3D11Core::SetCamera(DirectX::FXMMATRIX view) noexcept
	{
		m_View = view;
	}

	DirectX::XMMATRIX D3D11Core::GetProjection() const noexcept
	{
		return m_Projection;
	}

	DirectX::XMMATRIX D3D11Core::GetView() const noexcept
	{
		return m_View;
	}

	void D3D11Core::EnableImgui() noexcept
	{
		m_bImguiEnabled = true;
	}

	void D3D11Core::DisableImgui() noexcept
	{
		m_bImguiEnabled = false;
	}

	bool D3D11Core::IsImguiEnabled() const noexcept
	{
		return m_bImguiEnabled;
	}

	void D3D11Core::CreateDeviceAndSwapChain(HWND hWnd, int width, int height)
	{
		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2; // Double buffering
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		if (debugLayers)
		{
			m_DebugFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		GFX_EXC(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			m_DebugFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_Swapchain,
			&m_Device,
			nullptr,
			&m_DeviceContext));
	}

	void D3D11Core::OnResize(int width, int height)
	{
		GFX_EXC(m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&m_BackBuffer)));
		GFX_EXC(m_Device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, &m_RenderTargetView));

		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		GFX_EXC(m_Device->CreateDepthStencilState(&dsDesc, &m_DSState));

		m_DeviceContext->OMSetDepthStencilState(m_DSState.Get(), 1u);

		D3D11_TEXTURE2D_DESC depthDesc{};
		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1u;
		depthDesc.ArraySize = 1u;
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.SampleDesc.Count = 1u;
		depthDesc.SampleDesc.Quality = 0u;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GFX_EXC(m_Device->CreateTexture2D(&depthDesc, nullptr, &m_DepthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0u;
		GFX_EXC(m_Device->CreateDepthStencilView(m_DepthStencil.Get(), &dsvDesc, &m_DepthStencilView));

		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		D3D11_VIEWPORT vp{};
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		m_DeviceContext->RSSetViewports(1u, &vp);
	}
}
