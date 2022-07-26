#pragma once

#include "Actor.h"
#include "IndexBuffer.h"

template<class T>
class ActorBase : public Actor
{
protected:
	static bool IsStaticInitialized() noexcept { return !m_StaticBinds.empty(); }

	static void AddStaticBind(std::unique_ptr<D3D11::Bindable> bind) noexcept(!_DEBUG)
	{
		assert(typeid(*bind) != typeid(D3D11::IndexBuffer) && "MUST use AddIndexBuffer to bind a valid index buffer");

		m_StaticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<D3D11::IndexBuffer> ibuf) noexcept(!_DEBUG)
	{
		assert(m_IndexBuffer == nullptr && "You must only add one index buffer for each actor on your scene");

		m_IndexBuffer = ibuf.get();
		m_StaticBinds.push_back(std::move(ibuf));
	}

	void SetIndexFromStatic() noexcept(!_DEBUG)
	{
		assert(m_IndexBuffer == nullptr && "Attempting to add index buffer a second time");

		for (const auto& b : m_StaticBinds)
		{
			if (const auto p = dynamic_cast<D3D11::IndexBuffer*>(b.get()))
			{
				m_IndexBuffer = p;
				return;
			}
		}

		assert(m_IndexBuffer != nullptr && "Failed to find index buffer in static binds");
	}

private:
	const std::vector<std::unique_ptr<D3D11::Bindable>>& GetStaticBinds() const noexcept override
	{
		return m_StaticBinds;
	}

private:
	static std::vector<std::unique_ptr<D3D11::Bindable>> m_StaticBinds;
};

template<class T>
std::vector<std::unique_ptr<D3D11::Bindable>> ActorBase<T>::m_StaticBinds;