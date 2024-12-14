#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "IEnemyBehaviour.h"
#include "BaseMonster.h"
#include <functional>
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// Normal / Fire / Ice�� ��� ����� ���̴�.
/// </summary>
namespace Pg::Util
{
	class Tween;
}

namespace Pg::Data
{
	class StaticMeshRenderer;
	class BoxCollider;
	class PhysicsCollision;
}

namespace Pg::API
{
	namespace Time { class PgTime; } 
	namespace Tween { class PgTween; }
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
	class ArrowLogic : public ScriptInterface<ArrowLogic>, public IProjectile
	{
		DEFINE_PARAGON_SCRIPT(ArrowLogic);

	public:
		inline static const float ARROW_ATTACK_POWER = 8.f;
		inline static const float FIREARROW_ATTACK_POWER = 15.f;
		inline static const float ICEARROW_ATTACK_POWER = 15.f;
		//inline static const int TRAIL_DIVIDED_COUNT = 2; //�� ������ ������ ����.

	public:
		ArrowLogic(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void CleanOnSceneChange() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		//virtual void OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override;


	public:
		//Material�� �� Property ������ ���Ҿ� �ٸ��� ��ϵǾ�� �Ѵ�.
		//FitArrow�� ����, ���� Material�� ��� �ٸ��� �������.
		BEGIN_VISITABLES(ArrowLogic);
		VISITABLE(int, _arrowType); // -1 : Ice / 0 : Normal / 1 : Fire.
		END_VISITABLES;

		//Serializers.
		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;
	public:
		//BattleBehavior ��ũ��Ʈ�� ���� ���� ������ �� �ְ� �ϱ�. 
		//�������� �Ҵ��ؼ� ���� �־��ش�.
		PlayerHandler* _playerBattleBehavior{ nullptr };

	public:
		void ResetState(); //���� ���� ����.
		bool GetIsNowShooting(); //���� ��� �ִ��� ����.
		void ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir); //�ڱ� �ڽ��� ȭ���� ���.

	private:
		void InitSelfAsNormalArrow();
		void InitSelfAsIceArrow();
		void InitSelfAsFireArrow();
	
	private:
		void InitTrailObjects();
		void TurnOffAllTrailObjects();
	private:
		void NormalArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex);
		void IceArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex);
		void FireArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex);
		
		std::function<void(Pg::DataScript::IEnemyBehaviour*, int)> _assignedDamageLogic;

	private:
		void EndShootingSelf(); //�� ��ٴ� ���. �߷��� Ű���� ����, �׳ɻ������.
		void CarryOutShoot(); //������ ���� ��� ����, Tweening ���.
		void IfValidActualShootLogic(); //���� �� ��� If��.
		void TrailUpdateLogic(); // Trail ���̴� ���� ������Ʈ


	private:	//�ΰ��� �� ������Ƽ��.
		//N�� ���Ŀ� ������!
		const float _afterDestroySec{ 3.0f };
		//�������� ���� �Ÿ�.
		const float _arrowDistBeforeFall{ 13.0f };
		// �̸� �ڱ� �ڽſ� ���� �� �ִ� Layer �����Ǿ� �־��.
		const float _secondsBeforeGravity{ 1.0f };

	private:	//ArrowShooterBehavior(�Ŵ���)�� ������ �����̴�.
		bool _isNowShooting = false;
		//�� ��� ����.
		bool _isJustInvokedShoot = false;

		int _trailActiveCount{ 0 };
		float _trailActiveTime{ 0.f };

	private:
		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;
		Pg::Math::PGFLOAT3 _initialPos;
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGFLOAT3 _shootDir;

	private:	
		//�ڽ��� ������Ʈ��.
		Pg::Data::StaticMeshRenderer* _meshRenderer;
		Pg::Data::BoxCollider* _collider;

	private:	
		//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;
		Pg::API::Graphics::PgGraphics* _pgGraphics;

		Pg::Util::Tween* _usingTween;
	private:
		//ComboSystem ���� �ֱ�.
		ComboSystem* _comboSystem{ nullptr };

		CombatSystem* _combatSystem{ nullptr };

		Pg::Data::VisualEffectRenderObject* _soleTrail;
	};
}


