#pragma once

#include "Bindable.h"

namespace D3D11
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(D3D11Core& gfx, const std::wstring& filePath);

		void Bind(D3D11Core& gfx) noexcept override;
		ID3DBlob* GetByteCode() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob>			m_ByteCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_VertexShader;
	};
}

