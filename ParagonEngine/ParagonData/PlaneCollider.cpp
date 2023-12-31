#include "PlaneCollider.h"
#include "GameObject.h"
#include "Transform.h"

namespace Pg::Data
{
	using namespace Pg::Math;


	PlaneCollider::PlaneCollider(GameObject* owner) :
		StaticCollider(owner),
		_width(20.0f),
		_depth(20.0f)
	{

	}

	void PlaneCollider::Update()
	{
		using namespace Pg::Math;

		PGFLOAT4X4 worldTM = GetOffsetTM() * _object->_transform.GetWorldTM();
		PGFLOAT3 Wd = { GetWidth(), 1.0f, GetDepth()};
		PGFLOAT4 color = { 0.0f, 0.0f, 1.0f, 1.0f };
		_planeInfo.worldTM = worldTM;
		_planeInfo.scale = Wd;
		_planeInfo.color = color;
	}

	float PlaneCollider::GetDistance()
	{
		return PGFloat3Length(_object->_transform.GetLocalPosition());
	}

	float PlaneCollider::GetWidth()
	{
		return _width * _scaleOffset.x * _object->_transform.GetScale().x;
	}

	float PlaneCollider::GetDepth()
	{
		return _depth * _scaleOffset.z * _object->_transform.GetScale().z;
	}

	PGFLOAT3 PlaneCollider::GetNormalVector() const
	{
		PGFLOAT3 tmp = _object->_transform.GetRight();
		return PGFloat3Normalize(tmp);
	}
}