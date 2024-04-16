#pragma once
#include "../ParagonMath/PgMath.h"

namespace Pg::Engine
{
	namespace Physic
	{
		class PhysicSystem;
	}
}

namespace Pg::Data
{
	class Collider;
	class Camera;
}

/// <summary>
/// RayCast는 Collider와 다르게 하나의 객체로써 존재하지 않는다.
/// 그러기에 API에 추가하여 스크립팅 시 호출할 수 있게 만들 것이다.
/// 2024.01.24
/// </summary>
namespace Pg::API::Raycast
{
	class PgRayCast
	{
	public:
		void Initialize();

		//스크립팅 시 호출! 
		//히트한 포인트도 참조자로 리턴하게 설정, Collider가 Nullptr가 아닐 경우 참조자 리턴값은 내부에서 변경된 것이다.
		Pg::Data::Collider* MakeRay(Pg::Math::PGFLOAT3 origin, Pg::Math::PGFLOAT3 dir, float length, Pg::Math::PGFLOAT3& outHitPoint, int* type = nullptr);
		//Pg::Data::Collider* ScreenPointToRay(Pg::Data::Camera* cam, Pg::Math::PGFLOAT2 screenPointNormalized, float length, Pg::Math::PGFLOAT3& outHitPoint, int* type = nullptr);
		

	private:
		Pg::Engine::Physic::PhysicSystem* _physicSystem;
	};
}

