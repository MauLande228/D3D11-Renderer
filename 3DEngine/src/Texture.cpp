#include "Texture.h"
#include "stb_image.h"

D3D11::Texture::Texture(D3D11Core& gfx, const std::string& filePath, unsigned int slot) :
	m_Slot(slot)
{
	int width, height, numCh;
	unsigned char* imgBuffer = stbi_load(filePath.c_str(), &width, &height, &numCh, STBI_rgb_alpha);
	
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = imgBuffer;
	sd.SysMemPitch = width * 4;
	GFX_EXC(GetDevice(gfx)->CreateTexture2D(&texDesc, &sd, &m_Texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_EXC(GetDevice(gfx)->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureView));
	
}

void D3D11::Texture::Bind(D3D11Core& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(m_Slot, 1u, m_TextureView.GetAddressOf());
}
