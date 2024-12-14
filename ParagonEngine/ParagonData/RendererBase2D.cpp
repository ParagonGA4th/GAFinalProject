#include "RendererBase2D.h"
#include "GameObject.h"
#include <cassert>

namespace Pg::Data
{
	RendererBase2D::RendererBase2D(GameObject* obj) : BaseRenderer(obj)
	{
		
	}

	RendererBase2D::~RendererBase2D()
	{
		//
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
}