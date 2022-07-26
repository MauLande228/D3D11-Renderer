#pragma once

#include "D3D11Core.h"

#include <memory>
#include <vector>

namespace D3D11
{
	class Bindable;
	class IndexBuffer;
}

class Actor
{
public:
	using id_t = uint32_t;

	Actor() = default;
	Actor(const Actor&) = delete;
	virtual ~Actor() = default;

	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;

	virtual void Update(float dt) noexcept = 0;
	void Draw(D3D11::D3D11Core& gfx) const noexcept(!_DEBUG);

	void AddBind(std::unique_ptr<D3D11::Bindable> bind) noexcept(!_DEBUG);
	void AddIndexBuffer(std::unique_ptr<D3D11::IndexBuffer> ibuf) noexcept;

private:
	const D3D11::IndexBuffer*								m_IndexBuffer = nullptr;
	std::vector<std::unique_ptr<D3D11::Bindable>>	m_Binds;

	id_t m_ID;
};

