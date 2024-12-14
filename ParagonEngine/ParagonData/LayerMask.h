#pragma once

namespace Pg::Data::Enums
{
	enum eLayerMask : unsigned int
	{
		LAYER_DEFAULT = 0,			// �⺻ ����.
		LAYER_PLAYER = 1,			// �÷��̾��� ���̾�
		LAYER_MONSTER = 2,			// ����, ������ ���̾�
		LAYER_PROJECTILES = 3,		// ���ƴٴϴ� ����ü���� Area.
		LAYER_MAP = 4,				// �� ���(����, ���� ��)
		LAYER_BACKGROUND = 5,		// �� ���
		LAYER_MOVABLE_OBJECTS = 6,	// ������ �� �ִ� �� ���.
		LAYER_BOSS = 7,

		SELECT_NONE = 16 //�̴� ������ ���Ǿ�� �ȵȴ�. Raycast Filtering�� ����.
	};
}