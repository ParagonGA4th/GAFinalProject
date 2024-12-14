#pragma once
#include "BaseMonster.h"
#include "IEvent.h"

//��ġ�� ����
enum class BossStatus
{
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����
	DASH,				// ����
	EVADE,				// ȸ��

	DEAD,				// ���

	BASIC_ATTACK,		// �⺻ ����
};

/// <summary>
/// ��ġ�� ����
/// </summary>
namespace Pg::DataScript
{
	class BossInfo : public BaseMonsterInfo
	{
	public:
		BossInfo(float fullHealth, float attackPower);

		//�÷��̾���� �̺�Ʈ�� �߻��� �� ���� �ȴ�.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		virtual void ResetAll() override;

		BossInfo& SetArmor(float armor) { _armor = armor; return *this; }
		BossInfo& SetAttackRange(float attackRange) { _attackRange = attackRange; return *this; }
		BossInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; return *this; }
		BossInfo& SetCurrentDashTime(float time) { _cureentDashTime = time; return *this; }
		BossInfo& SetCurrentEvadeTime(float time) { _currentEvadeTime = time; return *this; }
		BossInfo& SetSightRange(float range) { _sightRange = range; return *this; }
		BossInfo& SetCurrentNeutralize(float time) { _currentNeutralize = time; return *this; }
		BossInfo& SetEndNeutralize(float time) { _endNeutralize = time; return *this; }
		BossInfo& SetWindBlastDuration(float time) { _windBlastDuration = time; return *this; }
		BossInfo& SetWindBlastSpeed(float speed) { _windBlastSpeed = speed; return *this; }
		BossInfo& SetStartWindBlastDurationTime(float time) { _startWindBlastTime = time; return *this; }
		BossInfo& SetCurrentWindBlastDurationTime(float time) { _currentWindBlastTime = time; return *this; }
		BossInfo& SetLightSkillDurationTime(float time) { _lightSkillDuration = time; return *this; }
		BossInfo& SetStartLightSkillTime(float time) { _startLightSkillTime = time; return *this; }
		BossInfo& SetCurrentLightSkillTime(float time) { _currentLightSkillTime = time; return *this; }

		float GetMoveSpeed() const { return _moveSpeed; }
		float GetDashSpeed() const { return _dashSpeed; }
		float GetEvadeSpeed() const { return _evadeSpeed; }
		float GetSightRange() const { return _sightRange; }
		float GetAttackRange() const { return _attackRange; }
		float GetDashRange() const { return _dashRange; }
		float GetDashDuration() const { return _dashDuration; }
		float GetEvadeDuration() const { return _evadeDuration; }
		float GetCurrentDashTime() { return _cureentDashTime; }
		float GetCurrentEvadeTime() { return _currentEvadeTime; }
		float GetCurrentNeutralize() { return _currentNeutralize; }
		float GetEndNeutralize() { return _endNeutralize; }
		float GetWindBlastDuration() { return _windBlastDuration; }
		float GetWindBlastSpeed() { return _windBlastSpeed; }
		float GetStartWindBlastTime() { return _startWindBlastTime; }
		float GetCurrentWindBlastTime() { return _currentWindBlastTime; }
		float GetLightSkillDuration() { return _lightSkillDuration; }
		float GetStartLightSkillTime() { return _startLightSkillTime; }
		float GetCurrentLightSkillTime() { return _currentLightSkillTime; }

		BossStatus _status; // ����

	private:
		float _armor;          // ����

		float _sightRange;		//�þ� ����
		float _attackRange;    // �⺻ ���� ����
		float _dashRange;		//���� ����
		float _moveSpeed;      // �̵��ӵ�
		float _rotationSpeed;  // ȸ�� �ӵ�
		float _dashSpeed;		// ���� �ӵ�
		float _evadeSpeed;		// ȸ�� �ӵ�

		float _dashDuration;	//���� ���� �ð�
		float _cureentDashTime; //���� ���� �ð�

		float _evadeDuration;	//ȸ�� ���� �ð�
		float _currentEvadeTime;//���� ȸ�� �ð�

		float _currentNeutralize; //����ȭ ���� �ð�
		float _endNeutralize; //����ȭ ���� �ð�

		float _windBlastDuration; //��ǳ ���� �ð�
		float _windBlastSpeed; //��ǳ �ӵ�
		float _startWindBlastTime; //��ǳ ���� �ð�
		float _currentWindBlastTime;

		float _lightSkillDuration; //�� ��ų ���� �ð�
		float _startLightSkillTime; //�� ��ų ���� �ð�
		float _currentLightSkillTime; //���� �� ��ų �ð�
	};
}


