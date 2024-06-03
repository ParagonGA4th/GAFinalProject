#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Button;
	class GameObject;
	class AudioSource;
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
		Pg::Data::AudioSource* _audioSource;
		Pg::API::PgScene* _pgScene = nullptr;
		Pg::API::Input::PgInput * _pgInput = nullptr;

		Pg::Data::GameObject* _title;
		Pg::Data::AudioSource* _titleAudioSource;

		std::string _nextScene;
	};
}

