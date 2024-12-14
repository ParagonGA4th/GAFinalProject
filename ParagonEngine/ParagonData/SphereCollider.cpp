#include "SphereCollider.h"
#include "GameObject.h"

namespace Pg::Data
{
	SphereCollider::SphereCollider(GameObject* owner) :
		DynamicCollider(owner),
		_rad(1.0f)
	{
	}

	void SphereCollider::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<SphereCollider>(this, sv);
	}

	void SphereCollider::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<SphereCollider>(this, sv);
	}

	void SphereCollider::Update()
	{
		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM() * GetOffsetTM();
		PGFLOAT3 Whd = { GetWidth(), GetHeight(), GetDepth() };
		PGFLOAT4 color = { 0.0f,1.0f,0.0f,1.0f };
		_sphereInfo.worldTM = worldTM;
		_sphereInfo.scale = Whd;
		_sphereInfo.color = color;
	}


	float SphereCollider::GetWidth() const
	{
		return _rad * 2 * _scaleOffset.x * _object->_transform._scale.x;
	}

	float SphereCollider::GetHeight() const
	{
		return _rad * 2 * _scaleOffset.y * _object->_transform._scale.y;
	}

	float SphereCollider::GetDepth() const
	{
		return _rad * 2 * _scaleOffset.z * _object->_transform._scale.z;
	}

	float SphereCollider::GetRadius() const
	{
		return _rad;
	}

	void SphereCollider::SetRadius(float rad)
	{
		_rad = rad;
	}
}