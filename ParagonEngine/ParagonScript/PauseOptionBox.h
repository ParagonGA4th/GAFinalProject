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

	namespace Sound
	{
		class PgAudio;
	}
}

namespace Pg::DataScript
{
	class PauseBox;
	class PauseOptionBox : public ScriptInterface<PauseOptionBox>, public IScriptResettable
	{
		DEFINE_PARAGON_SCRIPT(PauseOptionBox);

	public:
		PauseOptionBox(Pg::Data::GameObject* obj);

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
		Pg::API::Sound::PgAudio* _pgAudio = nullptr;


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

		Pg::Data::GameObject* _pauseBox;
		PauseBox* _pauseBoxComponent;

		Pg::Data::Slider* _soundBar;
		Pg::Data::Handle* _soundHandle;
		Pg::Data::ImageRenderer* _soundHandleRenderer;

		Pg::Data::AudioSource* _ingameSound;
	};
}


