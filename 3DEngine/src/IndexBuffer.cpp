#include "IndexBuffer.h"

D3D11::IndexBuffer::IndexBuffer(D3D11Core& gfx, const std::vector<uint16_t>& indices) :
	m_Count((uint32_t)indices.size())
{
	D3D11_BUFFER_DESC iBufferDesc{};
	iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;
	iBufferDesc.ByteWidth = UINT(m_Count * sizeof(uint16_t));
	iBufferDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA subresourceData{};
	subresourceData.pSysMem = indices.data();
	GFX_EXC(GetDevice(gfx)->CreateBuffer(&iBufferDesc, &subresourceData, &m_IndexBuffer));
}

void D3D11::IndexBuffer::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

uint32_t D3D11::IndexBuffer::GetCount() const noexcept
{
	return m_Count;
}
