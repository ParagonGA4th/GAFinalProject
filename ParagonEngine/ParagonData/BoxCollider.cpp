#include "BoxCollider.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Engine
{
	BoxCollider::BoxCollider(Pg::Data::GameObject* owner) :
		_width(1.0f),
		_height(1.0f),
		_depth(1.0f)
	{

	}

	void BoxCollider::Update()
	{

	}

	float BoxCollider::GetWidth() const
	{
		return _width;
	}

	float BoxCollider::GetHeight() const
	{
		return _height;
	}

	float BoxCollider::GetDepth() const
	{
		return _depth;
	}

}