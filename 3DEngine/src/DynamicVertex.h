#pragma once

#include "D3D11Core.h"
#include "Macros.h"

#include <vector>
#include <type_traits>

namespace D3D11
{
	struct BGRAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	/*
	************** VERTEX LAYOUT **************
	*/

	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count
		};

		template<ElementType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* Semantic = "POSITION";
			static constexpr const char* Code = "P2";
		};

		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "POSITION";
			static constexpr const char* Code = "P3";
		};

		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* Semantic = "TEXCOORD";
			static constexpr const char* Code = "T2";
		};

		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "NORMAL";
			static constexpr const char* Code = "N";
		};

		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* Semantic = "COLOR";
			static constexpr const char* Code = "C3";
		};

		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* Semantic = "COLOR";
			static constexpr const char* Code = "C4";
		};

		template<> struct Map<BGRAColor>
		{
			using SysType = D3D11::BGRAColor;
			static constexpr DXGI_FORMAT DxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* Semantic = "COLOR";
			static constexpr const char* Code = "C8";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset) :
				m_Type(type),
				m_Offset(offset)
			{}

			size_t GetOffsetAfter() const NOXND
			{
				return m_Offset + Size();
			}

			size_t GetOffset() const
			{
				return m_Offset;
			}

			size_t Size() const NOXND
			{
				return SizeOf(m_Type);
			}

			const char* GetCode() const noexcept
			{
				switch (m_Type)
				{
				case Position2D:
					return Map<Position2D>::Code;
				case Position3D:
					return Map<Position3D>::Code;
				case Texture2D:
					return Map<Texture2D>::Code;
				case Normal:
					return Map<Normal>::Code;
				case Float3Color:
					return Map<Float3Color>::Code;
				case Float4Color:
					return Map<Float4Color>::Code;
				case BGRAColor:
					return Map<BGRAColor>::Code;
				}
				assert("Invalid element type" && false);
				return "Invalid";
			}

			static constexpr size_t SizeOf(ElementType type) NOXND
			{
				switch (type)
				{
				case Position2D:
					return sizeof(Map<Position2D>::SysType);
				case Position3D:
					return sizeof(Map<Position3D>::SysType);
				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);
				case Normal:
					return sizeof(Map<Normal>::SysType);
				case Float3Color:
					return sizeof(Map<Float3Color>::SysType);
				case Float4Color:
					return sizeof(Map<Float4Color>::SysType);
				case BGRAColor:
					return sizeof(Map<BGRAColor>::SysType);
				}

				assert(false && "Invalid element type");
				return 0u;
			}

			ElementType GetType() const noexcept
			{
				return m_Type;
			}

			D3D11_INPUT_ELEMENT_DESC GetDesc() const NOXND
			{
				switch (m_Type)
				{
				case Position2D:
					return GenerateDesc<Position2D>(GetOffset());
				case Position3D:
					return GenerateDesc<Position3D>(GetOffset());
				case Texture2D:
					return GenerateDesc<Texture2D>(GetOffset());
				case Normal:
					return GenerateDesc<Normal>(GetOffset());
				case Float3Color:
					return GenerateDesc<Float3Color>(GetOffset());
				case Float4Color:
					return GenerateDesc<Float4Color>(GetOffset());
				case BGRAColor:
					return GenerateDesc<BGRAColor>(GetOffset());
				}
				assert(false && "Invalid element type");
				return { "INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}

		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) NOXND
			{
				return { Map<type>::Semantic, 0, Map<type>::DxgiFormat, 0, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}

		private:
			ElementType m_Type;
			size_t		m_Offset;
		};

	public:
		template<ElementType type>
		const Element& Resolve() const NOXND
		{
			for (auto& e : m_Elements)
			{
				if (e.GetType() == type)
				{
					return e;
				}
			}
			assert(false && "Could not resolve element type");

			return m_Elements.front();
		}

		const Element& ResolveByIndex(size_t i) const NOXND
		{
			return m_Elements[i];
		}

		VertexLayout& Append(ElementType type) NOXND
		{
			m_Elements.emplace_back(type, Size());
			return *this;
		}

		size_t Size() const NOXND
		{
			return m_Elements.empty() ? 0u : m_Elements.back().GetOffsetAfter();
		}

		size_t GetElementCount() const noexcept
		{
			return m_Elements.size();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const NOXND
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(GetElementCount());

			for (const auto& e : m_Elements)
			{
				desc.push_back(e.GetDesc());
			}

			return desc;
		}

		std::string GetCode() const NOXND
		{
			std::string code;
			for (const auto& e : m_Elements)
			{
				code += e.GetCode();
			}
			return code;
		}

	private:
		std::vector<Element> m_Elements;
	};

	/*
	************** VERTEX **************
	*/

	class Vertex
	{
		friend class VertexResource;
	public:
		template<VertexLayout::ElementType Type>
		auto& Attr() NOXND
		{
			auto pAttribute = m_pData + m_Layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) NOXND
		{
			const auto& element = m_Layout.ResolveByIndex(i);
			auto pAttribute = m_pData + element.GetOffset();

			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert(false && "Bad element type");
			}
		}

	protected:
		Vertex(char* pData, const VertexLayout& layout) NOXND :
			m_pData(pData),
			m_Layout(layout)
		{
			assert(m_pData != nullptr);
		}

	private:
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) NOXND
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		template<VertexLayout::ElementType DestLayoutTYpe, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) NOXND
		{
			using Dest = typename VertexLayout::Map<DestLayoutTYpe>::SysType;

			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert(false && "Parameter attribute type mismatch");
			}
		}

	private:
		char* m_pData = nullptr;
		const VertexLayout& m_Layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) NOXND :
			m_Vertex(v)
		{}

		template<VertexLayout::ElementType Type>
		const auto& Attr() const NOXND
		{
			return const_cast<Vertex&>(m_Vertex).Attr<Type>();
		}

	private:
		Vertex m_Vertex;
	};

	/*
	************** VERTEX RESOURCE **************
	*/

	class VertexResource
	{
	public:
		VertexResource(VertexLayout layout) NOXND :
			m_Layout(std::move(layout))
		{}

		const char* GetData() const NOXND
		{
			return m_Buffer.data();
		}

		const VertexLayout& GetLayout() const noexcept
		{
			return m_Layout;
		}

		size_t Size() const NOXND
		{
			return m_Buffer.size() / m_Layout.Size();
		}

		size_t SizeBytes() const NOXND
		{
			return m_Buffer.size();
		}

		template<typename ...Params>
		void EmplaceBack(Params&&... params) NOXND
		{
			assert(sizeof...(params) == m_Layout.GetElementCount() && "Param count does not match number of vertex elements");

			m_Buffer.resize(m_Buffer.size() + m_Layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back() NOXND
		{
			assert(m_Buffer.size() != 0u);

			return Vertex{ m_Buffer.data() + m_Buffer.size() - m_Layout.Size(), m_Layout };
		}

		Vertex Front() NOXND
		{
			assert(m_Buffer.size() != 0u);

			return Vertex{ m_Buffer.data(), m_Layout };
		}

		Vertex operator[](size_t i) NOXND
		{
			assert(i < Size());

			return Vertex{ m_Buffer.data() + m_Layout.Size() * i, m_Layout };
		}

		ConstVertex Back() const NOXND
		{
			return const_cast<VertexResource*>(this)->Back();
		}
		ConstVertex Front() const NOXND
		{
			return const_cast<VertexResource*>(this)->Front();
		}
		ConstVertex operator[](size_t i) const NOXND
		{
			return const_cast<VertexResource&>(*this)[i];
		}

	private:
		std::vector<char> m_Buffer;
		VertexLayout m_Layout;
	};
}