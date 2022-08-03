#pragma once

#include "Transform.h"

namespace D3D11
{
	class GlobalTransform : public Transform
	{
	public:
		GlobalTransform(D3D11Core& gfx, const Actor& parent, UINT slotV = 0, UINT slotP = 0);
		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		void UpdateBind(D3D11Core& gfx, const Transforms& tf) noexcept;

	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> m_ConstantBufferPS;
	};
}