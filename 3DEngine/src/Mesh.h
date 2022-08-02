#pragma once

#include "BindableBase.h"
#include "DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public Actor
{
public:
	Mesh(D3D11::D3D11Core& gfx, std::vector<std::shared_ptr<D3D11::Bindable>> bindPtrs);

	void Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX accumulatedTransform) const;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 m_Transform;
};

class Node
{
	friend class Model;
public:
	Node(std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform) NOXND;
	
	void Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX accumulatedTransfor) const NOXND;

private:
	void AddChild(std::unique_ptr<Node> pChild) NOXND;

private:
	std::vector<std::unique_ptr<Node>> m_ChildPtrs;
	std::vector<Mesh*> m_MeshPtrs;
	DirectX::XMFLOAT4X4 m_Transform;
};

class Model
{
public:
	Model(D3D11::D3D11Core& gfx, const std::string filePath);

	void Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX transform) const;
	void ShowWindow(const char* windowName = nullptr) noexcept;

private:
	static std::unique_ptr<Mesh> ParseMesh(D3D11::D3D11Core& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> ParseNode(const aiNode& node);

	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;

private:
	std::unique_ptr<Node> m_pRoot;
	std::vector<std::unique_ptr<Mesh>> m_MeshPtrs;
};