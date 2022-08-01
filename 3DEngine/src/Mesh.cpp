#include "Mesh.h"

Mesh::Mesh(D3D11::D3D11Core& gfx, std::vector<std::unique_ptr<D3D11::Bindable>> bindPtrs)
{
	if (!IsStaticInitialized())
	{
		AddStaticBind(std::make_unique<D3D11::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto& pb : bindPtrs)
	{
		if (auto pi = dynamic_cast<D3D11::IndexBuffer*>(pb.get()))
		{
			AddIndexBuffer(std::unique_ptr<D3D11::IndexBuffer>{pi});
			pb.release();
		}
		else
		{
			AddBind(std::move(pb));
		}
	}

	AddBind(std::make_unique<D3D11::Transform>(gfx, *this));
}

void Mesh::Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX accumulatedTransform) const
{
	DirectX::XMStoreFloat4x4(&m_Transform, accumulatedTransform);
	Actor::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_Transform);
}

Node::Node(std::vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform) NOXND
	:
	m_MeshPtrs(std::move(pMeshes))
{

	DirectX::XMStoreFloat4x4(&m_Transform, transform);
}

void Node::Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX accumulatedTransfor) const NOXND
{
	const auto built = DirectX::XMLoadFloat4x4(&m_Transform) * accumulatedTransfor;

	for (const auto pm : m_MeshPtrs)
	{
		pm->Draw(gfx, built);
	}

	for (const auto& pc : m_ChildPtrs)
	{
		pc->Draw(gfx, built);
	}
}

void Node::AddChild(std::unique_ptr<Node> pChild) NOXND
{
	assert(pChild);

	m_ChildPtrs.push_back(std::move(pChild));
}

Model::Model(D3D11::D3D11Core& gfx, const std::string filePath)
{
	Assimp::Importer importer;
	const auto pScene = importer.ReadFile(
		filePath.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		m_MeshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
	}

	m_pRoot = ParseNode(*pScene->mRootNode);
}

void Model::Draw(D3D11::D3D11Core& gfx, DirectX::FXMMATRIX transform) const
{
	m_pRoot->Draw(gfx, transform);
}

std::unique_ptr<Mesh> Model::ParseMesh(D3D11::D3D11Core& gfx, const aiMesh& mesh)
{
	using D3D11::VertexLayout;

	D3D11::VertexResource vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)));

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]));
	}

	std::vector<uint16_t> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::unique_ptr<D3D11::Bindable>> bindablePtrs;
	bindablePtrs.push_back(std::make_unique<D3D11::VertexBuffer>(gfx, vbuf));
	bindablePtrs.push_back(std::make_unique<D3D11::IndexBuffer>(gfx, indices));

	auto pvs = std::make_unique<D3D11::VertexShader>(gfx, L"PhongVS.cso");
	auto pvsbc = pvs->GetByteCode();

	bindablePtrs.push_back(std::move(pvs));
	bindablePtrs.push_back(std::make_unique<D3D11::PixelShader>(gfx, L"PhongPS.cso"));

	bindablePtrs.push_back(std::make_unique<D3D11::InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = { 0.6f, 0.6f, 0.8f };
		float specularIntesity = 0.6f;
		float speculatPower = 30.0f;
		float padding[3];
	}Pmc;

	bindablePtrs.push_back(std::make_unique<D3D11::PixelConstantBuffer<PSMaterialConstant>>(gfx, Pmc, 1u));

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	const auto transform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(
		reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)));

	std::vector<Mesh*> currMeshPtrs;
	currMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshId = node.mMeshes[i];
		currMeshPtrs.push_back(m_MeshPtrs.at(meshId).get());
	}

	auto pNode = std::make_unique<Node>(std::move(currMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[i]));
	}

	return pNode;
}
