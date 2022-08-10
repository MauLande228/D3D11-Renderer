#pragma once

#include "Bindable.h"

#include <array>

namespace D3D11
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(D3D11Core& gfx, bool twoSided);

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<Rasterizer> Resolve(D3D11Core& gfx, bool twosided);
		static std::string GenerateUID(bool twoSided);
		std::string GetUID() const noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_Rasterizer;
		bool m_bTwoSided;
	};
}