#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
	class StaticSphereCollider;
	class PhysicsCollision;
}

namespace Pg::API
{
	namespace Time
	{
		class PgTime;
	}

	namespace Tween
	{
		class PgTween;
	}
	
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class ComboSystem;
	class CombatSystem;
}

namespace Pg::DataScript
{
	class UltimateArrowLogic : public ScriptInterface<UltimateArrowLogic>, public IProjectile
	{
		DEFINE_PARAGON_SCRIPT(UltimateArrowLogic);

	public:
		inline static const float ULTIMATE_ATTACK_POWER = 30.f;

	public:
		UltimateArrowLogic(Pg::Data::GameObject* obj);
		virtual void CleanOnSceneChange() override;

		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		//virtual void OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override;

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

		//BattleBehavior 스크립트를 갖고 로직 제어할 수 있게 하기. 
		//외적으로 할당해서 값을 넣어준다.
		PlayerHandler* _playerBattleBehavior{ nullptr };

	public:
		void CarryOutShoot(); //실제로 총을 쏘는 로직, Tweening 사용.
		void StartSkill();

		bool GetUltimateSkillEnd();
	private:
		void EndShootingSelf();
	
	private:
		//UpdateEffect;
		//StartEffect();
		void StartEffectHit();
		void UpdateEffect();

	private:
		//스킬 끝나는 시간.
		bool _isSkillEnd{ false };
		bool _ultimateSkill{ false };

		// 애니매이션 관련
		bool _isAnimEnd{ false };
		inline static const float ALPHA_PERCENT = 1.f;

	private:

		bool _isSkillStart{ false };
		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;

		bool _isHitEffectAlive = false;
		float _effectCountTime = 0.f;


	private:	//자신의 컴포넌트들.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::StaticSphereCollider* _collider;

	private:	//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;

	private:
		//ComboSystem 갖고 있기.
		ComboSystem* _comboSystem{ nullptr };
		PlayerHandler* _playerHandler{ nullptr };
		CombatSystem* _combatSystem{ nullptr };

	private:
		//VO
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		Pg::Data::VisualEffectRenderObject* _voUltimateHitOnMonster{ nullptr };
	};
}



