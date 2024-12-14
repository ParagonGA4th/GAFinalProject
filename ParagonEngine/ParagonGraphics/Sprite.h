#pragma once

#include "DX11Headers.h"

#include "dxtk/SpriteBatch.h"

#include <memory>
#include <string>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class Sprite
	{
	public:
		Sprite(ID3D11DeviceContext* deviceContext, std::wstring textureFilepath);

	public:
		void Draw();
		void SetPosition(float x, float y);
		ID3D11ShaderResourceView* GetSRV();

	private:
		void CreateAndSetTexture(std::wstring textureFilepath);

	private:
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;

	private:
		ID3D11Resource* _textureBuffer;
		ID3D11ShaderResourceView* _textureSRV;
		DirectX::XMFLOAT2 _position;

		Pg::Graphics::LowDX11Storage* _DXStorage;
	};
}