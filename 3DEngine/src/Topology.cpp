#include "Topology.h"

D3D11::Topology::Topology(D3D11Core& gfx, D3D11_PRIMITIVE_TOPOLOGY topology) :
	m_Topology(topology)
{
}

void D3D11::Topology::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(m_Topology);
}
