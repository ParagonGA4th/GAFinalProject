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

		//BattleBehavior ��ũ��Ʈ�� ���� ���� ������ �� �ְ� �ϱ�. 
		//�������� �Ҵ��ؼ� ���� �־��ش�.
		PlayerHandler* _playerBattleBehavior{ nullptr };

	public:
		void CarryOutShoot(); //������ ���� ��� ����, Tweening ���.
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
		//��ų ������ �ð�.
		bool _isSkillEnd{ false };
		bool _ultimateSkill{ false };

		// �ִϸ��̼� ����
		bool _isAnimEnd{ false };
		inline static const float ALPHA_PERCENT = 1.f;

	private:

		bool _isSkillStart{ false };
		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;

		bool _isHitEffectAlive = false;
		float _effectCountTime = 0.f;


	private:	//�ڽ��� ������Ʈ��.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::StaticSphereCollider* _collider;

	private:	//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;

	private:
		//ComboSystem ���� �ֱ�.
		ComboSystem* _comboSystem{ nullptr };
		PlayerHandler* _playerHandler{ nullptr };
		CombatSystem* _combatSystem{ nullptr };

	private:
		//VO
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		Pg::Data::VisualEffectRenderObject* _voUltimateHitOnMonster{ nullptr };
	};
}



