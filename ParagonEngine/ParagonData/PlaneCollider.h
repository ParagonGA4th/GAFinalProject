#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// �������� PlaneCollider. �̰Ŵ� �׳� Static����...
/// �������� ������ �� ����.
/// 2023.12.19
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class PlaneCollider : public StaticCollider
	{
	public:
		PlaneCollider(GameObject* owner);

		virtual void OnSerialize(SerializeVector& sv);
		virtual void OnDeserialize(SerializeVector& sv);

	protected:
		virtual void Update() override;

	public:
		Pg::Math::PGFLOAT3 GetNormalVector() const;
		float GetDistance();

		float GetWidth();
		float GetDepth();


		//������� ���� ����ü
		PlaneInfo _planeInfo;


		//Plane�� �β��� ����. ���� ���� ���̸� ����

		BEGIN_VISITABLES(PlaneCollider);
		VISITABLE(float,_width);
		VISITABLE(float,_depth);
		END_VISITABLES;		

	};
}
