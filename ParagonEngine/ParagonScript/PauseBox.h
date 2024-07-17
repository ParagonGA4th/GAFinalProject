#pragma once
#include "ScriptInterface.h"
#include "IScriptResettable.h"
namespace Pg::Data
{
	class Button;
	class AudioSource;
	class GameObject;
}

namespace Pg::API
{
	class PgScene;

	namespace Input
	{
		class PgInput;
	}

	namespace Time
	{
		class PgTime;
	}
}

namespace Pg::DataScript
{
	class PauseBox : public ScriptInterface<PauseBox>, public IScriptResettable
	{
		DEFINE_PARAGON_SCRIPT(PauseBox);

	public:
		PauseBox(Pg::Data::GameObject* obj);

	public:
		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		virtual void ResetAll() override;

	private:
		Pg::Data::Button* _button;
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::API::PgScene* _pgScene = nullptr;
		Pg::API::Time::PgTime* _pgTime = nullptr;


		//ÇÃ·¡±×
		bool _isPaused = false;

	private:
		Pg::Data::GameObject* btnObj;
		Pg::Data::GameObject* menuObj;
		Pg::Data::GameObject* tutorialObj;
		Pg::Data::GameObject* optionObj;
		Pg::Data::GameObject* ingameSoundObj;
		Pg::Data::AudioSource* _ingameSound;
	};
}

