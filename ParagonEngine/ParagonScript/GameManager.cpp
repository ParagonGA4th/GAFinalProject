#include "GameManager.h"
#include "Portal.h"

#include "../ParagonData/Scene.h"

#include <algorithm>


void Pg::DataScript::GameManager::Awake()
{
	_gameState = Pg::DataScript::eGameState::_NOTSTARTED;	// 게임이 시작하지 않음
	_playerState = Pg::DataScript::ePlayerState::_NONE;		// 플레이어에 대한 정보 없음
	_artifacts.reserve(3);	// 아티팩트의 개수
	_playersLife = 3;		// 플레이어의 실제 목숨 개수	
}

void Pg::DataScript::GameManager::Start()
{
	_currentSceneName = _object->GetScene()->GetSceneName();

	/// portal -> next scene name 전달 해주기	
	// next scene Name 찾기
	auto currentIt = std::find_if(_sceneList.begin(), _sceneList.end(),
		[&](std::string sceneName) {return _currentSceneName == sceneName; });

	std::string nextSceneName;
	if (currentIt != _sceneList.end())
	{
		auto nextIt = std::next(currentIt, 1);
		nextSceneName = *nextIt;
	}
	else
		nextSceneName = (* _sceneList.begin());

	// 찾은 next scene Name 전달
	for (auto& obj : _object->GetScene()->GetObjectList())
	{
		if (obj->GetComponent<Pg::DataScript::Portal>() != nullptr)
		{
			obj->GetComponent<Pg::DataScript::Portal>()->SetNextScene(nextSceneName);
		}
	}
}

void Pg::DataScript::GameManager::Update()
{
	// Play -> UI Button Click
	_gameState = Pg::DataScript::eGameState::_PLAY;

	if (_gameState == eGameState::_PLAY)
	{
		// Pause -> UI Button Click
		_gameState = Pg::DataScript::eGameState::_PAUSE;
		// gameTime -> stop
		// player, monster -> not move
		
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
		// --------------------------------------------------------------------------

		/// player
		// -> 체력에 따라 live/die 판별
		// -> player가 죽었을 경우 게임 내의 시간을 멈춘다. (몬스터의 공격도 멈춘다..?)
		// -> player의 위치는 곧 현재 씬
		// -> player의 목숨 관리(체력 말고 다시 되살아 날 수 있는 목숨 -> 3개)
		if (_playerState == ePlayerState::_DIE)
		{
			if (_playersLife == 0)
			{		
				// 1. playersLife == 0
				_gameState = Pg::DataScript::eGameState::_STOP;

				// 2. current scene == play button in UI
				_gameState = Pg::DataScript::eGameState::_NOTSTARTED;
			}
			else
				_playersLife -= 1;
		}
	}
}

void Pg::DataScript::GameManager::ResetGame()
{
	_gameTime = 0.0f;
	_gameState = eGameState::_NOTSTARTED;
	_playersLife = 3;

	for (auto& artifact : _artifacts)
	{
		artifact.second = false;
	}
}
