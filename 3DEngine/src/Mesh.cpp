#include "Mesh.h"
#include "../imgui/imgui.h"

#include <string>

Mesh::Mesh(D3D11::D3D11Core& gfx, std::vector<std::shared_ptr<D3D11::Bindable>> bindPtrs)
{
	AddBind(std::make_shared<D3D11::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}

	AddBind(std::make_shared<D3D11::Transform>(gfx, *this));
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
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		m_MeshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
	}

	m_pRoot = ParseNode(*pScene->mRootNode);
}

void Model::Draw(D3D11::D3D11Core& gfx) const
{
	const auto transformXM = DirectX::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	m_pRoot->Draw(gfx, transformXM);
}

void Model::ShowWindow(const char* windowName) noexcept
{
	if (ImGui::Begin(windowName))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}

std::unique_ptr<Mesh> Model::ParseMesh(D3D11::D3D11Core& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	using D3D11::VertexLayout;

	D3D11::VertexResource vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Bitangent)
		.Append(VertexLayout::Texture2D)));

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mBitangents[i]),
			*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]));
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

	std::vector<std::shared_ptr<D3D11::Bindable>> bindablePtrs;

	using namespace std::string_literals;
	const auto base = "models/nano_textured/"s;

	bool hasSpecularMap = false;
	float shininess = 35.0f;
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
		bindablePtrs.push_back(D3D11::Texture::Resolve(gfx, base + texFileName.C_Str()));

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(D3D11::Texture::Resolve(gfx, base + texFileName.C_Str(), 1));
			hasSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		material.GetTexture(aiTextureType_NORMALS, 0, &texFileName);
		bindablePtrs.push_back(D3D11::Texture::Resolve(gfx, base + texFileName.C_Str(), 2));

		bindablePtrs.push_back(D3D11::Sampler::Resolve(gfx));
	}

	auto meshTag = base + "%" + mesh.mName.C_Str();

	bindablePtrs.push_back(D3D11::VertexBuffer::Resolve(gfx, meshTag, vbuf));

	bindablePtrs.push_back(D3D11::IndexBuffer::Resolve(gfx, meshTag, indices));

	auto pvs = D3D11::VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
	auto pvsbc = pvs->GetByteCode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back(D3D11::InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

	if (hasSpecularMap)
	{
		bindablePtrs.push_back(D3D11::PixelShader::Resolve(gfx, "PhongPSSpecNormalMap.cso"));

		struct PSMaterialConstant
		{
			BOOL normalMapEnabled = TRUE;
			float padding[3];
		}pmc;

		//TODO: Check this later. Possible conflict
		bindablePtrs.push_back(D3D11::PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1));
	}
	else
	{
		bindablePtrs.push_back(D3D11::PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.18f;
			float specularPower;
			BOOL normalMapEnabled = TRUE;
			float padding[1];
		} pmc;

		pmc.specularPower = shininess;
		bindablePtrs.push_back(D3D11::PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1));
	}

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
