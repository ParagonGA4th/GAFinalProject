#pragma once
#include "ScriptInterface.h"
#include "GameState.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <unordered_map>
#include <vector>

/// <summary>
/// 기존 내용은 InGameManager로 이전, 얘는 Total GameFlow를 관리한다.
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class IAreaHandler;
	class IEnemyHandler;
	class IGUIHandler;
	class PlayerBattleBehavior;

	class TotalGameManager : public ScriptInterface<TotalGameManager>
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(TotalGameManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		//Scene이 바뀔 때
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;
	private:
		Pg::API::PgScene* _pgScene{ nullptr };


	public:
		//개별적인 Struct 관리. Flow 관련된 구조체들을 묶어 놓은 것이다.
		//모두 다 개별 3D Scene에 개별적으로 존재해야 하는 애들이다.
		struct HandlerBundle
		{
			IAreaHandler* _areaHandler = nullptr;
			IEnemyHandler* _enemyHandler = nullptr;
			IGUIHandler* _guiHandler = nullptr;
			PlayerBattleBehavior* _playerBehavior = nullptr;
		};

	private:
		//Flow 관리를 위해 보관하는 정보.
		std::unordered_map<Pg::Data::Scene*, HandlerBundle> _scene3dHandlerBundleMap;
	};
}

