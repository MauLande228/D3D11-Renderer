#pragma once

#include "Bindable.h"

#include <vector>

namespace D3D11
{
	class IndexBuffer : public Bindable
	{
	public: 
		IndexBuffer(D3D11Core& gfx, const std::vector<uint16_t>& indices);
		IndexBuffer(D3D11Core& gfx, std::string tag, const std::vector<uint16_t>& indices);

		void Bind(D3D11Core& gfx) noexcept override;
		uint32_t GetCount() const noexcept;
		static std::shared_ptr<IndexBuffer> Resolve(
			D3D11Core& gfx,
			const std::string& tag,
			const std::vector<uint16_t>& indices);

		template<typename... Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&... ignore)
		{
			return GenerateUID_(tag);
		}

		std::string GetUID() const noexcept override;

	private:
		static std::string GenerateUID_(const std::string& tag);

	protected:
		std::string m_Tag;
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}

