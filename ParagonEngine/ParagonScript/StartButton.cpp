#include "StartButton.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonUtil/CheckInBox.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	StartButton::StartButton(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void StartButton::Awake()
	{
		_button = _object->GetComponent<Pg::Data::Button>();
		_audioSource = _object->GetComponent<Pg::Data::AudioSource>();

		//타이틀에 존재하는 AudioSource 찾기
		_title = _object->GetScene()->FindObjectWithName("Title");
		_titleAudioSource = _title->GetComponent<Pg::Data::AudioSource>();
	}

	void StartButton::Start()
	{
		_titleAudioSource->Play();

		_button->SetOnClickDownEvent([this]
			{
				_audioSource->Play();
			});
		_button->SetOnClickUpEvent([this]
			{
				_pgScene->SetCurrentScene("Stage1");
			});
	}

	void StartButton::Update()
	{
		//if(_pgInput->GetKeyUp(API::Input::MouseLeft))
		//{
		//	_pgScene->SetCurrentScene("Stage1");
		//}
	}
}