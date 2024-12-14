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
/// Recast Navigation�� ����� �ʵ�.
/// �Ƹ� PlaneCollider ��ü���� �� �� ����.
/// �� Field�� ����Ǿ�� Agent��ü���� �����δ�.
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

