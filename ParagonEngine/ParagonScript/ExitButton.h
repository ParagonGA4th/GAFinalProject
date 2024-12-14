#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Button;
}

namespace Pg::DataScript
{
	class ExitButton : public ScriptInterface<ExitButton>
	{
		DEFINE_PARAGON_SCRIPT(ExitButton);

	public:
		ExitButton(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* _button;
	};
}

