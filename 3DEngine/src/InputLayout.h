#pragma once

#include "Bindable.h"
#include "DynamicVertex.h"

#include <vector>

namespace D3D11
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(
			D3D11Core& gfx,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShaderByteCode);

		InputLayout(
			D3D11Core& gfx,
			VertexLayout layout,
			ID3DBlob* pVertexShaderByteCode);

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<InputLayout> Resolve(
			D3D11Core& gfx,
			const VertexLayout& layout,
			ID3DBlob* pVertexShaderByteCode);
		static std::string GenerateUID(const VertexLayout& layout, ID3DBlob* pVertexShaderByteCode = nullptr);
		std::string GetUID() const noexcept override;

	protected:
		VertexLayout m_Layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}

