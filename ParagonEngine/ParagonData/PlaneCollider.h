#pragma once
#include "StaticCollider.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 PlaneCollider. 이거는 그냥 Static으로...
/// 2023.12.19
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class PlaneCollider : public StaticCollider
	{
		PlaneCollider(GameObject* owner);

	protected:
		virtual void Update() override;

	public:

		//Pg::Math::PGFLOAT3 GetNormalVector() const;
	};

}
