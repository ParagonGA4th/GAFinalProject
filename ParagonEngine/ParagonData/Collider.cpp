#include "Collider.h"
#include "PhysicsCollision.h"
#include "../ParagonUtil/Log.h"

namespace Pg::Data
{
	using namespace Pg::Math;

	Collider::Collider(GameObject* owner) :
		Component(owner),
		_positionOffSet(0.0f, 0.0f, 0.0f),
		_rotationOffset(1.0f, 0.0f, 0.0f, 0.0f),
		_scaleOffset(1.0f, 1.0f, 1.0f),
		_pxScene(nullptr),
		_isTrigger(false),
		_isCollide(false),
		_wasCollided(false),
		_layer(0)
	{
		_collisionStorage.reserve(5);	
	}

	void Collider::SetPositionOffset(PGFLOAT3 position)
	{
		_positionOffSet = position;
	}

	void Collider::SetRotationOffset(PGQuaternion rotation)
	{
		_rotationOffset = rotation;
	}

	void Collider::SetScaleOffset(PGFLOAT3 scale)
	{
		_scaleOffset = scale;
	}

	PGFLOAT3 Collider::GetPositionOffset() const
	{
		return _positionOffSet;
	}

	PGQuaternion Collider::GetRotationOffset() const
	{
		return _rotationOffset;
	}

	PGFLOAT3 Collider::GetScaleOffset() const
	{
		return _scaleOffset;
	}

	PGFLOAT4X4 Collider::GetOffsetTranslateMatrix() const
	{
		PGFLOAT4X4 posMatrix =
		{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			_positionOffSet.x, _positionOffSet.y, _positionOffSet.z, 1
		};

		return posMatrix;
	}

	Pg::Math::PGFLOAT4X4 Collider::GetOffsetRotationMatrix() const
	{
		PGFLOAT4X4 rotationMatrix =
		{
		   1.0f - 2.0f * (_rotationOffset.y * _rotationOffset.y + _rotationOffset.z * _rotationOffset.z),
		   2.0f * (_rotationOffset.x * _rotationOffset.y + _rotationOffset.z * _rotationOffset.w),
		   2.0f * (_rotationOffset.x * _rotationOffset.z - _rotationOffset.y * _rotationOffset.w),
		   0,

		   2.0f * (_rotationOffset.x * _rotationOffset.y - _rotationOffset.z * _rotationOffset.w),
		   1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.z * _rotationOffset.z),
		   2.0f * (_rotationOffset.y * _rotationOffset.z + _rotationOffset.x * _rotationOffset.w),
		   0,

		   2.0f * (_rotationOffset.x * _rotationOffset.z + _rotationOffset.y * _rotationOffset.w),
		   2.0f * (_rotationOffset.y * _rotationOffset.z - _rotationOffset.x * _rotationOffset.w),
		   1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.y * _rotationOffset.y),
		   0,

		   0,
		   0,
		   0,
		   1
		};

		return rotationMatrix;
	}

	Pg::Math::PGFLOAT4X4 Collider::GetOffsetScaleMatrix() const
	{
		PGFLOAT4X4 scaleMatrix =
		{
			_scaleOffset.x,		0,					0,					0,
			0,					_scaleOffset.y,		0,					0,
			0,					0,					_scaleOffset.z,		0,
			0,					0,					0,					1
		};

		return scaleMatrix;
	}

	Pg::Math::PGFLOAT4X4 Collider::GetOffsetTM() const
	{
		PGFLOAT4X4 res = GetOffsetScaleMatrix() * GetOffsetRotationMatrix() * GetOffsetTranslateMatrix();

		return res;
	}


	void Collider::SetPxScene(physx::PxScene* scene)
	{
		_pxScene = scene;
	}

	physx::PxScene* Collider::GetPxScene()
	{
		return _pxScene;
	}

	void Collider::SetTrigger(bool isTrigger)
	{
		_trigger = isTrigger;
	}

	bool Collider::GetTrigger()
	{
		return _trigger;
	}

	bool Collider::GetIsCollide()
	{
		return _isCollide;
	}

	bool Collider::GetWasCollided()
	{
		return _wasCollided;
	}

	bool Collider::GetIsTrigger()
	{
		return _isTrigger;
	}

	bool Collider::GetWasTrigger()
	{
		return _wasTriggered;
	}
	
	void Collider::Flush()
	{
		_wasCollided = _isCollide;
		_isCollide = false;

		_wasTriggered = _isTrigger;
		_isTrigger = false;

		//매 프레임 체크할 때 마다 초기화.
		_collisionStorage.clear();
		_triggerStorage.clear();
	}

	///API용.
	void Collider::Collider_OnCollisionEnter(PhysicsCollision& c)
	{
		std::string tRes = "Collider_OnCollisionEnter : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		//bool값을 변경해주고 상태를 설정해줘야 Object의 이벤트와 연결이 가능하다.
		this->_isCollide = true;
		_collisionStorage.push_back(&c);
	}

	void Collider::Collider_OnCollisionExit(PhysicsCollision& c)
	{
		std::string tRes = "Collider_OnCollisionExit : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isCollide = false;
		_collisionStorage.push_back(&c);
	}

	void Collider::Collider_OnTriggerEnter(Collider* c)
	{
		std::string tRes = "Collider_OnTriggerEnter : ";
		//
		// 
		// (tRes.append(this->_object->GetName()).c_str());

		this->_isTrigger = true;
		_triggerStorage.push_back(c);
	}

	void Collider::Collider_OnTriggerExit(Collider* c)
	{
		std::string tRes = "Collider_OnTriggerExit : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isTrigger = false;
		_triggerStorage.push_back(c);
	}

	void Collider::SetPxShape(physx::PxShape* shape)
	{
		_shape = shape;
		_shape->userData = this;
	}

	uint32_t Collider::GetLayer()
	{
		return _layer;
	}

	void Collider::SetLayer(uint32_t layer)
	{
		_layer = layer;
		_shape->setSimulationFilterData({ GetLayer(), 0, 0, 0 });
	}

}