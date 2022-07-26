#pragma once

#include <cstdint>
#include <DirectXMath.h>
#include <vector>

class Geometry
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(
			const DirectX::XMFLOAT3& p,
			const DirectX::XMFLOAT3& b,
			const DirectX::XMFLOAT3& t,
			const DirectX::XMFLOAT2& uv) :
			position(p),
			normal(b),
			tangentU(t),
			texCord(uv)
		{}
		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v) :
			position(px, py, pz),
			normal(nx, ny, nz),
			tangentU(tx, ty, tz),
			texCord(u, v)
		{}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangentU;
		DirectX::XMFLOAT2 texCord;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices32;

		std::vector<uint16_t> GetIndices16()
		{
			if (indices16.empty())
			{
				indices16.resize(indices32.size());
				for (size_t i = 0; i < indices32.size(); i++)
				{
					indices16[i] = static_cast<uint16_t>(indices32[i]);
				}
			}
			return indices16;
		}

	private:
		std::vector<uint16_t> indices16;
	};

	//Creates a box centered at the origin with the given dimensions, where each
	//face has m rows and n columns of vertices
	MeshData CreateBox(float width, float height, float depth, uint32_t numSubdivisions);

	//Creates a sphere centered at the origin with the given radius. The slices and
	//stacks parameters control the degree of tessellation
	MeshData CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);

	//Creates a geosphere centered at the origin with the given radius. The depth
	//controls the level of tessellation
	MeshData CreateGeosphere(float radius, uint32_t numSubdivisions);

	//Creates a cylinder parallel to the y-axis and centered about the origin.
	//The bottom and top radius can vary from various cone shapes rather than
	//true cylinders. The slices and stacks parameters control the degree of
	//tessellation
	MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount);

	//Creates an m x n grid in the xz-plane with m rows and n columns, centered
	//at the origin with the specified width and depth
	MeshData CreateGrid(float width, float depth, uint32_t m, uint32_t n);

	//Create a quad aligned with the screen. This is useful for postprocessig and
	//screen effects.
	MeshData CreateQuad(float x, float y, float w, float h, float depth);

private:
	void Subdivide(MeshData& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
	Vertex MidPoint(const Vertex& v0, const Vertex& v1);

};

