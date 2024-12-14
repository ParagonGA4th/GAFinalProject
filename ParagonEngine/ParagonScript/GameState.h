#pragma once

namespace Pg::DataScript
{
	enum class eGameState
	{
		_NOTSTARTED,	// ������ ó�� ���� �Ǿ��� ��
		_PLAY,			// ������ ���� ���� ��
		_PAUSE,			// ������ ������ ��
		_STOP,			// ������ ������ ��
	};

	
	enum class ePlayerState
	{
		_NONE,		// �÷��̾ �������� ���� ��(������ �÷��� ���� �ʾ��� ��)
		_LIVE,		// �÷��̾ ������� ��
		_DIE,		// �÷��̾ �׾��� ��
	};
}