#include "VertexBuffer.h"
#include "ResourcePool.h"

void D3D11::VertexBuffer::Bind(D3D11Core& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &m_Stride, &offset);
}

std::shared_ptr<D3D11::VertexBuffer> D3D11::VertexBuffer::Resolve(
	D3D11Core& gfx,
	const std::string& tag,
	const VertexResource& vbuf)
{
	assert(tag != "?");
	return Pool::Resolve<D3D11::VertexBuffer>(gfx, tag, vbuf);
}

std::string D3D11::VertexBuffer::GetUID() const noexcept
{
	return GenerateUID(m_Tag);
}

std::string D3D11::VertexBuffer::GenerateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(D3D11::VertexBuffer).name() + "#"s + tag;
}
