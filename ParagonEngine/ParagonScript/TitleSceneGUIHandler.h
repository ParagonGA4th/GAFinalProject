#pragma once
#include "ScriptInterface.h"
#include "IGUIHandler.h"

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

/// <summary>
/// 2D / TitleScene / GUI Handler.
/// </summary>

namespace Pg::DataScript
{
	class TitleSceneGUIHandler : public ScriptInterface<TitleSceneGUIHandler>, public IGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(TitleSceneGUIHandler);

	public:
		TitleSceneGUIHandler(Pg::Data::GameObject* obj);

	public:
		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;


		virtual void ResetToInitialState() override;

	private:
		Pg::API::PgScene* _pgScene = nullptr;
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::Data::AudioSource* _btnClick{ nullptr };

		Pg::Data::GameObject* _title{ nullptr };
		Pg::Data::AudioSource* _titleAudioSource{ nullptr };

		Pg::Data::GameObject* _start{ nullptr };
		Pg::Data::Button* _startButton{ nullptr };

		Pg::Data::GameObject* _saveFile{ nullptr };
		Pg::Data::Button* _saveFileButton{ nullptr };

		Pg::Data::GameObject* _howToPlay{ nullptr };
		Pg::Data::Button* _howToPlayButton{ nullptr };
		
		Pg::Data::GameObject* _exit{ nullptr };
		Pg::Data::Button* _exitButton{ nullptr };
		
	private:
		bool _isOnButton;
	};
}

