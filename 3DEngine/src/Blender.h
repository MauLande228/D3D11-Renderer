#pragma once

#include "Bindable.h"

#include <array>
#include <optional>

namespace D3D11
{
	class Blender : public Bindable
	{
	public:
		Blender(D3D11Core& gfx, bool blending, std::optional<float> factors = {});
		
		void Bind(D3D11Core& gfx) noexcept override;
		void SetFactor(float factor) NOXND;
		float GetFactor() const NOXND;

		static std::shared_ptr<Blender> Resolve(D3D11Core& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);
		std::string GetUID() const noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;
		std::optional<std::array<float, 4>> m_Factors;
		bool m_bBlending;
	};
}
