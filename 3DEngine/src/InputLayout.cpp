#include "InputLayout.h"

D3D11::InputLayout::InputLayout(
	D3D11Core& gfx,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* pVertexShaderByteCode)
{
	GFX_EXC(GetDevice(gfx)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_InputLayout));
}

void D3D11::InputLayout::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(m_InputLayout.Get());
}
