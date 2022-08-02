#include "PixelShader.h"
#include "ResourcePool.h"

D3D11::PixelShader::PixelShader(D3D11Core& gfx, const std::string& filePath) :
	m_FilePath(filePath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_EXC(D3DReadFileToBlob(std::wstring{filePath.begin(), filePath.end()}.c_str(), &pBlob));
	GFX_EXC(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_PixelShader));
}

void D3D11::PixelShader::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
}

std::shared_ptr<D3D11::PixelShader> D3D11::PixelShader::Resolve(D3D11Core& gfx, const std::string& filePath)
{
	return Pool::Resolve<D3D11::PixelShader>(gfx, filePath);
}

std::string D3D11::PixelShader::GenerateUID(const std::string& filePath)
{
	using namespace std::string_literals;
	return typeid(D3D11::PixelShader).name() + "#"s + filePath;
}

std::string D3D11::PixelShader::GetUID() const noexcept
{
	return GenerateUID(m_FilePath);
}
