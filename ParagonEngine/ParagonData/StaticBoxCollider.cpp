#include "StaticBoxCollider.h"
#include "GameObject.h"

namespace Pg::Data
{

	StaticBoxCollider::StaticBoxCollider(GameObject* owner) :
		StaticCollider(owner)
	{

	}

	void StaticBoxCollider::Update()
	{

	}

	float StaticBoxCollider::GetWidth() const
	{
		return _width;
	}

	float StaticBoxCollider::GetHeight() const
	{
		return _height;
	}

	float StaticBoxCollider::GetDepth() const
	{
		return _depth;
	}

}