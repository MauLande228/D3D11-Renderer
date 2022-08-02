#pragma once

#include "D3D11Core.h"
#include "Macros.h"

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
	void Draw(D3D11::D3D11Core& gfx) const NOXND;

protected:
	void AddBind(std::shared_ptr<D3D11::Bindable> bind) NOXND;

	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : m_Binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
	}

private:
	const D3D11::IndexBuffer* m_IndexBuffer = nullptr;
	std::vector<std::shared_ptr<D3D11::Bindable>> m_Binds;

	id_t m_ID;
};

