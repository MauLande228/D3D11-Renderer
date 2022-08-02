#include "Actor.h"
#include "IndexBuffer.h"

#include <cassert>
#include <typeinfo>

void Actor::Draw(D3D11::D3D11Core& gfx) const NOXND
{
	for (auto& b : m_Binds)
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed(m_IndexBuffer->GetCount());
}

void Actor::AddBind(std::shared_ptr<D3D11::Bindable> bind) NOXND
{
	if (typeid(*bind) == typeid(D3D11::IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && m_IndexBuffer == nullptr);
		m_IndexBuffer = &static_cast<D3D11::IndexBuffer&>(*bind);
	}

	m_Binds.push_back(std::move(bind));
}

