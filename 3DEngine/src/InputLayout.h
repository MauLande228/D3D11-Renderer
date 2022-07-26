#pragma once

#include "Bindable.h"

#include <vector>

namespace D3D11
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(
			D3D11Core& gfx,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShaderByteCode);

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}

