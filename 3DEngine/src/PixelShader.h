#pragma once

#include "Bindable.h"

namespace D3D11
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(D3D11Core& gfx, const std::wstring& filePath);

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}

