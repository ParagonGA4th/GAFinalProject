#include "PlaneCollider.h"
#include "GameObject.h"
#include "Transform.h"

namespace Pg::Data
{
	using namespace Pg::Math;

	void PlaneCollider::Update()
	{
		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM();
		PGFLOAT3 Wd = { 0.01f, GetWidth(), GetDepth() };
		PGFLOAT4 color = { 0.0f, 0.1f, 0.0f, 1.0f };
		_planeInfo.worldTM = worldTM;
		_planeInfo.scale = Wd;
		_planeInfo.color = color;
	}

	float PlaneCollider::GetDistance()
	{
		return true;
	}

	float PlaneCollider::GetWidth()
	{
		return _width;
	}

	float PlaneCollider::GetDepth()
	{
		return _depth;
	}

	/*Pg::Math::PGFLOAT3 PlaneCollider::GetNormalVector() const
	{
		return true;
	}*/
}