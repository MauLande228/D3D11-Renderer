#pragma once

#include "Bindable.h"

#include <vector>

namespace D3D11
{
	class VertexBuffer : public Bindable
	{
	public:
		template<class T>
		VertexBuffer(D3D11Core& gfx, const std::vector<T>& vertices) :
			m_Stride((UINT)sizeof(T))
		{
			D3D11_BUFFER_DESC vBufferDesc{};
			vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vBufferDesc.CPUAccessFlags = 0u;
			vBufferDesc.MiscFlags = 0u;
			vBufferDesc.ByteWidth = UINT(sizeof(T) * vertices.size());
			vBufferDesc.StructureByteStride = sizeof(T);

			D3D11_SUBRESOURCE_DATA subresourceData{};
			subresourceData.pSysMem = vertices.data();
			GFX_EXC(GetDevice(gfx)->CreateBuffer(&vBufferDesc, &subresourceData, &m_VertexBuffer));
		}

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		UINT m_Stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};
}

