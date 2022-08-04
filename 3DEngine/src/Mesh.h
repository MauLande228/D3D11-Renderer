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

	struct PSMaterialConstantFullmonte
	{
		BOOL  normalMapEnabled = TRUE;
		BOOL  specularMapEnabled = TRUE;
		BOOL  hasGlossMap = FALSE;
		float specularPower = 2.1f;
		DirectX::XMFLOAT3 specularColor = { 0.75f,0.75f,0.75f };
		float specularMapWeight = 0.671f;
	};

	struct PSMaterialConstantNotex
	{
		DirectX::XMFLOAT4 materialColor = { 0.447970f,0.327254f,0.176283f,1.0f };
		DirectX::XMFLOAT4 specularColor = { 0.65f,0.65f,0.65f,1.0f };
		float specularPower = 120.0f;
		float padding[3];
	};

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

	void Draw(D3D11::D3D11Core& gfx) const;
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