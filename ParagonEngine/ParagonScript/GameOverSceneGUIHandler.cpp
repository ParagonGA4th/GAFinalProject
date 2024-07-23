#include "GameOverSceneGUIHandler.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	GameOverSceneGUIHandler::GameOverSceneGUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();
	}

	void GameOverSceneGUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();

		_backgroundImage = _object->GetScene()->FindObjectWithName("GameOverBackground")->GetComponent<Pg::Data::ImageRenderer>();
		assert(_backgroundImage != nullptr);

		_overText = _object->GetScene()->FindObjectWithName("GameOverText")->GetComponent<Pg::Data::ImageRenderer>();
		assert(_overText != nullptr);
	}

	void GameOverSceneGUIHandler::Awake()
	{
		//씬 막 들어왔을 때 일 것이다.
		_backgroundImage->SetActive(true);
		_overText->SetActive(true);
	}

	void GameOverSceneGUIHandler::Start()
	{
		_inputTime = 0.f;
	}

	void GameOverSceneGUIHandler::Update()
	{
		_inputTime += _pgTime->GetDeltaTime();

		if (_inputTime > STAY_IN_SCENE_TIME)
		{
			_pgScene->SetCurrentScene("TitleScene");
			_inputTime = 0.f;
		}
	}

	void GameOverSceneGUIHandler::AssignPointersToGUI()
	{
		//
	}



}
