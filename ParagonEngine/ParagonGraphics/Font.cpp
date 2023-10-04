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
		_spriteFont = std::make_unique<DirectX::SpriteFont>(_DXStorage->_device, L"../Resources/Fonts/NotoSansKR_13.spritefont");
	}

	void Font::Draw()
	{
		_spriteBatch->Begin();

		//_position.x = _spriteFont->MeasureString(_text.c_str()) / 2.0f;
		//_position.y = _spriteFont->MeasureString(_text.c_str()) / 2.0f;

		_spriteFont->DrawString(_spriteBatch.get(), _text.c_str(), _position, DirectX::Colors::White);
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