#include "Actor.h"
#include "IndexBuffer.h"

#include <cassert>
#include <typeinfo>

void Actor::Draw(D3D11::D3D11Core& gfx) const noexcept(!_DEBUG)
{
	for (auto& b : m_Binds)
	{
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx);
	}

	//gfx.DrawIndexed(m_IndexBuffer->GetCount());
	gfx.DrawIndexed(m_Count);
}

void Actor::AddBind(std::unique_ptr<D3D11::Bindable> bind) noexcept(!_DEBUG)
{
	assert(typeid(*bind) != typeid(D3D11::IndexBuffer) && "MUST use AddIndexBuffer to bind a valid index buffer");

	m_Binds.push_back(std::move(bind));
}

void Actor::AddIndexBuffer(std::unique_ptr<D3D11::IndexBuffer> ibuf) noexcept
{
	assert(m_IndexBuffer == nullptr && "You must only add one index buffer for each actor on your scene");

	m_IndexBuffer = ibuf.get();
	m_Binds.push_back(std::move(ibuf));
}

size_t Actor::m_Count = 0;
