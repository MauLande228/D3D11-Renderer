#include "IndexBuffer.h"
#include "ResourcePool.h"

D3D11::IndexBuffer::IndexBuffer(D3D11Core& gfx, const std::vector<uint16_t>& indices) :
	IndexBuffer(gfx, "?", indices)
{
}

D3D11::IndexBuffer::IndexBuffer(D3D11Core& gfx, std::string tag, const std::vector<uint16_t>& indices) :
	m_Tag(tag),
	m_Count((UINT)indices.size())
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

std::shared_ptr<D3D11::IndexBuffer> D3D11::IndexBuffer::Resolve(
	D3D11Core& gfx,
	const std::string& tag,
	const std::vector<uint16_t>& indices)
{
	assert(tag != "?");
	return Pool::Resolve<D3D11::IndexBuffer>(gfx, tag, indices);
}

std::string D3D11::IndexBuffer::GetUID() const noexcept
{
	return GenerateUID_(m_Tag);
}

std::string D3D11::IndexBuffer::GenerateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(D3D11::IndexBuffer).name() + "#"s + tag;
}
