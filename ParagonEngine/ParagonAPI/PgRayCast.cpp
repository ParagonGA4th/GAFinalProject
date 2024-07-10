#include "PgRayCast.h"
#include "../ParagonData/Camera.h"
#include "../ParagonGameEngine/PhysicSystem.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API::Raycast
{
	void PgRayCast::Initialize()
	{
		auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
		_physicSystem = &tPhysicSystem;
	}

	Pg::Data::Collider* PgRayCast::MakeRay(Pg::Math::PGFLOAT3 origin, Pg::Math::PGFLOAT3 dir, float length, Pg::Math::PGFLOAT3& outHitPoint, int* type, bool detectTriggers)
	{
		return _physicSystem->MakeRayCast(origin, dir, length, outHitPoint, type, detectTriggers);
	}
	void PgRayCast::MakeSphereRay(const Pg::Math::PGFLOAT3& tOrigin, const Pg::Math::PGFLOAT3& tDir, float tRad, float max, unsigned int maxColCnt, Pg::Data::Collider**& colDataPointer)
	{
		_physicSystem->MakeSphereCast(tOrigin, tDir, tRad, max, maxColCnt, colDataPointer);
	}
}