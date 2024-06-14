#pragma once
#include "IScriptResettable.h"
#include "IObserver.h"
#include "IScriptResettable.h"

#include <functional>

///모든 몬스터가 상속받아야 할 베이스 클래스.
///2024.06.10 기점으로 인터페이스로 사용하기로 변경.

//ScriptResettable은 밑에서 따로 구현해야 하는데, 
//일단 상속받게 하고 현 클래스에서는 자체 assert false를 발동해주자.
namespace Pg::DataScript
{
	class BaseMonsterInfo : public IObserver, public IScriptResettable
	{
	public:
		BaseMonsterInfo(float fullHealth, float attackPower);

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		void SetMonsterHp(float hp);

		//몬스터의 체력을 깎을 떄.
		void ChangeMonsterHp(float level);

		void SetMonsterDamage(float damage);

		float GetMonsterHp();
		float GetMonsterDamage();

		//사망 시 발생하는 함수.
		std::function<void()> _onDead;

		//피격 시 발생하는 함수.
		std::function<void()> _onHit;
	protected:
		//체력 / 공격력 관리는 여기서 진행된다.
		const float _fullHealthValue; //전체 체력 외적으로 저장한다.
		float _hp;
		float _damage;

	};

	struct BaseMonsterHealthChangePair
	{
		BaseMonsterHealthChangePair(BaseMonsterInfo* mon, float hChangeLvl) : _baseMonster(mon), _healthChangeLvl(hChangeLvl) {}
		BaseMonsterInfo* _baseMonster;
		float _healthChangeLvl; //음수로 되어야 한다.
	};

	struct BaseMonsterHitPair
	{
		BaseMonsterHitPair(BaseMonsterInfo* mon) : _baseMonster(mon) {}
		BaseMonsterInfo* _baseMonster;
	};
}


