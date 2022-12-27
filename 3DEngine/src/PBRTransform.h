#pragma once

#include "ConstantBuffers.h"
#include "Actor.h"
#include "Camera.h"

namespace D3D11
{
	class PBRTransform : public Bindable
	{
	public:
		PBRTransform(D3D11Core& gfx, const Actor& parent, Camera& camera, UINT slot = 0);

		void Bind(D3D11Core& gfx) noexcept override;

	protected:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMFLOAT3 CamPos;
		};

	protected:
		void UpdateBind(D3D11Core& gfx, const Transforms& tf) noexcept;
		Transforms GetTransforms(D3D11Core& gfx) noexcept;

	private:
		const Actor& m_Parent;
		Camera& m_Camera;
		static std::unique_ptr<VertexConstantBuffer<Transforms>> m_ConstantBuffer;
	};
}
