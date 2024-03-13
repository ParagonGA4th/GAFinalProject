#include "NavigationSystem.h"

#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	NavigationSystem::NavigationSystem()
	{

	}

	void NavigationSystem::Initialize()
	{
		//Crowd와 NavMeshQuery의 구조체 초기화 및 할당

		_crowd = dtAllocCrowd();

		_navMeshQuery = dtAllocNavMeshQuery();
	}

	void NavigationSystem::Update(float deltaTime)
	{
		if (_navMesh == nullptr || _crowd == nullptr)
		{
			return;
		}

		_crowd->update(deltaTime, nullptr);
	}

	void NavigationSystem::Finalize()
	{

	}

	dtNavMesh* NavigationSystem::GetNavMesh() const
	{
		return _navMesh;
	}

	dtCrowd* NavigationSystem::GetCrowd() const
	{
		return _crowd;
	}

	dtNavMeshQuery* NavigationSystem::GetNavMeshQuery() const
	{
		return _navMeshQuery;
	}

	void NavigationSystem::AddAgent()
	{

	}
}