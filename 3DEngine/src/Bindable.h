#pragma once

#include "D3D11Core.h"

namespace D3D11
{
	class Bindable
	{
	public:
		virtual void Bind(D3D11::D3D11Core& gfx) noexcept = 0;
		virtual ~Bindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext(D3D11::D3D11Core& gfx) noexcept;
		static ID3D11Device* GetDevice(D3D11::D3D11Core& gfx) noexcept;
	};
}

