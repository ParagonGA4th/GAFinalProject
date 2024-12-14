#pragma once
#include "ScriptInterface.h"
#include "IScriptResettable.h"
#include "../ParagonData/ISortableGlobalObject.h"
#include "IEvent.h"
#include "ScriptEventHelper.h"
#include "BaseMonster.h"
#include "EnemyDefinesAndStructs.h"
#include <functional>
#include <string>
#include <variant>
#include <cassert>

/// ������ ������Ʈ�� ����ϴ� Ŭ�������� ���漱���� ���� ��.
namespace Pg::DataScript
{
	class PlayerHandler;
	class CombatSystem;
	class BaseMonster;
	struct HandlerBundle3D;
	class IEnemyBehaviour;
	class IProjectile;
}

namespace Pg::API { class PgScene; }
namespace Pg::API 
{ 
	namespace Time
	{
		class PgTime;
	}
	
}

/// <summary>
/// ���� �ȿ��� �Ĺ��� �����ϴ� �ý���. �̱��� ��ũ��Ʈ�� �����ȴ�.
/// ������ Event Manager ���� + ������� �ϵ��ڵ� �Ҵ����� ��� �� ��. (�̱�-�����常 ����)
/// Reference : https://bastian.rieck.me/blog/2015/event_system_cxx11/
/// ����μ��� ��ũ��Ʈ�μ� ����� ������Ʈ�� ����.
/// </summary>

namespace Pg::DataScript
{
	class CombatSystem : public ScriptInterface<CombatSystem>, public Pg::Data::ISortableGlobalObject, public IScriptResettable
	{
		//�� ��ũ�θ� ���ٸ�, �����ڸ� �ڴ�� ���� �ȵȴ�.
		//�ٸ� �����ڿ� �ڴ�� Singleton CombatSystem �������� �ȵ�!
		//����, DontDestroyOnLoad ������Ʈ�� �����ٰ� ��� �� ��.
		//Title Scene ���ο� ���ԵǾ� �־�� �Ѵ�.
		DEFINE_PARAGON_SCRIPT_SINGLETON(CombatSystem);
	
	public:
		inline static const float INCREMENT_MANA_ONHIT_AMT = 5.f;

	public:
		virtual void Start() override;

		// ��κ��� ������ FixedUpdate()���� Ȱ�������� ���ڴ�. 
		// Update���� ������ ó���ϰ� - FixedUpdate���� CombatSystem - 
		// LateUpdate���� ���� ������Ʈ�� �̿� �´� ó���� ���ش�.
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;

		//������������, ���� ���� �� �����ٰ� ������ �Ѵ�.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;

		//������ �����͵��� ���� �����Ѵ�.
		virtual void ResetAll() override;

	public:
		//����μ��� ���� ������ Priority.
		virtual unsigned int GetPriorityIndex() override { return 2; }

	public:
		//�ܺ� �ڵ鷯���� �� �Լ��� ȣ���� �� �ְ� �Ѵ�.
		//�̴� ��ü ������ �ƴ� �̺�Ʈ ����� ����.
		//���������� Subscribe�ص� ������, ���� �Ϻ� ��Ҵ� ���ϰ� �۵���ų �� �ְ� �Ѵ�.
		//�� �Լ��� ���� �÷��̾ Combat System���� ������Ų��. 
		void RegisterPlayer(PlayerHandler* pl);

		//���⿡�� Monster���� ���� BaseClass / Interface�� �����ؾ� �Ѵ�.
		void RegisterSingleEnemy(IEnemyBehaviour* enemy);

		//���� �ȿ��� ���̴� ��� ����ü���� ���� BaseClass / Interface�� �����ؾ� �Ѵ�.
		void RegisterSingleProjectile(IProjectile* proj);
	
	public:
		// Player�� ������Ƽ�� ���� �����ϴ� Wrapper�̴�. ������ Combat System�� ���ؼ� ���� ������ �����ؾ�
		// ������ �ʰ� �� ���̴�.
		//�÷��̾�� ������ �������� ������ ���� �и���.
		void ChangePlayerHealth(float level, bool isTrap = false);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(float level);
		void ChangePlayerLife(int level);

		float GetPlayerHealth();
		float GetPlayerMana();
		unsigned int GetPlayerStamina();

		
	public:
		//�������� Combat ���.
		//Monster Script���� ���������� ȣ���ϴ� �Լ�.
		//Normal / Strong / Ultimate ��� Ȱ��.
		void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl, ePartialAttackType partType);
		void AddMonsterOnHitList(BaseMonsterInfo* monster);

		//IceArrow ���.
		void AddMonsterIceDamageList(BaseMonsterInfo* monster);
		//FireArrow ���.
		void AddMonsterFireDamageList(BaseMonsterInfo* monster);

		//�� �����Ӹ��� ���������� �ʴ´�.
		//�ٸ�, MonsterInfo�� 
	private:
		//�� �����Ӹ��� clear.
		std::vector<BaseMonsterHealthChangePair> _monsterHealthChangeList;
		std::vector<BaseMonsterHitPair> _monsterOnHitList;

		std::list<IceEffect_MonsterHitPair> _iceMonsterHealthChangeList;
		std::list<FireEffect_MonsterHitPair> _fireMonsterHealthChangeList;

		HandlerBundle3D* _currentHandlerBundle3D{ nullptr };
	private:
		void CalculateMonsterDamages();
		void CalculateIceDamages();
		void CalculateFireDamages();
		void CalculateMonsterHit();

		

	private:
		//��ü ���� �޾������� -> Initialize�� �ʱ⿡ �ϱ� ���ؼ�.
		bool _isManagingInitializeCalled{ false };

		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };

		//Initialize.
		void Initialize(Pg::Data::Scene* changedScene);


		//���� Handler�� ������� �� �̻��� �ʿ�X.
		//private:
		//	PlayerHandler* _player{ nullptr };
		//	std::vector<Pg::Data::GameObject*> _monsterList;
		//	std::vector<Pg::Data::GameObject*> _projectileList;


	public: //Combat System�� ������ �̺�Ʈ �ý���ó�� �����ϴ� �κ��̴�.
		
		//�̺�Ʈ�� ȣ��� ������, �ش� �ĺ��ڸ� �ް�, ���� ������ Observer ��ü�� ȣ��.
		void Subscribe(const std::string& descriptor, SlotType&& slot, bool isGlobal = false);

		//�ڽſ� ���� �ִ� ��� �ɼ��������� �̺�Ʈ���� ���� �� �ִ�.
		void Post(const IEvent& eventVal, UsedVariant usedVariant1, UsedVariant usedVariant2) const;

	private:
		std::map<std::string, std::vector<SlotType>> _observers;
		std::map<std::string, std::vector<SlotType>> _globalObservers; //InGameManager ���� ���µ��� �ʴ� ������ ���, �ٸ��� �����Ǿ��.
	};
}



