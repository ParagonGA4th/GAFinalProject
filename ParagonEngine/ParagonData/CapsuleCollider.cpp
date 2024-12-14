#include "CapsuleCollider.h"

namespace Pg::Data
{
	CapsuleCollider::CapsuleCollider(GameObject* owner) :
		DynamicCollider(owner),
		_radius(1.0f),
		_halfHeight(1.0f)
	{
	}

	void CapsuleCollider::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<CapsuleCollider>(this, sv);
	}

	void CapsuleCollider::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<CapsuleCollider>(this, sv);
	}

	void CapsuleCollider::Update()
	{
		//Capsule의 업데이트는 오브젝트의 Scale과 맞게 설정해주어야 함.
		//PGFLOAT4X4 worldTM = GetOffsetTM() * _object->_transform.GetWorldTM();
		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM() * GetOffsetTM();
		PGFLOAT3 Whd = { GetWidth() / _object->_transform._scale.x, 
						GetHeight() / _object->_transform._scale.y, 
						GetDepth()  / _object->_transform._scale.z };
		PGFLOAT4 color = { 0.0f,1.0f,0.0f,1.0f };

		_capsuleInfo.worldTM = worldTM;
		_capsuleInfo.scale = Whd;
		_capsuleInfo.color = color;
	}

	float CapsuleCollider::GetWidth() const
	{
		return (_radius + _halfHeight) * _scaleOffset.x * _object->_transform._scale.x;
	}

	float CapsuleCollider::GetHeight() const
	{
		return _radius * 2 * _scaleOffset.y * _object->_transform._scale.y;
	}

	float CapsuleCollider::GetDepth() const
	{
		return _radius * 2 * _scaleOffset.z * _object->_transform._scale.z;
	}

	float CapsuleCollider::GetRadius() const
	{
		return _radius;
	}

	float CapsuleCollider::GetHalfHeight() const
	{
		return _halfHeight;
	}

	void CapsuleCollider::SetRadius(float r)
	{
		_radius = r;
	}

	void CapsuleCollider::SetHalfHeight(float h)
	{
		_halfHeight = h;
	}

	void CapsuleCollider::SetCapsuleInfo(float rad, float halfHeight)
	{
		_radius = rad;
		_halfHeight = halfHeight;
	}

}