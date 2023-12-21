#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// 변지상의 PlaneCollider. 이거는 그냥 Static으로...
/// 동적으로 움직일 일 없음.
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
		float GetDistance();

		float GetWidth();
		float GetDepth();

		//디버깅을 위한 구조체
		PlaneInfo _planeInfo;

	private:
		//Plane은 두께는 고정. 가로 세로 길이만 조정
		float _width;
		float _depth;
	};

}
