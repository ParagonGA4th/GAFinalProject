#include "RendererBase2D.h"
#include "GameObject.h"
#include "IRenderObject.h"

namespace Pg::Data
{
	RendererBase2D::RendererBase2D(GameObject* obj) : BaseRenderer(obj)
	{
		
	}

	RendererBase2D::~RendererBase2D()
	{
		//
	}

	void Pg::Data::RendererBase2D::Start()
	{

	}

	void RendererBase2D::SetSize(float width, float height)
	{
		_height = height;
		_width = width;
	}

	void RendererBase2D::SetWidth(float width)
	{
		_width = width;
	}

	void RendererBase2D::SetHeight(float height)
	{
		_height = height;
	}

	void RendererBase2D::SetIsFocused(bool isFocused)
	{
		_isFocused = isFocused;
	}

	PGFLOAT2 RendererBase2D::GetSize()
	{
		PGFLOAT2 result;

		result.x = _width;
		result.y = _height;

		return result;
	}

	float RendererBase2D::GetWidth()
	{
		return _width;
	}

	float RendererBase2D::GetHeight()
	{
		return _height;
	}

	bool RendererBase2D::IsFocused()
	{
		return _isFocused;
	}

	void RendererBase2D::SetIsClicked(bool isClicked)
	{
		_isClicked = isClicked;
	}

	bool RendererBase2D::IsClicked()
	{
		return _isClicked;
	}

}