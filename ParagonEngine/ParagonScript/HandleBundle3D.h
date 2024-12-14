#pragma once
#include "TransformSimpleStorage.h"
///3D �� ���� ������ ���ؼ�.

namespace Pg::DataScript
{
	class BaseAreaHandler;
	class BaseEnemyHandler;
	class BaseGUIHandler;
	class PlayerHandler;

	//�������� Struct ����. Flow ���õ� ����ü���� ���� ���� ���̴�.
	//��� �� ���� 3D Scene�� ���������� �����ؾ� �ϴ� �ֵ��̴�.

	struct HandlerBundle3D
	{
		BaseAreaHandler* _areaHandler = nullptr;
		BaseEnemyHandler* _enemyHandler = nullptr;
		BaseGUIHandler* _guiHandler = nullptr;
		PlayerHandler* _playerBehavior = nullptr;
		TransformSimpleStorage _originalPlayerTransStorage;
	};
}
