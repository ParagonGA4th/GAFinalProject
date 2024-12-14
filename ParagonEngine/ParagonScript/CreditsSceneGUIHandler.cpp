#include "CreditsSceneGUIHandler.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"

#include <singleton-cpp/singleton.h>
namespace Pg::DataScript
{
	CreditsSceneGUIHandler::CreditsSceneGUIHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
	}

	void CreditsSceneGUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();
	}

	void CreditsSceneGUIHandler::Awake()
	{

	}

	void CreditsSceneGUIHandler::Start()
	{
		_vo = new Pg::Data::VisualEffectRenderObject();
		_vo->_position = { 1280, 720, 0 };

		_pgGraphics->RegisterEffectObject("Effect_Credits", _vo);

		_inputTime = 0.f;
	}

	void CreditsSceneGUIHandler::Update()
	{
		//if (tObj != nullptr)
		//{
		//	_pgGraphics->RemoveEffectObject(tObj);
		//	delete tObj;
		//
		//	tObj = nullptr;
		//}

		_inputTime += _pgTime->GetDeltaTime();

		if (_inputTime > STAY_IN_SCENE_TIME)
		{
			_pgScene->SetCurrentScene("TitleScene");
			_inputTime = 0.f;
		}

	}

	void CreditsSceneGUIHandler::AssignPointersToGUI()
	{
		//
	}

	void CreditsSceneGUIHandler::CleanOnSceneChange()
	{
		if (_vo != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_vo);
			delete _vo;

			_vo = nullptr;
		}
	}
	

}
