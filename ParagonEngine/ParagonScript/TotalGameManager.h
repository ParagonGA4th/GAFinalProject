#pragma once
#include "ScriptInterface.h"
#include "GameState.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

/// <summary>
/// 기존 내용은 InGameManager로 이전, 얘는 Total GameFlow를 관리한다.
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API::Input { class PgInput; }
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

		//Scene이 바뀔 때 Global Object만 루프로 호출됨.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;


	private:
		void Initialize(Pg::Data::Scene* changedScene);
		void SetupBundlesForAllScenes();
		void SetCurrentHandlerBundle(Pg::Data::Scene* changedScene);


	private:
		//하드코딩된 치트 함수들. '\' + 콤비네이션으로 호출할 수 있게.
		//버튼 하나로 호출되지는 않을 것이다.
		void SelectCheatCodeWithin();


	private:
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };

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
		//Flow 관리를 위해 보관하는 정보. (3D)
		std::unordered_map<Pg::Data::Scene*, std::unique_ptr<HandlerBundle>> _scene3dHandlerBundleMap;
		//Flow 관리를 위해 보관하는 정보. (2D)
		std::unordered_map<Pg::Data::Scene*, IGUIHandler*> _scene2dHandlerBundleMap;

		//2D/3D 보관에 따라 nullptr 할당이 달라진다.
		HandlerBundle* _currentHandlerBundle3d{ nullptr }; //3D의 경우 nullptr면 안됨, 2D면 nullptr여야.
		IGUIHandler* _currentGUIHander2d{ nullptr }; //3D의 경우 nullptr여야, 2D면 nullptr면 안됨..

		//전체 값이 받아졌는지 -> 
		bool _isManagingCalled{ false };
	};
}

