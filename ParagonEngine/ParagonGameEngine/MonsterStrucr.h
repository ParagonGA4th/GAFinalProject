#pragma once

// ������ ����
enum class MonsterStatus
{
	ENTER,				// ���� �� �÷��̾� ����ġ�� ��
	IDLE,				// ���� ���� ������ ����

	CHASE,				// �÷��̾� ����

	DEAD,				// ���

	PATTERN_READY,		// ���� �غ�
	BASIC_ATTACK,		// �⺻ ����
	CORE_ATTACK,		// ��ų ���
	PATTERN_END			// ������ ���� ���
};