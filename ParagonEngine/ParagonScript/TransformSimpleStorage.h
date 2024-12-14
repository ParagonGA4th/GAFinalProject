#pragma once
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/Transform.h"

/// <summary>
/// 원래 Transform을 회복할 수 있게 만들기.
/// 부모 자식 관계가 바뀔 일이 없으니,
/// 이는 저장 X.
/// </summary>

namespace Pg::DataScript
{
	struct TransformSimpleStorage
	{
		TransformSimpleStorage() = default;
		TransformSimpleStorage(const Pg::Data::Transform* const trans)
			: _pos(trans->_position), _rot(trans->_rotation), _scl(trans->_scale)
		{}

		void ToTransform(Pg::Data::Transform* trans)
		{
			trans->_position = this->_pos;
			trans->_rotation = this->_rot;
			trans->_scale = this->_scl;
		}

	private:
		Pg::Math::PGFLOAT3 _pos;
		Pg::Math::PGQuaternion _rot;
		Pg::Math::PGFLOAT3 _scl;
	};
}
