#include "Mesh.h"
#include "../imgui/imgui.h"

#include <string>
#include <stdexcept>

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

Model::Model(D3D11::D3D11Core& gfx, const std::string& pathString, const float scale)
{
	Assimp::Importer importer;
	const auto pScene = importer.ReadFile(
		pathString.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace);

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		m_MeshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials, pathString, scale));
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

std::unique_ptr<Mesh> Model::ParseMesh(
	D3D11::D3D11Core& gfx,
	const aiMesh& mesh,
	const aiMaterial* const* pMaterials,
	const std::filesystem::path& pathString,
	float scale)
{
	using namespace std::string_literals;
	using D3D11::VertexLayout;
	using namespace D3D11;
	namespace dx = DirectX;

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	//const auto base = "Models/gobber/"s;
	const auto rootPath = pathString.parent_path().string() + "/";

	bool hasSpecularMap = false;
	bool hasAlphaGloss = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;
	float shininess = 2.0f;
	dx::XMFLOAT4 specularColor = { 0.18f,0.18f,0.18f,1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };

	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, rootPath + texFileName.C_Str()));
			hasDiffuseMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
		}
		if (!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasNormalMap = true;
		}

		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		{
			bindablePtrs.push_back(Sampler::Resolve(gfx));
		}
	}

	const auto meshTag = pathString.string() + "%" + mesh.mName.C_Str();

	//const float scale = 6.0f;

	if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
	{
		VertexResource vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpecNormalMap.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Node::PSMaterialConstantFullmonte pmc;
		pmc.specularPower = shininess;
		pmc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantFullmonte>::Resolve(gfx, pmc, 1u));
	}
	else if (hasDiffuseMap && hasNormalMap)
	{
		VertexResource vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffnorm
		{
			float specularIntensity;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding[1];
		} pmc;
		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		
		//TODO: ISSUE all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffnorm>::Resolve(gfx, pmc, 1u));
	}
	else if (hasDiffuseMap && !hasNormalMap && hasSpecularMap)
	{
		VertexResource vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpec.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffuseSpec
		{
			float specularPowerConst;
			BOOL hasGloss;
			float specularMapWeight;
			float padding;
		} pmc;
		pmc.specularPowerConst = shininess;
		pmc.hasGloss = hasAlphaGloss ? TRUE : FALSE;
		pmc.specularMapWeight = 1.0f;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffuseSpec>::Resolve(gfx, pmc, 1u));
	}
	else if (hasDiffuseMap)
	{
		VertexResource vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		struct PSMaterialConstantDiffuse
		{
			float specularIntensity;
			float specularPower;
			float padding[2];
		} pmc;
		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffuse>::Resolve(gfx, pmc, 1u));
	}
	else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
	{
		VertexResource vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNotex.cso");
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNotex.cso"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Node::PSMaterialConstantNotex pmc;
		pmc.specularPower = shininess;
		pmc.specularColor = specularColor;
		pmc.materialColor = diffuseColor;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantNotex>::Resolve(gfx, pmc, 1u));
	}
	else
	{
		throw std::runtime_error("terrible combination of textures in material smh");
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
