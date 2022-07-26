#include "Bindable.h"

namespace D3D11
{
	ID3D11DeviceContext* Bindable::GetContext(D3D11::D3D11Core& gfx) noexcept
	{
		return gfx.m_DeviceContext.Get();
	}

	ID3D11Device* Bindable::GetDevice(D3D11::D3D11Core& gfx) noexcept
	{
		return gfx.m_Device.Get();
	}
}
