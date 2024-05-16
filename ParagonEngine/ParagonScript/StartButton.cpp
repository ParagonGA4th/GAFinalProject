#include "StartButton.h"
#include "../ParagonData/Button.h"
#include "../ParagonAPI/PgScene.h"

namespace Pg::DataScript
{
	StartButton::StartButton(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
	}

	void StartButton::Awake()
	{
		_button = _object->GetComponent<Pg::Data::Button>();
	}

	void StartButton::Start()
	{
		_button->SetOnClickEvent([this]
			{
				_pgScene->SetCurrentScene("Stage1");
			});
	}

	void StartButton::Update()
	{

	}
}