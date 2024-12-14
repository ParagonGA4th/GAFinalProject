#pragma once
#include "Component.h"
#include "MonsterFlag.h"


/// <summary>
/// 24.05.20 �ֹμ�
/// script�� �ִ� ���� ���� �������� ���� helperClass
/// BTNode�� ���� ����
/// </summary>

namespace Pg::Data
{
	class MonsterHelper : public Component
	{
	public:
		// �ӽ� �⺻������
		MonsterHelper() = default;
		MonsterHelper(GameObject* obj);

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;
		void Reset();

	private:
		void BossInit();
		void GolemBossInit();
		void GolemInit();
		void StubInit();
		void MimicInit();
		void WaspInit();

	public:
		/// Public
		bool _isPlayerDetected;			// �÷��̾ �þ� �ȿ� �ִ���
		bool _isPlayerinHitSpace;		// �÷��̾ ���� ���� �ȿ� �ִ���
		bool _isDistanceClose;			// �÷��̾�� �Ÿ��� �������(��ų�� ���� ���� �Ÿ�)
		bool _isChase;					// ���Ͱ� �÷��̾ �i�� �ִ���
		bool _isHit;					// ���Ͱ� ���� �޾Ҵ���
		bool _isDead;					// ���Ͱ� �׾�����

		/// Animation
		bool _isAnimationEnd;			// �ִϸ��̼��� ��������
		bool _isAnimChange;				// �ִϸ��̼��� �ٲ������
		bool _isDeadDelay;				// ���Ͱ� ���� �� ������

		/// Monsters
		Pg::Data::MonsterState _state;	// ������ ���� ���͵��� state

		Pg::Data::Boss _bossFlag;
		Pg::Data::GolemBoss _bGolemFlag;
		Pg::Data::MiniGolem _mGolemFlag;
		Pg::Data::Stub _stubFlag;
		Pg::Data::Mimic _mimicFlag;
		Pg::Data::Wasp _waspFlag;
	};
}

