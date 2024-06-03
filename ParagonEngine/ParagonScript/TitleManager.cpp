#include "TitleManager.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>
#include <windows.h>

namespace Pg::DataScript
{
	TitleManager::TitleManager(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void TitleManager::Awake()
	{
		//Start버튼
		_start = _pgScene->GetCurrentScene()->FindObjectWithName("Start");
		_startButton = _start->GetComponent<Pg::Data::Button>();

		_btnClick = _start->GetComponent<Pg::Data::AudioSource>();

		//SaveFile버튼
		_saveFile = _pgScene->GetCurrentScene()->FindObjectWithName("SaveFile");
		_saveFileButton = _saveFile->GetComponent<Pg::Data::Button>();

		//Tutorial버튼
		_howToPlay = _pgScene->GetCurrentScene()->FindObjectWithName("HowToPlay");
		_howToPlayButton = _howToPlay->GetComponent<Pg::Data::Button>();

		//Exit버튼
		_exit = _pgScene->GetCurrentScene()->FindObjectWithName("Exit");
		_exitButton = _exit->GetComponent<Pg::Data::Button>();

		//타이틀에 존재하는 AudioSource 찾기
		_title = _pgScene->GetCurrentScene()->FindObjectWithName("Title");
		_titleAudioSource = _title->GetComponent<Pg::Data::AudioSource>();
	}

	void TitleManager::Start()
	{
		_titleAudioSource->Play();

		///각 버튼마다 이벤트 설정해주기

		_startButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
				_titleAudioSource->Stop();
			});
		_startButton->SetOnClickUpEvent([this]
			{
				_pgScene->SetCurrentScene("Stage1");
			});

		_howToPlayButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
			});

		_saveFileButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
			});

		_exitButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
			});
		_exitButton->SetOnClickUpEvent([this]
			{
				Sleep(500);
				SendMessage(GetActiveWindow(), WM_CLOSE, NULL, NULL);
			});
	}

	void TitleManager::Update()
	{

	}
}