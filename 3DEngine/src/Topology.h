#pragma once

#include "Bindable.h"

namespace D3D11
{
	class Topology : public Bindable
	{
	public:
		Topology(D3D11Core& gfx, D3D11_PRIMITIVE_TOPOLOGY topology);

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<Topology> Resolve(D3D11Core& gfx, D3D11_PRIMITIVE_TOPOLOGY topology);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology);
		std::string GetUID() const noexcept override;

	protected:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}

