#include "UIRenderer.h"

namespace Pg::Data
{

	UIRenderer::UIRenderer(GameObject* obj)
		: Component(obj),
		_width(1.f),
		_height(1.f),
		_isFocused(false)
	{

	}

	void Pg::Data::UIRenderer::Start()
	{

	}

	void UIRenderer::SetSize(float width, float height)
	{
		_height = height;
		_width = width;
	}

	void UIRenderer::SetWidth(float width)
	{
		_width = width;
	}

	void UIRenderer::SetHeight(float height)
	{
		_height = height;
	}

	void UIRenderer::SetIsFocused(bool isFocused)
	{
		_isFocused = isFocused;
	}

	Pg::Math::PGFLOAT2 UIRenderer::GetSize()
	{
		PGFLOAT2 result = { _width, _height };
		return result;
	}

	float UIRenderer::GetWidth()
	{
		return _width;
	}

	float UIRenderer::GetHeight()
	{
		return _height;
	}

	bool UIRenderer::IsFocused()
	{
		return _isFocused;
	}

	void UIRenderer::SetIsClicked(bool isClicked)
	{
		_isClicked = isClicked;
	}

	bool UIRenderer::IsClicked()
	{
		return _isClicked;
	}

}
