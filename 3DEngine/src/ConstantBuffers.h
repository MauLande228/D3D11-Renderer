#pragma once

#include "Bindable.h"

namespace D3D11
{
	template<class T>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(D3D11Core& gfx, const T& data, UINT slot = 0u) :
			m_Slot(slot)
		{
			D3D11_BUFFER_DESC cBufferDesc{};
			cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cBufferDesc.MiscFlags = 0u;
			cBufferDesc.ByteWidth = sizeof(data);
			cBufferDesc.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA subresourceData{};
			subresourceData.pSysMem = &data;
			GFX_EXC(GetDevice(gfx)->CreateBuffer(&cBufferDesc, &subresourceData, &m_ConstantBuffer));
		}

		ConstantBuffer(D3D11Core& gfx, UINT slot = 0) :
			m_Slot(slot)
		{
			D3D11_BUFFER_DESC cBufferDesc{};
			cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cBufferDesc.MiscFlags = 0u;
			cBufferDesc.ByteWidth = sizeof(T);
			cBufferDesc.StructureByteStride = 0u;

			GFX_EXC(GetDevice(gfx)->CreateBuffer(&cBufferDesc, nullptr, &m_ConstantBuffer));
		}

		void Update(D3D11Core& gfx, const T& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			GFX_EXC(GetContext(gfx)->Map(
				m_ConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr
			));
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(gfx)->Unmap(m_ConstantBuffer.Get(), 0u);
		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		UINT m_Slot;
	};

	template<class T>
	class VertexConstantBuffer : public ConstantBuffer<T>
	{
		using ConstantBuffer<T>::m_ConstantBuffer;
		using ConstantBuffer<T>::m_Slot;
		using Bindable::GetContext;

	public:
		using ConstantBuffer<T>::ConstantBuffer;
		
		void Bind(D3D11Core& gfx) noexcept override
		{
			GetContext(gfx)->VSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<class T>
	class PixelConstantBuffer : public ConstantBuffer<T>
	{
		using ConstantBuffer<T>::m_ConstantBuffer;
		using ConstantBuffer<T>::m_Slot;
		using Bindable::GetContext;

	public:
		using ConstantBuffer<T>::ConstantBuffer;

		void Bind(D3D11Core& gfx) noexcept override
		{
			GetContext(gfx)->PSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};
}