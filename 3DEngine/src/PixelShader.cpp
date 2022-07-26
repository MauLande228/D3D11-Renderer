#include "PixelShader.h"

D3D11::PixelShader::PixelShader(D3D11Core& gfx, const std::wstring& filePath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_EXC(D3DReadFileToBlob(filePath.c_str(), &pBlob));
	GFX_EXC(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_PixelShader));
}

void D3D11::PixelShader::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
}
