#pragma once

#include "Bindable.h"

namespace D3D11
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(D3D11Core& gfx, const std::string& filePath);

		void Bind(D3D11Core& gfx) noexcept override;
		ID3DBlob* GetByteCode() const noexcept;

		static std::shared_ptr<VertexShader> Resolve(D3D11Core& gfx, const std::string& filePath);
		static std::string GenerateUID(const std::string& filePath);
		std::string GetUID() const noexcept override;

	protected:
		std::string m_FilePath;
		Microsoft::WRL::ComPtr<ID3DBlob>			m_ByteCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_VertexShader;
	};
}

