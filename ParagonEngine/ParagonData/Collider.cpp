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
		_wasTriggered(false),
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

	PGQuaternion Collider::GetOffsetRotationQuat() const
	{
		physx::PxQuat rotationQuat;

		float x2 = _rotationOffset.x + _rotationOffset.x;
		float y2 = _rotationOffset.y + _rotationOffset.y;
		float z2 = _rotationOffset.z + _rotationOffset.z;

		float xx = _rotationOffset.x * x2;
		float xy = _rotationOffset.x * y2;
		float xz = _rotationOffset.x * z2;

		float yy = _rotationOffset.y * y2;
		float yz = _rotationOffset.y * z2;
		float zz = _rotationOffset.z * z2;

		float wx = _rotationOffset.w * x2;
		float wy = _rotationOffset.w * y2;
		float wz = _rotationOffset.w * z2;

		rotationQuat.x = xy + wz;
		rotationQuat.y = 1.0f - (xx + yy);
		rotationQuat.z = xz - wy;
		rotationQuat.w = 1.0f - (yy + zz);

		PGQuaternion resQuat;

		resQuat.x = rotationQuat.x;
		resQuat.y = rotationQuat.y;
		resQuat.z = rotationQuat.z;
		resQuat.w = rotationQuat.w;

		return resQuat;
	}

	PGFLOAT4X4 Collider::GetOffsetRotationMatrix() const
	{
		PGFLOAT4X4 rotationMatrix =
		{
			1.0f - 2.0f * (_rotationOffset.y * _rotationOffset.y + _rotationOffset.z * _rotationOffset.z),
			2.0f * (_rotationOffset.x * _rotationOffset.y + _rotationOffset.z * _rotationOffset.w),
			2.0f * (_rotationOffset.x * _rotationOffset.z - _rotationOffset.y * _rotationOffset.w),
			0,

			2.0f * (_rotationOffset.x *_rotationOffset.y - _rotationOffset.z * _rotationOffset.w),
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
		//return PGRotationMatrix(GetOffsetRotationQuat());
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
		_wasCollided = false;
		_isCollide = false;

		_wasTriggered = false;
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
		this->_wasCollided = false;
		_collisionStorage.push_back(&c);
	}

	void Collider::Collider_OnCollisionExit(PhysicsCollision& c)
	{
		//std::string tRes = "Collider_OnCollisionExit : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isCollide = false;
		this->_wasCollided = true;
		_collisionStorage.push_back(&c);
	}

	void Collider::Collider_OnTriggerEnter(Collider* c)
	{
		std::string tRes = "Collider_OnTriggerEnter : ";
		//
		// 
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isTrigger = true;
		this->_wasTriggered = false;
		_triggerStorage.push_back(c);
	}

	void Collider::Collider_OnTriggerExit(Collider* c)
	{
		//std::string tRes = "Collider_OnTriggerExit : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isTrigger = false;
		this->_wasTriggered = true;
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

		if (_shape != nullptr)
		{
			_shape->setSimulationFilterData({ GetLayer(), 0, 0, 0 });
		}
	}

	void Collider::BeforePhysicsAwake()
	{
		//위에서는 Nullptr라 다를 것이다. 이제 제대로 들어가는 것.
		SetLayer(_layer);
	}

}