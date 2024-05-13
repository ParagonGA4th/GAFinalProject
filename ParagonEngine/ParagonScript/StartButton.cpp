#include "StartButton.h"
#include "../ParagonData/Button.h"
#include "../ParagonAPI/PgScene.h"

namespace Pg::DataScript
{
	StartButton::StartButton(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void StartButton::Awake()
	{
		_button = _object->GetComponent<Pg::Data::Button>();

		_sceneHelper = new Pg::API::PgScene();
		_sceneHelper->Initialize();
	}

	void StartButton::Start()
	{
		_button->SetOnClickEvent([this]
			{
				_sceneHelper->SetCurrentScene("testing_Content1");
			});
	}

	void StartButton::Update()
	{

	}
}