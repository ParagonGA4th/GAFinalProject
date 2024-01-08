#include "Collider.h"

namespace Pg::Data
{
	using namespace Pg::Math;

	Collider::Collider(GameObject* owner) :
		Component(owner),
		_positionOffSet(0.0f, 0.0f, 0.0f),
		_rotationOffset(1.0f, 0.0f, 0.0f, 0.0f),
		_scaleOffset(1.0f, 1.0f, 1.0f),
		_pxScene(nullptr)
	{
		
	}

	void Collider::SetPoisitonOffset(PGFLOAT3 position)
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
		_isTrigger = isTrigger;
	}

	bool Collider::GetTrigger()
	{
		return _isTrigger;
	}

}