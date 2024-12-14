#pragma once
#include "IObserver.h"
#include "IScriptResettable.h"
//#include "EnemyDefinesAndStructs.h"
#include "PartialAttackType.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include <functional>

///모든 몬스터가 상속받아야 할 베이스 클래스.
///2024.06.10 기점으로 인터페이스로 사용하기로 변경.

//ScriptResettable은 밑에서 따로 구현해야 하는데, 
//일단 상속받게 하고 현 클래스에서는 자체 assert false를 발동해주자.

namespace Pg::API
{
	namespace Time { class PgTime; }
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class BaseMonsterInfo : public IObserver, public IScriptResettable
	{
	public:
		BaseMonsterInfo(float fullHealth, float attackPower);
		~BaseMonsterInfo();

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		void StartBaseMonsterLogic(); //Start 내부에
		void OnHitEnableHitEffect(int hitType); // Hit() 내부에
		void UpdateBaseMonsterLogic(Pg::Data::GameObject* obj); // Update() 내부에

		void SetMonsterHp(float hp);

		//몬스터의 체력을 깎을 떄.
		void ChangeMonsterHp(float level);

		void SetMonsterDamage(float damage);

		float GetMonsterHp();
		float GetMonsterDamage();

		bool IsMonsterDead();

		bool IsEffectValid() { return _ptrEffectFrameIndex != nullptr; }

		//이는 모든 Monster가 이동할 때, 고려해야 할 Factor이다.
		void SetMonsterSpeedRatio(float speed);
		float GetMonsterSpeedRatio(); 

		//사망 시 발생하는 함수.
		std::function<void()> _onDead;

		//피격 시 발생하는 함수.
		std::function<void()> _onHit;
	protected:
		//체력 / 공격력 관리는 여기서 진행된다.
		const float _fullHealthValue; //전체 체력 외적으로 저장한다.
		float _hp;
		float _damage;

		//Speed Multiplier. 개별 Behaviour들이 모두 얘를 기반으로 이동 곱해야 한다.
		float _speed{ 1.0f };

	private:
		bool _isActivatedEffect{ false };
		float _isActivatedTime{ 0.f };

		Pg::Data::VisualEffectRenderObject* _hitVO;
		Pg::Data::VisualEffectRenderObject* _fireHitVO;
		Pg::Data::VisualEffectRenderObject* _iceHitVO;

		Pg::Data::VisualEffectRenderObject* _chosenVO;
		unsigned int* _chosenPtrEffect{ nullptr };

		unsigned int* _ptrEffectFrameIndex{ nullptr };
		unsigned int* _ptrEffectFireFrameIndex{ nullptr };
		unsigned int* _ptrEffectIceFrameIndex{ nullptr };

		const float HIT_EFFECT_LASTING_TIME = 0.5f;

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics;
		Pg::API::Time::PgTime* _getPgTime;
	};

	struct BaseMonsterHealthChangePair
	{
		BaseMonsterHealthChangePair(BaseMonsterInfo* mon, float hChangeLvl, ePartialAttackType partType) : 
			_baseMonster(mon), _healthChangeLvl(hChangeLvl), _partialAttackType(partType) {}
		BaseMonsterInfo* _baseMonster;
		float _healthChangeLvl; //음수로 되어야 한다.
		ePartialAttackType _partialAttackType; //마나 계산을 위해.
	};

	struct BaseMonsterHitPair
	{
		BaseMonsterHitPair(BaseMonsterInfo* mon) : _baseMonster(mon) {}
		BaseMonsterInfo* _baseMonster;
	};

	//Fire / Ice 데미지에 활용될 것이다.
	//총 5초간, 1초마다 1 데미지가 들어간다.
	//죽었으면 당연히 캔슬.
	//막 맞았을 때 피격 존재. Immediate + 도트뎀.
	struct FireEffect_MonsterHitPair
	{
		inline static const float DOT_DAMAGE = 3.0f;
		inline static const float REMAINING_TIME = 3.0f;

		FireEffect_MonsterHitPair(BaseMonsterInfo* mon) : 
			_baseMonster(mon), _remainingTime(REMAINING_TIME), _roundingNum(REMAINING_TIME) {}
		BaseMonsterInfo* _baseMonster;
		float _remainingTime; //남은 시간 실시간 카운팅.
		int _roundingNum; //N번 남았다.
	};
	
	//3초간, 원래 속도를 1로 둘 때, 0.5배의 속도로 CombatSystem이 해줘야.
	struct IceEffect_MonsterHitPair
	{
		inline static const float REMAINING_TIME = 3.f;
		inline static const float SLOW_SPEED_MULTIPLIER = 0.5f;
		inline static const float NORMAL_SPEED_MULTIPLIER = 1.0f;

		IceEffect_MonsterHitPair(BaseMonsterInfo* mon) : 
			_baseMonster(mon), _remainingTime(REMAINING_TIME) {}
		BaseMonsterInfo* _baseMonster;
		float _remainingTime;
	};

	
}


