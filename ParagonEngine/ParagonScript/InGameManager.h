#pragma once

#pragma once
#include "ScriptInterface.h"
#include "GameState.h"
#include "HandleBundle3D.h"

#include "../ParagonData/ISortableGlobalObject.h"

#include <string>
#include <vector>

/// <summary>
///	Total이 플로우 관리라면, InGameManager는 
/// 컴뱃 등 게임 내부를 총괄해서 실행한다.
///  
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API::Input { class PgInput; }
namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class TotalGameManager;

	class InGameManager : public ScriptInterface<InGameManager>, public Pg::Data::ISortableGlobalObject
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(InGameManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;

		//Scene이 바뀔 때 Global Object만 루프로 호출됨.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;
		
		//TotalGameManager 다음으로 호출.
		virtual unsigned int GetPriorityIndex() override { return 1; }

	public:
		bool GetEnableGameManagerUpdate();

	private:
		//Initialize.
		void Initialize(Pg::Data::Scene* changedScene);

		// Logic 업데이트, 3D일 경우에.
		// 밖에서 들어올지 말지 결정.

	private:
		float _gameTime;				// 게임 내 시간
		eGameState _gameState;			// 게임 상태 
		//std::string _currentSceneName;	// 현재 씬
		//std::vector<std::string> _sceneList; // 씬들의 목록

		/// Player
		int _playersLife;				// 플레이어의 목숨
		ePlayerState _playerState;		// 플레이어의 상태
		std::vector<std::pair<std::string, bool>> _artifacts;	// 플레이어가 보유 중인 아티팩트

		/// Stage
		int _numberOfMonstersKilled;	// 죽은 몬스터의 개수
		
	
		

	private:
		//전체 값이 받아졌는지 -> Initialize를 초기에 하기 위해서.
		bool _isManagingInitializeCalled{ false };

		//TotalGameManager에 의한 Update Mode Set.
		bool _isUpdating{ false };

	private:
		//API
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };

		//Total Game Manager 등, 기타 매니저들 보관.
		TotalGameManager* _totalGameManager{ nullptr };

		//현재 3D일 경우, 무조건 nullptr가 아님. 아닐 경우는 nullptr 맞다.
		HandlerBundle3D* _handlerBundle3D{ nullptr };

	};
}


