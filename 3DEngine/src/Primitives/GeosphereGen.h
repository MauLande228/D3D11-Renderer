#pragma once
#pragma once
#include <optional>
#include <DirectXMath.h>

#include "../DynamicVertex.h"
#include "../IndexedTriangleList.h"
#include "../Geometry.h"
#include "../Math.h"

class GeosphereGen
{
public:
	static IndexedTriangleList MakeTesselated(D3D11::VertexLayout layout, int latDiv, int longDiv)
	{
		namespace dx = DirectX;
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		Geometry geoGen;

		//auto model = geoGen.CreateGeosphere(2.0f, 10);
		auto model = geoGen.CreateSphere(5, 40, 40);
		D3D11::VertexResource vb{ std::move(layout) };
		float scale = 1.0f;

		for (unsigned int i = 0; i < model.Vertices.size(); i++)
		{
			vb.EmplaceBack(
				dx::XMFLOAT3(model.Vertices[i].Position.x * scale, model.Vertices[i].Position.y * scale, model.Vertices[i].Position.z * scale),
				model.Vertices[i].Normal,
				model.Vertices[i].TangentU,
				model.Vertices[i].TexC);
		}

		return { std::move(vb),std::move(model.GetIndices16()) };
	}

	static IndexedTriangleList Make(std::optional<D3D11::VertexLayout> layout = std::nullopt)
	{
		using Element = D3D11::VertexLayout::ElementType;
		if (!layout)
		{
			layout = D3D11::VertexLayout{}
				.Append(Element::Position3D)
				.Append(Element::Normal)
				.Append(Element::Tangent)
				.Append(Element::Texture2D);
		}
		return MakeTesselated(std::move(*layout), 12, 24);
	}
};