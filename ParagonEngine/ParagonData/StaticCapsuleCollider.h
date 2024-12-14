#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

#include <visit_struct/visit_struct.hpp>
namespace Pg::Data
{
	class GameObject;

	class StaticCapsuleCollider : public StaticCollider
	{
	public:
		StaticCapsuleCollider(GameObject* owner);

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

	public:
		virtual void Update() override;

		virtual float GetWidth() const;
		virtual float GetHeight() const;
		virtual float GetDepth() const;

	public:
		void SetRadius(float r);
		void SetHalfHeight(float h);
		float GetRadius() const;
		float GetHalfHeight() const;

		void SetCapsuleInfo(float rad, float halfHeight);

		Pg::Data::CapsuleInfo _capsuleInfo;

	public:
		BEGIN_VISITABLES(StaticCapsuleCollider);
		//������ �ٸ��� �ȴ�.. ������, Unreal Exporter ���� ������, ������ Width�� �̸��� �ٲ� ���̴�. ���� �ǹ̴� Radius.
		//VISITABLE(float, _radius);
		VISITABLE(float, _width);
		VISITABLE(float, _halfHeight);
		END_VISITABLES;
	};
}

