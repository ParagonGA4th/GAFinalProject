#include "PgRayCast.h"
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

	Pg::Data::Collider* PgRayCast::MakeRay(Pg::Math::PGFLOAT3 origin, Pg::Math::PGFLOAT3 dir, float length, int* type /*= nullptr*/)
	{
		return _physicSystem->MakeRayCast(origin, dir, length, type);
	}

}