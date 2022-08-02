#pragma once

#include "Bindable.h"
#include "DynamicVertex.h"

#include <vector>

namespace D3D11
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(D3D11Core& gfx, const std::string& tag, const VertexResource& vbuf) :
			m_Stride((UINT)vbuf.GetLayout().Size()),
			m_Tag(tag)
		{
			D3D11_BUFFER_DESC vBufferDesc{};
			vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vBufferDesc.CPUAccessFlags = 0u;
			vBufferDesc.MiscFlags = 0u;
			vBufferDesc.ByteWidth = UINT(vbuf.SizeBytes());
			vBufferDesc.StructureByteStride = m_Stride;

			D3D11_SUBRESOURCE_DATA subresourceData{};
			subresourceData.pSysMem = vbuf.GetData();
			GFX_EXC(GetDevice(gfx)->CreateBuffer(&vBufferDesc, &subresourceData, &m_VertexBuffer));
		}

		VertexBuffer(D3D11Core& gfx, const VertexResource& vbuf) :
			VertexBuffer(gfx, "?", vbuf)
		{
		}

		void Bind(D3D11Core& gfx) noexcept override;
		static std::shared_ptr<VertexBuffer> Resolve(
			D3D11Core& gfx,
			const std::string& tag,
			const VertexResource& vbuf);
		
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
		UINT m_Stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};
}

