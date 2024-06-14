#include "StaticSphereCollider.h"
#include "GameObject.h"

namespace Pg::Data
{

	StaticSphereCollider::StaticSphereCollider(GameObject* owner) :
		StaticCollider(owner),
		_rad(1.0f)
	{
	}

	void StaticSphereCollider::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<StaticSphereCollider>(this, sv);
	}

	void StaticSphereCollider::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<StaticSphereCollider>(this, sv);
	}

	void StaticSphereCollider::Update()
	{
		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM() * GetOffsetTM();
		PGFLOAT3 Whd = { GetWidth(), GetHeight(), GetDepth() };
		PGFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };
		_sphereInfo.worldTM = worldTM;
		_sphereInfo.scale = Whd;
		_sphereInfo.color = color;
	}

	float StaticSphereCollider::GetWidth() const
	{
		return _rad * 2 * _scaleOffset.x * fabs(_object->_transform._scale.x);
	}

	float StaticSphereCollider::GetHeight() const
	{
		return _rad * 2 * _scaleOffset.y * fabs(_object->_transform._scale.y);
	}

	float StaticSphereCollider::GetDepth() const
	{
		return _rad * 2 * _scaleOffset.z * fabs(_object->_transform._scale.z);
	}

	float StaticSphereCollider::GetRadius() const
	{
		return _rad;
	}

	void StaticSphereCollider::SetRadius(float rad)
	{
		_rad = rad;
	}

}