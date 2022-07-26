#include "VertexShader.h"

D3D11::VertexShader::VertexShader(D3D11Core& gfx, const std::wstring& filePath)
{
	GFX_EXC(D3DReadFileToBlob(filePath.c_str(), &m_ByteCode));
	GFX_EXC(GetDevice(gfx)->CreateVertexShader(
		m_ByteCode->GetBufferPointer(),
		m_ByteCode->GetBufferSize(),
		nullptr,
		&m_VertexShader));
}

void D3D11::VertexShader::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
}

ID3DBlob* D3D11::VertexShader::GetByteCode() const noexcept
{
	return m_ByteCode.Get();
}
