#pragma once
#include "../ParagonMath/PgMath.h"
#include "BuildSettings.h"
#include "Component.h"

#include <DetourTileCache/DetourTileCache.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>

#include <vector>

/// <summary>
/// Recast Navigation이 적용될 필드.
/// 아마 PlaneCollider 객체에만 들어갈 것 같다.
/// 이 Field가 빌드되어야 Agent객체들이 움직인다.
/// 2024.03.10
/// </summary>
namespace Pg::Data
{
	class GameObject;

	class NavigationField : public Component
	{
	public:
		NavigationField(GameObject* owner);

		virtual void Start() override;
		virtual void Update() override;
	
	private:
	};
}

