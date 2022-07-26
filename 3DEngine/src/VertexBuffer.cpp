#include "VertexBuffer.h"

void D3D11::VertexBuffer::Bind(D3D11Core& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &m_Stride, &offset);
}
