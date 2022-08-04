#pragma once

#include "Bindable.h"

namespace D3D11 
{
	class Texture : public Bindable
	{
	public:
		Texture(D3D11Core& gfx, const std::string& filePath, unsigned int slot = 0);

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<Texture> Resolve(D3D11Core& gfx, const std::string& filePath, UINT slot = 0);
		static std::string GenerateUID(const std::string& filePath, UINT slot = 0);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;

	protected:
		std::string m_FilePath;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_TextureView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_Texture;
		unsigned int m_Slot;
		bool m_HasAlpha;
	};
}