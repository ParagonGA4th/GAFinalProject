#include "Sprite.h"

#include "dxtk/DDSTextureLoader.h"
#include "LowDX11Storage.h"

#pragma comment (lib, "DirectXTK.lib")

Pg::Graphics::Sprite::Sprite(ID3D11DeviceContext* deviceContext, std::wstring textureFilepath) :_spriteBatch(),
	_textureSRV(nullptr),
	_position()
{
	_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
	_DXStorage = Pg::Graphics::LowDX11Storage::GetInstance();

	CreateAndSetTexture(textureFilepath);
}

void Pg::Graphics::Sprite::Draw()
{
	_spriteBatch->Begin();
	_spriteBatch->Draw(_textureSRV, _position);
	_spriteBatch->End();
}

ID3D11ShaderResourceView* Pg::Graphics::Sprite::GetSRV()
{
	return _textureSRV;
}

void Pg::Graphics::Sprite::CreateAndSetTexture(std::wstring textureFilepath)
{
	// TODO: DDS가 아닌 파일도 셋팅 가능하도록 기능 추가
	//_DXStorage->_bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//_DXStorage->_bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//_DXStorage->_device->CreateTexture2D(&(_DXStorage->_bufferDesc), NULL, &_textureBuffer);

	//_DXStorage->_shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//_DXStorage->_shaderResourceViewDesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
	//_DXStorage->_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	//_DXStorage->_shaderResourceViewDesc.Texture2D.MipLevels = -1;

	//HRESULT hr = _DXStorage->_device->CreateShaderResourceView()

	HRESULT hr = DirectX::CreateDDSTextureFromFile(_DXStorage->_device, textureFilepath.c_str(), &_textureBuffer, &_textureSRV);

}

void Pg::Graphics::Sprite::SetPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

