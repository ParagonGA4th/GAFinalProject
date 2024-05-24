#include "StaticCapsuleCollider.h"
#include "GameObject.h"

namespace Pg::Data 
{

	StaticCapsuleCollider::StaticCapsuleCollider(GameObject* owner) :
		StaticCollider(owner),
		_width(1.0f),
		_halfHeight(1.0f)
	{

	}

	void StaticCapsuleCollider::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<StaticCapsuleCollider>(this, sv);
	}

	void StaticCapsuleCollider::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<StaticCapsuleCollider>(this, sv);
	}

	void StaticCapsuleCollider::Update()
	{
		//Capsule의 업데이트는 오브젝트의 Scale과 맞게 설정해주어야 함.
		PGFLOAT4X4 worldTM = _object->_transform.GetWorldTM() * GetOffsetTM();
		PGFLOAT3 Whd = { GetWidth() / _object->_transform._scale.x,
						GetHeight() / _object->_transform._scale.y,
						GetDepth() / _object->_transform._scale.z };
		PGFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };

		_capsuleInfo.worldTM = worldTM;
		_capsuleInfo.scale = Whd;
		_capsuleInfo.color = color;
	}

	float StaticCapsuleCollider::GetWidth() const
	{
		return (_width + _halfHeight) * _scaleOffset.x * fabs(_object->_transform._scale.x);
	}

	float StaticCapsuleCollider::GetHeight() const
	{
		return _width * 2 * _scaleOffset.y * fabs(_object->_transform._scale.y);
	}

	float StaticCapsuleCollider::GetDepth() const
	{
		return _width * 2 * _scaleOffset.z * fabs(_object->_transform._scale.z);
	}

	float StaticCapsuleCollider::GetRadius() const
	{
		return _width;
	}

	float StaticCapsuleCollider::GetHalfHeight() const
	{
		return _halfHeight;
	}

	void StaticCapsuleCollider::SetRadius(float r)
	{
		_width = r;
	}

	void StaticCapsuleCollider::SetHalfHeight(float h)
	{
		_halfHeight = h;
	}

	void StaticCapsuleCollider::SetCapsuleInfo(float rad, float halfHeight)
	{
		_width = rad;
		_halfHeight = halfHeight;
	}
}