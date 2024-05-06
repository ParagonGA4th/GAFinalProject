#include "GameManager.h"

#include "../ParagonData/Scene.h"

#include <algorithm>

void Pg::DataScript::GameManager::Awake()
{
	_gameState = Pg::DataScript::eGameState::_NOTSTARTED;
}

void Pg::DataScript::GameManager::Start()
{
	_currentSceneName = _object->GetScene()->GetSceneName();
	_playerState = Pg::DataScript::ePlayerState::_LIVE;

	// awake만 한번 불리는지
	// start도 한번 불리는지
}

void Pg::DataScript::GameManager::Update()
{
	// portal
	// -> next scene name 전달 해주기

	// player
	// -> 체력에 따라 live/die 판별, 게임 상태 관리까지
	// -> player가 죽었을 경우 게임 내의 시간을 멈춘다. (몬스터의 공격도 멈춘다..?)
	// -> player의 위치는 곧 현재 씬

	// monster
	// -> 현재 씬 내에 몬스터가 몇 마리 있는지 판별
	// -> 스테이지 1의 경우 각 전투 구역 마다 몇 마리 있는지 판별

	// stage
	// -> 사운드. 각 스테이지 마다 사운드가 다를 수 있음. 또는 같을 수 있음. 끊어지지 않게 계속 들릴 수 있도록
	// -> 팝업. 스테이지 1의 경우, 튜토리얼과 비슷한 느낌이기에 팝업이 떠야 함. +) pause 팝업
	// -> 목숨 개수에 따라 다시 소환 될 경우 위치를 기억하고 있어야 한다.(몇 스테이지, 무슨 구역) 
	// ->> 구역을 나누지는 않을 것 같으니, 죽었다가 다시 소환 될 경우 스테이지만 복기 하는 것으로

	// etc
	// -> 아티팩트 관리
	// -> 게임 내 시간 관리
	// -> player의 목숨 관리(체력 말고 다시 되살아 날 수 있는 목숨 -> 3개)
	// --------------------------------------------------------------------------

	// Play -> UI Button Click
	_gameState = Pg::DataScript::eGameState::_PLAY;

	// 1. playersList == 0
	_gameState = Pg::DataScript::eGameState::_STOP;

	// 2. current scene == play button in UI
	_gameState = Pg::DataScript::eGameState::_NOTSTARTED;

	// Pause -> UI Button Click
	_gameState = Pg::DataScript::eGameState::_PAUSE;
	// gameTime -> stop
	// player, monster -> not move


	// find portal
	auto currentit = std::find_if(_sceneList.begin(), _sceneList.end(),
		[&](std::string sceneName) {return _currentSceneName == sceneName; });
	auto nextit = std::next(currentit, 1);
}
