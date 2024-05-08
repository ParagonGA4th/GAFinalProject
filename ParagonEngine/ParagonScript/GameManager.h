#pragma once
#include "ScriptInterface.h"
#include "GameState.h"

#include <string>
#include <vector>

namespace Pg::API::Time { class PgTime; }

namespace Pg::DataScript
{
	class GameManager : public ScriptInterface<GameManager>
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(GameManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		void ResetGame();

	private:
		float _gameTime;				// 게임 내 시간
		eGameState _gameState;			// 게임 상태 
		std::string _currentSceneName;	// 현재 씬
		std::vector<std::string> _sceneList; // 씬들의 목록

		/// Player
		int _playersLife;				// 플레이어의 목숨
		ePlayerState _playerState;		// 플레이어의 상태
		std::vector<std::pair<std::string, bool>> _artifacts;	// 플레이어가 보유 중인 아티팩트
		
		/// Stage
		int _numberOfMonstersKilled;	// 죽은 몬스터의 개수
		// SceneName, monsterType, monsters Number
		std::vector<std::pair<std::string, std::pair<std::string, int>>> _numberOfMonsters;	// 스테이지 별 몬스터의 개수
	};
}

