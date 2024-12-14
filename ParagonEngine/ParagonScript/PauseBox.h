#pragma once
#include "ScriptInterface.h"
#include "IScriptResettable.h"

namespace Pg::Data
{
	class Button;
	class Slider;
	class Handle;
	class AudioSource;
	class ImageRenderer;
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
		Pg::Data::Button* _exitBtn;

		Pg::Data::GameObject* menuObj;
		Pg::Data::Button* _menuBtn;
		
		Pg::Data::GameObject* tutorialObj;
		Pg::Data::Button* _tutorialBtn;
		
		Pg::Data::GameObject* optionObj;
		Pg::Data::Button* _optionBtn;
		
		Pg::Data::GameObject* ingameSoundObj;
		
		Pg::Data::AudioSource* _ingameSound;

		Pg::Data::GameObject* _optionBox;
		
		Pg::Data::Slider* _soundBar;
		Pg::Data::Button* _optionExitBtn;
		Pg::Data::Handle* _soundHandle;
		Pg::Data::ImageRenderer* _soundHandleRenderer;
		Pg::Data::ImageRenderer* _tutorialRenderer;
		Pg::Data::ImageRenderer* _tutorialExitRenderer;
		Pg::Data::Button* _tutorialExitButton;
	};
}

