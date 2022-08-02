#include "InputLayout.h"
#include "ResourcePool.h"

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

D3D11::InputLayout::InputLayout(D3D11Core& gfx, VertexLayout layout, ID3DBlob* pVertexShaderByteCode) :
	m_Layout(std::move(layout))
{
	const auto d3dLayout = m_Layout.GetD3DLayout();

	GFX_EXC(GetDevice(gfx)->CreateInputLayout(
		d3dLayout.data(),
		(UINT)d3dLayout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_InputLayout));
}

void D3D11::InputLayout::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(m_InputLayout.Get());
}

std::shared_ptr<D3D11::InputLayout> D3D11::InputLayout::Resolve(
	D3D11Core& gfx,
	const VertexLayout& layout,
	ID3DBlob* pVertexShaderByteCode)
{
	return Pool::Resolve<D3D11::InputLayout>(gfx, layout, pVertexShaderByteCode);
}

std::string D3D11::InputLayout::GenerateUID(const VertexLayout& layout, ID3DBlob* pVertexShaderByteCode)
{
	using namespace std::string_literals;
	return typeid(D3D11::InputLayout).name() + "#"s + layout.GetCode();
}

std::string D3D11::InputLayout::GetUID() const noexcept
{
	return GenerateUID(m_Layout);
}
