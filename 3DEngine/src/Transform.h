#pragma once

#include "ConstantBuffers.h"
#include "Actor.h"

namespace D3D11
{
	class Transform : public Bindable
	{
	public:
		Transform(D3D11Core& gfx, const Actor& parent, UINT slot = 0);
		
		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProj;
		};

	protected:
		void UpdateBind(D3D11Core& gfx, const Transforms& tf) noexcept;
		Transforms GetTransforms(D3D11Core& gfx) noexcept;

	private:
		const Actor& m_Parent;
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_ConstantBuffer;
	};
}
