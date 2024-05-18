#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Button;
}

namespace Pg::API 
{
	class PgScene; 
}

namespace Pg::DataScript
{
	class StartButton : public ScriptInterface<StartButton>
	{
		DEFINE_PARAGON_SCRIPT(StartButton);

	public:
		StartButton(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* _button;
		Pg::API::PgScene* _pgScene = nullptr;

		std::string _nextScene;
	};
}

