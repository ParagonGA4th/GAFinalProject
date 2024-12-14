#pragma once
#include "../ParagonData/GameObject.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include <string>
#include <array>

//�����ؾ� �ϴ� ��� Monster ����Ʈ.
//����, Monster���� ���������� �μ� ������Ʈ���� GrabManagedObjects()���� ���� �־�� �Ѵ�.
//Ex. TAG_Arrow ���� �ֵ�.

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
	
	//Enemy Handler���� ���� ����.
	//class IEnemyBehaviour;

	struct EnemyAggregate
	{
		IEnemyBehaviour* _enemyBehav = nullptr;
		Pg::Data::GameObject* _enemyObj = nullptr;
		TransformSimpleStorage _transStorage;
		bool _isAlive{ true }; //��� ������ �̷��� ����.
	};
}
