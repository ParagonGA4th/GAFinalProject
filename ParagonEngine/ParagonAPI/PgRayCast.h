#pragma once
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/LayerMask.h"

#include <memory>

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
/// RayCast�� Collider�� �ٸ��� �ϳ��� ��ü�ν� �������� �ʴ´�.
/// �׷��⿡ API�� �߰��Ͽ� ��ũ���� �� ȣ���� �� �ְ� ���� ���̴�.
/// 2024.01.24
/// </summary>
namespace Pg::API::Raycast
{
	class PgRayCast
	{
	public:
		void Initialize();

		//��ũ���� �� ȣ��! 
		//��Ʈ�� ����Ʈ�� �����ڷ� �����ϰ� ����, Collider�� Nullptr�� �ƴ� ��� ������ ���ϰ��� ���ο��� ����� ���̴�.
		Pg::Data::Collider* MakeRay(Pg::Math::PGFLOAT3 origin, Pg::Math::PGFLOAT3 dir, float length, Pg::Math::PGFLOAT3& outHitPoint, int* type = nullptr, bool detectTriggers = true);
		//Pg::Data::Collider* ScreenPointToRay(Pg::Data::Camera* cam, Pg::Math::PGFLOAT2 screenPointNormalized, float length, Pg::Math::PGFLOAT3& outHitPoint, int* type = nullptr);
		
		void MakeSphereRay(const Pg::Math::PGFLOAT3& tOrigin, const Pg::Math::PGFLOAT3& tDir,
			float tRad, float max, unsigned int maxColCnt, Pg::Data::Collider**& colDataPointer);

	private:
		Pg::Engine::Physic::PhysicSystem* _physicSystem;
	};
}

