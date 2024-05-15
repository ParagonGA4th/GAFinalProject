#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Button;
}

namespace Pg::API
{
	class PgScene;

	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::DataScript
{
	class PauseBox : public ScriptInterface<PauseBox>
	{
		DEFINE_PARAGON_SCRIPT(PauseBox);

	public:
		PauseBox(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* _button;
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::API::PgScene* _pgScene = nullptr;
	};
}

