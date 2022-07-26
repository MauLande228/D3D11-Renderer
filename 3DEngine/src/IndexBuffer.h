#pragma once

#include "Bindable.h"

#include <vector>

namespace D3D11
{
	class IndexBuffer : public Bindable
	{
	public: 
		IndexBuffer(D3D11Core& gfx, const std::vector<uint16_t>& indices);

		void Bind(D3D11Core& gfx) noexcept override;
		uint32_t GetCount() const noexcept;

	protected:
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}

