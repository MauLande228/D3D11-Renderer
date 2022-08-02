#include "VertexShader.h"
#include "ResourcePool.h"

#include <typeinfo>

D3D11::VertexShader::VertexShader(D3D11Core& gfx, const std::string& filePath) :
	m_FilePath(filePath)
{
	GFX_EXC(D3DReadFileToBlob(std::wstring{filePath.begin(), filePath.end()}.c_str(), &m_ByteCode));
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

std::shared_ptr<D3D11::VertexShader> D3D11::VertexShader::Resolve(D3D11Core& gfx, const std::string& filePath)
{
	return Pool::Resolve<D3D11::VertexShader>(gfx, filePath);
}

std::string D3D11::VertexShader::GenerateUID(const std::string& filePath)
{
	using namespace std::string_literals;
	return typeid(D3D11::VertexShader).name() + "#"s + filePath;
}

std::string D3D11::VertexShader::GetUID() const noexcept
{
	return GenerateUID(m_FilePath);
}
