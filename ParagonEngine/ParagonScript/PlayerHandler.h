#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"
#include "IScriptResettable.h"
#include "BaseMonster.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::DataScript
{
	class CombatSystem;
	class ComboSystem;

	class ArrowLogic;
	class PlayerMovementSector;
	class PlayerCombatSector;
}

namespace Pg::API
{
	namespace Input { class PgInput; };
	namespace Time { class PgTime; };
	namespace Graphics { class PgGraphics; }
	class PgScene;
}

namespace Pg::Data
{
	class DynamicCollider;
	class CapsuleCollider;
	class AudioSource;
	class SkinnedMeshRenderer;
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class PlayerHandler : public ScriptInterface<PlayerHandler>, public IObserver, public IScriptResettable
	{
		DEFINE_PARAGON_SCRIPT(PlayerHandler);

		friend class CombatSystem;
		friend class PlayerMovementSector;
		friend class PlayerCombatSector;

	public:
		//디파인.
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		inline static const float PLAYER_HEALTH_LOW_BARRIER = 20.0f;
		inline static const float MAX_PLAYER_MANA = 100.0f; // Mana는 100이 최대.
		inline static const int MAX_PLAYER_STAMINA = 3;
		inline static const float STAMINA_ONE_SLOT_CHARGE_TIME = 20.0f;
		inline static const int MAX_ARTIFACT = 3;			// 아티팩트 개수는 최대 3개
		inline static const int MAX_LIFE_COUNT = 3;
	public:
		PlayerHandler(Pg::Data::GameObject* obj);
		~PlayerHandler();

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		//Animation이 끝났을 때 호출 되는 함수
		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

		//이거 CombatSystem에 있어야.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);

		//CPP에서는 잔재가 남아있었다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		void SetPlayerMoveSpeed(float val);
		float GetPlayerMoveSpeed();

	private:
		//플레이어에게 들어오는 개별적인 로직은 따로 분리됨.
		void ChangePlayerHealth(float level, bool isTrap = false);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(int level);

		int GetPlayerLife();
		void ChangePlayerLife(int level);

		void CreateEffectObjects();

	public:
		//체력 관련. -> 지금은 맵 기믹때문에 이렇게 해놨지만, 나중에는 별도로 이동해야.
		//최고 체력은 일단 100이다.
		float healthPoint{ MAX_PLAYER_HEALTH };
		float manaPoint{ MAX_PLAYER_MANA };
		int staminaPoint{ MAX_PLAYER_STAMINA };

		//플레이어의 라이프.
		//떨어지거나 체력이 0이면 하나씩 없어진다.
		int _playerlife{ MAX_LIFE_COUNT };

		/// 아티팩트 관련
	public:
		int artifactCount{ 0 }; // 초기 아티팩트 개수는 0개
	private:
		Pg::Data::ImageRenderer* _imgRenderer;

	public:
		//Sector 사이의 교류를 위해.
		//나중에는 사용 최소화를 목적으로.
		PlayerMovementSector* GetPlayerMovementSector();
		PlayerCombatSector* GetPlayerCombatSector();
		Pg::Data::CapsuleCollider* GetPlayerSelfCol();
		Pg::Data::AudioSource* GetUltimateSkillAudio() { return _ultimateSkillAudio; }

	public:
		//GUI를 보여주기 위해, 수정이 불가능한 포인터를 반환하게 한다.
		const float* ReturnPlayerHealthPointPointerConst() const;
		const float* ReturnPlayerManaPointPointerConst() const;
		const int* ReturnPlayerStaminaPointPointerConst() const;

		bool GetIsStaminaReadyToUse();
		void InvokeHitEffect();
	private:
		void GetInternalVariables();
		void UpdateStamina();
		void UpdateVisualEffectObjects();
	private:
		CombatSystem* _combatSystem{ nullptr };
		ComboSystem* _comboSystem{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::PgScene* _sceneHelper{ nullptr };
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		unsigned int* _dustImagePointer;
		const unsigned int _groundDustMaxIndex{ 7 };
		unsigned int _dustCounterVar{ 0 };

		//Stamina 관련된 스탯 (회피 / 강공격)
		//두 섹터가 공유해서 써야 한다.
		bool _isStaminaReadyToUse{ true };
		bool _shouldStaminaCharge{ false };
		float _staminaCountingTime{ 0.f };

	private:
		//내부적으로 Movement 등 Sector들 관리.
		std::unique_ptr<PlayerMovementSector> _playerMovementSector;
		std::unique_ptr<PlayerCombatSector> _playerCombatSector;

		Pg::Data::CapsuleCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };

		//사운드 관련 변수
		Pg::Data::AudioSource* _commonAttackAudio;
		Pg::Data::AudioSource* _walkAudio;
		Pg::Data::AudioSource* _jumpAudio;
		Pg::Data::AudioSource* _avoidAudio;
		Pg::Data::AudioSource* _fireSkillAudio;
		Pg::Data::AudioSource* _iceSkillAudio;
		Pg::Data::AudioSource* _ultimateSkillAudio;
		Pg::Data::AudioSource* _hitAudio;
		Pg::Data::AudioSource* _dieAudio;

		Pg::Math::PGFLOAT3 _rendererOffSet = { 0.f, -1.f, 0.f };

		Pg::Data::VisualEffectRenderObject* _groundDustRO{ nullptr };
		Pg::Data::VisualEffectRenderObject* _hitRO{ nullptr };

		bool _isEffectPlayOnHit = false;
		float _effectPlayOnHitTime = 0.f;
		unsigned int* _chosenPtrEffect = nullptr;
		//무적 상태일 떄의 효과 
		//보관하고 있어야 한다.

	};
}


