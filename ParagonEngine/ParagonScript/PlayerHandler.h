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
		//������.
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		inline static const float PLAYER_HEALTH_LOW_BARRIER = 20.0f;
		inline static const float MAX_PLAYER_MANA = 100.0f; // Mana�� 100�� �ִ�.
		inline static const int MAX_PLAYER_STAMINA = 3;
		inline static const float STAMINA_ONE_SLOT_CHARGE_TIME = 20.0f;
		inline static const int MAX_ARTIFACT = 3;			// ��Ƽ��Ʈ ������ �ִ� 3��
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

		//IObserver : ��ü���� Event ������ �������� �۵��� ��.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. �ٽ� �ڱ� �ڽ��� �����ϴ� �Լ�.
		virtual void ResetAll() override;

		//Animation�� ������ �� ȣ�� �Ǵ� �Լ�
		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

		//�̰� CombatSystem�� �־��.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);

		//CPP������ ���簡 �����־���.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		void SetPlayerMoveSpeed(float val);
		float GetPlayerMoveSpeed();

	private:
		//�÷��̾�� ������ �������� ������ ���� �и���.
		void ChangePlayerHealth(float level, bool isTrap = false);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(int level);

		int GetPlayerLife();
		void ChangePlayerLife(int level);

		void CreateEffectObjects();

	public:
		//ü�� ����. -> ������ �� ��Ͷ����� �̷��� �س�����, ���߿��� ������ �̵��ؾ�.
		//�ְ� ü���� �ϴ� 100�̴�.
		float healthPoint{ MAX_PLAYER_HEALTH };
		float manaPoint{ MAX_PLAYER_MANA };
		int staminaPoint{ MAX_PLAYER_STAMINA };

		//�÷��̾��� ������.
		//�������ų� ü���� 0�̸� �ϳ��� ��������.
		int _playerlife{ MAX_LIFE_COUNT };

		/// ��Ƽ��Ʈ ����
	public:
		int artifactCount{ 0 }; // �ʱ� ��Ƽ��Ʈ ������ 0��
	private:
		Pg::Data::ImageRenderer* _imgRenderer;

	public:
		//Sector ������ ������ ����.
		//���߿��� ��� �ּ�ȭ�� ��������.
		PlayerMovementSector* GetPlayerMovementSector();
		PlayerCombatSector* GetPlayerCombatSector();
		Pg::Data::CapsuleCollider* GetPlayerSelfCol();
		Pg::Data::AudioSource* GetUltimateSkillAudio() { return _ultimateSkillAudio; }

	public:
		//GUI�� �����ֱ� ����, ������ �Ұ����� �����͸� ��ȯ�ϰ� �Ѵ�.
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

		//Stamina ���õ� ���� (ȸ�� / ������)
		//�� ���Ͱ� �����ؼ� ��� �Ѵ�.
		bool _isStaminaReadyToUse{ true };
		bool _shouldStaminaCharge{ false };
		float _staminaCountingTime{ 0.f };

	private:
		//���������� Movement �� Sector�� ����.
		std::unique_ptr<PlayerMovementSector> _playerMovementSector;
		std::unique_ptr<PlayerCombatSector> _playerCombatSector;

		Pg::Data::CapsuleCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };

		//���� ���� ����
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
		//���� ������ ���� ȿ�� 
		//�����ϰ� �־�� �Ѵ�.

	};
}


