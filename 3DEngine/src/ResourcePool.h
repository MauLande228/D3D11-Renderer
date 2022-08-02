#pragma once

#include "Bindable.h"

#include <memory>
#include <type_traits>
#include <unordered_map>

class Pool
{
public:
	template<class T, typename... Params>
	static std::shared_ptr<T> Resolve(D3D11::D3D11Core& gfx, Params&&... p) NOXND
	{
		static_assert(std::is_base_of<D3D11::Bindable, T>::value, "Can only resolve classes derived from Bindable");

		return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
	}

private:
	template<class T, typename... Params>
	std::shared_ptr<T> Resolve_(D3D11::D3D11Core& gfx, Params&&... p) NOXND
	{
		const auto key = T::GenerateUID(std::forward<Params>(p)...);
		const auto i = m_Binds.find(key);

		if (i == m_Binds.end())
		{
			auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
			m_Binds[key] = bind;
			
			return bind;
		}
		else
		{
			return std::static_pointer_cast<T>(i->second);
		}
	}

	static Pool& Get()
	{
		static Pool pool;
		return pool;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<D3D11::Bindable>> m_Binds;
};