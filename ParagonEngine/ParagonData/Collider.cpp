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
		return {};
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

	void Collider::SetPxScene(physx::PxScene* scene)
	{
		_pxScene = scene;
	}

	physx::PxScene* Collider::GetPxScene()
	{
		return _pxScene;
	}

}