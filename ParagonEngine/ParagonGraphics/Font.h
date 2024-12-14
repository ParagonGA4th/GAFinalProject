#pragma once

#include "dxtk/SpriteBatch.h"
#include "dxtk/SpriteFont.h"

#include <string>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class Font
	{
	public:
		Font();

	public:
		void Draw();
		void SetPosition(float x, float y);
		void SetText(std::wstring text);

	private:
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> _spriteFont;
		DirectX::XMFLOAT2 _position;
		std::wstring _text;

	private:
		LowDX11Storage* _DXStorage;
	};
}


