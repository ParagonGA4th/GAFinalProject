#pragma once
#include "../ParagonData/GameObject.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include <string>
#include <array>

//관리해야 하는 모든 Monster 리스트.
//또한, Monster들은 개별적으로 부속 오브젝트들을 GrabManagedObjects()에서 갖고 있어야 한다.
//Ex. TAG_Arrow 같은 애들.

namespace Pg::DataScript
{
	class EnemyDefines
	{
	public:
		static constexpr std::array ENEMY_TAG_LIST =
		{
			 "TAG_MiniGolem",
			 "TAG_Boss",
			 "TAG_GolemBoss",
			 "TAG_Mimic",
			 "TAG_Stub",
			 "TAG_Wasp"
		};
	};
	
	//Enemy Handler들을 위해 존재.
	//class IEnemyBehaviour;

	struct EnemyAggregate
	{
		IEnemyBehaviour* _enemyBehav = nullptr;
		Pg::Data::GameObject* _enemyObj = nullptr;
		TransformSimpleStorage _transStorage;
		bool _isAlive{ true }; //살아 있으면 이렇게 리셋.
	};
}
