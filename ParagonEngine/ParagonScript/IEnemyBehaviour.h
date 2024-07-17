#pragma once
#include "BaseMonster.h"
//#include "IScriptResettable.h"

/// <summary>
/// 모든 몬스터의 BehaviourScript가 상속받을 인터페이스.
/// 오직 GetBaseMonsterInfo만 Player에게 넘겨주기 위함이다.
/// 2024.06.11
/// </summary>

namespace Pg::DataScript
{
	class IEnemyBehaviour
	{
		//플레이어가 일반공격으로 몬스터를 타격 시 충전되는 마나
		//inline static const float INCREMENT_MANA_AMT = 5.f;
	public:
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() abstract;
		virtual unsigned int GetBelongAreaIndex() abstract; //자신이 속해 있는 Area Index를 반환한다.
		virtual void ResetAll() abstract;
	};
}

