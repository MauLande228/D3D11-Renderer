#pragma once

#include "Bindable.h"

namespace D3D11 
{
	class Texture : public Bindable
	{
	public:
		Texture(D3D11Core& gfx, const std::string& filePath, unsigned int slot = 0);

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_TextureView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_Texture;
		unsigned int m_Slot;
	};
}