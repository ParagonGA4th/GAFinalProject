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
	class TitleManager : public ScriptInterface<TitleManager>
	{
		DEFINE_PARAGON_SCRIPT(TitleManager);

	public:
		TitleManager(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::API::PgScene* _pgScene = nullptr;
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::Data::AudioSource* _btnClick;

		Pg::Data::GameObject* _title;
		Pg::Data::AudioSource* _titleAudioSource;

		Pg::Data::GameObject* _start;
		Pg::Data::Button* _startButton;

		Pg::Data::GameObject* _saveFile;
		Pg::Data::Button* _saveFileButton;

		Pg::Data::GameObject* _howToPlay;
		Pg::Data::Button* _howToPlayButton;
		
		Pg::Data::GameObject* _exit;
		Pg::Data::Button* _exitButton;
		
	private:
		std::string _nextScene;

		bool _isOnButton;
	};
}

