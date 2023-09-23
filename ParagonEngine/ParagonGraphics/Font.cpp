#include "Font.h"

#include "LowDX11Storage.h"

namespace Pg::Graphics
{

	Font::Font()
		:_spriteBatch(), _spriteFont(),
		_text(),
		_DXStorage()
	{
		_DXStorage = Pg::Graphics::LowDX11Storage::GetInstance();

		_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);
		_spriteFont = std::make_unique<DirectX::SpriteFont>(_DXStorage->_device, L"../Fonts/NotoSansKR_16.spritefont");

	}

	void Font::Draw()
	{
		_spriteBatch->Begin();
		_spriteFont->DrawString(_spriteBatch.get(), _text.c_str(), _position);
		_spriteBatch->End();
	}

	void Font::SetPosition(float x, float y)
	{
		_position.x = x;
		_position.y = y;
	}

	void Font::SetText(std::wstring text)
	{
		_text = text;
	}

}