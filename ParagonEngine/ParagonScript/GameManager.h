#pragma once
#include "ScriptInterface.h"
#include "GameState.h"

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
		// 현재 Scene Info -> name 정도
		// Scene Info
		// Scene Name
		// {
		//	1. Main
		//  2. GameStage_1
		//  3. GameStage_2
		//  4. GameStage_3
		//  5. Ending
		// }

		// 현재 게임이 실행 중인지 아닌지(Pause / ReStart / End)
		// Play
		// pause
		// stop
		eGameState _gameState;

		// Player state 
		// live
		// die

	};
}

