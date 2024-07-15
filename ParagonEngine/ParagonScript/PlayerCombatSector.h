#pragma once
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/DynamicCollider.h"

#include "IObserver.h"
#include "IScriptResettable.h"

/// <summary>
/// (구) PlayerBattleBehavior가 담당하는 역할.
/// </summary>

namespace Pg::Data 
{
	class StaticSphereCollider;
}
namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
}

namespace Pg::DataScript
{
	class ArrowLogic;
	class UltimateArrowLogic;
}

namespace Pg::DataScript
{
	class PlayerHandler;

	class PlayerCombatSector : public IObserver, public IScriptResettable
	{
		friend class PlayerHandler;
	public:
		PlayerCombatSector(PlayerHandler* playerHandler);


		void GrabManagedObjects();

		//얘네들은 Component에 의해 작동되는 것 X, Script 자체에서 수명을 관리한다.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void OnAnimationEnd(const std::string& justEndedAnimation);

	public:
		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

	public:
		void ArrowShootingLogic();

		//CombatSystem으로 이동.
		////Monster Script들이 자의적으로 호출하는 함수.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);


		//플레이어 궁극기
		void ShootUltimateArrowLogic();
	private:
		void PlayAdequateAnimation();
		void FindAllArrowsInMap();
	private:
		PlayerHandler* _playerHandler;
		Pg::Data::GameObject* _object;
		std::vector<ArrowLogic*> _arrowVec;

		Pg::Data::GameObject* _ultimateArrow;
		Pg::Data::StaticSphereCollider* _ulArrowCol;
		UltimateArrowLogic* _ulArrowLogic;
	private:
		float _timeSinceLastShot = 0.f;
		//공격 쿨타임
		const float _shootCooldown = 0.6f;

	private:
		// 플레이어 애니매이션 관련 변수
		std::string _prevAnimationInput;
		bool _isHit;
		bool _useUltimateSkill{ false };
		int _hitCount = 0;

	private:
		Pg::API::Input::PgInput* _pgInput;
		Pg::API::Time::PgTime* _pgTime;
	};
}