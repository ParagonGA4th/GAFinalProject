#pragma once
#include "IObserver.h"
#include "IScriptResettable.h"
//#include "EnemyDefinesAndStructs.h"
#include "PartialAttackType.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include <functional>

///��� ���Ͱ� ��ӹ޾ƾ� �� ���̽� Ŭ����.
///2024.06.10 �������� �������̽��� ����ϱ�� ����.

//ScriptResettable�� �ؿ��� ���� �����ؾ� �ϴµ�, 
//�ϴ� ��ӹް� �ϰ� �� Ŭ���������� ��ü assert false�� �ߵ�������.

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

		//IObserver : ��ü���� Event ������ �������� �۵��� ��.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. �ٽ� �ڱ� �ڽ��� �����ϴ� �Լ�.
		virtual void ResetAll() override;

		void StartBaseMonsterLogic(); //Start ���ο�
		void OnHitEnableHitEffect(int hitType); // Hit() ���ο�
		void UpdateBaseMonsterLogic(Pg::Data::GameObject* obj); // Update() ���ο�

		void SetMonsterHp(float hp);

		//������ ü���� ���� ��.
		void ChangeMonsterHp(float level);

		void SetMonsterDamage(float damage);

		float GetMonsterHp();
		float GetMonsterDamage();

		bool IsMonsterDead();

		bool IsEffectValid() { return _ptrEffectFrameIndex != nullptr; }

		//�̴� ��� Monster�� �̵��� ��, ����ؾ� �� Factor�̴�.
		void SetMonsterSpeedRatio(float speed);
		float GetMonsterSpeedRatio(); 

		//��� �� �߻��ϴ� �Լ�.
		std::function<void()> _onDead;

		//�ǰ� �� �߻��ϴ� �Լ�.
		std::function<void()> _onHit;
	protected:
		//ü�� / ���ݷ� ������ ���⼭ ����ȴ�.
		const float _fullHealthValue; //��ü ü�� �������� �����Ѵ�.
		float _hp;
		float _damage;

		//Speed Multiplier. ���� Behaviour���� ��� �긦 ������� �̵� ���ؾ� �Ѵ�.
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
		float _healthChangeLvl; //������ �Ǿ�� �Ѵ�.
		ePartialAttackType _partialAttackType; //���� ����� ����.
	};

	struct BaseMonsterHitPair
	{
		BaseMonsterHitPair(BaseMonsterInfo* mon) : _baseMonster(mon) {}
		BaseMonsterInfo* _baseMonster;
	};

	//Fire / Ice �������� Ȱ��� ���̴�.
	//�� 5�ʰ�, 1�ʸ��� 1 �������� ����.
	//�׾����� �翬�� ĵ��.
	//�� �¾��� �� �ǰ� ����. Immediate + ��Ʈ��.
	struct FireEffect_MonsterHitPair
	{
		inline static const float DOT_DAMAGE = 3.0f;
		inline static const float REMAINING_TIME = 3.0f;

		FireEffect_MonsterHitPair(BaseMonsterInfo* mon) : 
			_baseMonster(mon), _remainingTime(REMAINING_TIME), _roundingNum(REMAINING_TIME) {}
		BaseMonsterInfo* _baseMonster;
		float _remainingTime; //���� �ð� �ǽð� ī����.
		int _roundingNum; //N�� ���Ҵ�.
	};
	
	//3�ʰ�, ���� �ӵ��� 1�� �� ��, 0.5���� �ӵ��� CombatSystem�� �����.
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


