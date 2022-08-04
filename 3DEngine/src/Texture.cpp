#include "Texture.h"
#include "ResourcePool.h"
#include "stb_image.h"

D3D11::Texture::Texture(D3D11Core& gfx, const std::string& filePath, unsigned int slot) :
	m_FilePath(filePath),
	m_Slot(slot)
{
	int width, height, numCh;
	unsigned char* imgBuffer = stbi_load(filePath.c_str(), &width, &height, &numCh, 4);
	
	if (numCh == 4)
	{
		m_HasAlpha = true;
	}
	else
	{
		m_HasAlpha = false;
	}

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	
	GFX_EXC(GetDevice(gfx)->CreateTexture2D(&texDesc, nullptr, &m_Texture));
	
	GetContext(gfx)->UpdateSubresource(
		m_Texture.Get(),
		0u,
		nullptr,
		imgBuffer,
		width * 4,
		0u);

	/*D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = imgBuffer;
	sd.SysMemPitch = width * 4;*/

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	GFX_EXC(GetDevice(gfx)->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureView));
	
	GetContext(gfx)->GenerateMips(m_TextureView.Get());
}

void D3D11::Texture::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(m_Slot, 1u, m_TextureView.GetAddressOf());
}

std::shared_ptr<D3D11::Texture> D3D11::Texture::Resolve(D3D11Core& gfx, const std::string& filePath, UINT slot)
{
	return Pool::Resolve<D3D11::Texture>(gfx, filePath, slot);
}

std::string D3D11::Texture::GenerateUID(const std::string& filePath, UINT slot)
{
	using namespace std::string_literals;
	return typeid(D3D11::Texture).name() + "#"s + filePath + "#" + std::to_string(slot);
}

std::string D3D11::Texture::GetUID() const noexcept
{
	return GenerateUID(m_FilePath, m_Slot);
}

bool D3D11::Texture::HasAlpha() const noexcept
{
	return m_HasAlpha;
}
