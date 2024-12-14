#include "ExitButton.h"
#include "../ParagonData/Button.h"
#include <windows.h>

namespace Pg::DataScript
{
	ExitButton::ExitButton(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void ExitButton::Awake()
	{
		_button = _object->GetComponent<Pg::Data::Button>();
	}

	void ExitButton::Start()
	{
		_button->SetOnClickDownEvent([this]
			{
				Sleep(500);
				SendMessage(GetActiveWindow(), WM_CLOSE, NULL, NULL);
			});
	}

	void ExitButton::Update()
	{

	}
}
