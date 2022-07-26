#pragma once

#include "ConstantBuffers.h"
#include "Actor.h"

namespace D3D11
{
	class Transform : public Bindable
	{
	public:
		Transform(D3D11Core& gfx, const Actor& parent);
		
		void Bind(D3D11Core& gfx) noexcept override;

	private:
		const Actor&							m_Parent;
		VertexConstantBuffer<DirectX::XMMATRIX> m_ConstantBuffer;
	};
}
