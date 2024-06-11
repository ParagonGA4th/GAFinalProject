#pragma once
#include "IScriptResettable.h"
#include "IObserver.h"
#include "IScriptResettable.h"

///모든 몬스터가 상속받아야 할 베이스 클래스.
///2024.06.10 기점으로 인터페이스로 사용하기로 변경.

//ScriptResettable은 밑에서 따로 구현해야 하는데, 
//일단 상속받게 하고 현 클래스에서는 자체 assert false를 발동해주자.
namespace Pg::DataScript
{
	class BaseMonster : public IObserver, public IScriptResettable
	{
	public:
		BaseMonster(float fullHealth, float attackPower);

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		//몬스터의 체력을 깎을 떄.
		void ChangeMonsterHealth(float level);


		//체력 / 공격력 관리는 여기서 진행된다.
		const float _fullHealthValue; //전체 체력 외적으로 저장한다.
		float _health;
		float _attackPower;
	};

	struct BaseMonsterHealthChangePair
	{
		BaseMonsterHealthChangePair(BaseMonster* mon, float hChangeLvl) : _baseMonster(mon), _healthChangeLvl(hChangeLvl) {}
		BaseMonster* _baseMonster;
		float _healthChangeLvl; //음수로 되어야 한다.
	};
}


