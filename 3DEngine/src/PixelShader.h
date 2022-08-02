#pragma once

#include "Bindable.h"

namespace D3D11
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(D3D11Core& gfx, const std::string& filePath);

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(D3D11Core& gfx, const std::string& filePath);
		static std::string GenerateUID(const std::string& filePath);
		std::string GetUID() const noexcept override;

	protected:
		std::string m_FilePath;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}

