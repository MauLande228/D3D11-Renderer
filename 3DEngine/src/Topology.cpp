#include "Topology.h"
#include "ResourcePool.h"

D3D11::Topology::Topology(D3D11Core& gfx, D3D11_PRIMITIVE_TOPOLOGY topology) :
	m_Topology(topology)
{
}

void D3D11::Topology::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(m_Topology);
}

std::shared_ptr<D3D11::Topology> D3D11::Topology::Resolve(D3D11Core& gfx, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	return Pool::Resolve<Topology>(gfx, topology);
}

std::string D3D11::Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(topology);
}

std::string D3D11::Topology::GetUID() const noexcept
{
	return GenerateUID(m_Topology);
}
