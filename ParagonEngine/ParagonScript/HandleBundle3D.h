#pragma once

///3D 씬 정보 관리를 위해서.

namespace Pg::DataScript
{
	class IAreaHandler;
	class IEnemyHandler;
	class IGUIHandler;
	class PlayerBattleBehavior;

	//개별적인 Struct 관리. Flow 관련된 구조체들을 묶어 놓은 것이다.
	//모두 다 개별 3D Scene에 개별적으로 존재해야 하는 애들이다.

	struct HandlerBundle3D
	{
		IAreaHandler* _areaHandler = nullptr;
		IEnemyHandler* _enemyHandler = nullptr;
		IGUIHandler* _guiHandler = nullptr;
		PlayerBattleBehavior* _playerBehavior = nullptr;
	};
}
