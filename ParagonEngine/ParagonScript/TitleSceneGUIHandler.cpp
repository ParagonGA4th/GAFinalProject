#include "TitleSceneGUIHandler.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>
#include <windows.h>

namespace Pg::DataScript
{
	TitleSceneGUIHandler::TitleSceneGUIHandler(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isOnButton(false)
	{
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void TitleSceneGUIHandler::GrabManagedObjects()
	{
		GrabOrganizeAllGUI(_object);
		AssignPointersToGUI();
	}

	void TitleSceneGUIHandler::Start()
	{
		_titleAudioSource->Play();

		///각 버튼마다 이벤트 설정해주기

		//시작 버튼.
		_startButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
				_titleAudioSource->Stop();
			});
		_startButton->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_startButton->GetImageRenderer()->SetImageIndex(1);
			});
		_startButton->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_startButton->GetImageRenderer()->SetImageIndex(0);
			});
		_startButton->SetOnClickUpEvent([this]
			{
				_pgScene->SetCurrentScene("Stage1");
			});

		//튜토리얼 버튼.
		_howToPlayButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();

				_tutorialRenderer->SetActive(true);

				_leftbutton->SetActive(true);
				_leftRenderer->SetActive(true);

				_rightbutton->SetActive(true);
				_rightRenderer->SetActive(true);

				_exitbutton->SetActive(true);
				_exitRenderer->SetActive(true);
			});
		_howToPlayButton->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_howToPlayButton->GetImageRenderer()->SetImageIndex(1);
			});
		_howToPlayButton->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_howToPlayButton->GetImageRenderer()->SetImageIndex(0);
			});

		//튜토리얼 버튼.
		_creditButton->SetOnClickDownEvent([this]
			{
				_pgScene->SetCurrentScene("CreditsScene");
				_btnClick->Play();
			});
		_creditButton->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_creditButton->GetImageRenderer()->SetImageIndex(1);
			});
		_creditButton->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_creditButton->GetImageRenderer()->SetImageIndex(0);
			});

		//종료 버튼.
		_exitButton->SetOnClickDownEvent([this]
			{
				_btnClick->Play();
			});
		_exitButton->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_exitButton->GetImageRenderer()->SetImageIndex(1);
			});
		_exitButton->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_exitButton->GetImageRenderer()->SetImageIndex(0);
			});
		_exitButton->SetOnClickUpEvent([this]
			{
				Sleep(500);
				SendMessage(GetActiveWindow(), WM_CLOSE, NULL, NULL);
			});
	}

	void TitleSceneGUIHandler::Update()
	{
	}

	void TitleSceneGUIHandler::AssignPointersToGUI()
	{
		// 얘는 일단 이 자체로도 잘 작동하기에,
		// 내버려둔다.

		//Start버튼
		//이제 여기에서는 현재 씬이 아니라, 자신의 오브젝트가 속한 씬을 가져와야 한다.
		//_start = _object->GetScene()->FindObjectWithName("Start");
		_start = _object->GetScene()->FindObjectWithName("Start");
		_startButton = _start->GetComponent<Pg::Data::Button>();

		_btnClick = _start->GetComponent<Pg::Data::AudioSource>();

		//Tutorial버튼
		//_howToPlay = _object->GetScene()->FindObjectWithName("HowToPlay");
		_howToPlay = _object->GetScene()->FindObjectWithName("HowToPlay");
		_howToPlayButton = _howToPlay->GetComponent<Pg::Data::Button>();

		//Exit버튼
		//_exit = _object->GetScene()->FindObjectWithName("Exit");
		_exit = _object->GetScene()->FindObjectWithName("Exit");
		_exitButton = _exit->GetComponent<Pg::Data::Button>();

		//타이틀에 존재하는 AudioSource 찾기
		//_title = _object->GetScene()->FindObjectWithName("Title");
		_title = _object->GetScene()->FindObjectWithName("Title");
		_titleAudioSource = _title->GetComponent<Pg::Data::AudioSource>();

		//타이틀에 존재하는 AudioSource 찾기
//_title = _object->GetScene()->FindObjectWithName("Title");
		_credit = _object->GetScene()->FindObjectWithName("Credit");
		_creditButton = _credit->GetComponent<Pg::Data::Button>();

		Pg::Data::GameObject* _tutorial = _object->GetScene()->FindObjectWithName("TutorialImage");
		_tutorialRenderer = _tutorial->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _left = _object->GetScene()->FindObjectWithName("LeftArrow");
		_leftbutton = _left->GetComponent<Pg::Data::Button>();
		_leftRenderer = _left->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _right = _object->GetScene()->FindObjectWithName("RightArrow");
		_rightbutton = _right->GetComponent<Pg::Data::Button>();
		_rightRenderer = _right->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _exit = _object->GetScene()->FindObjectWithName("TutorialExit");
		_exitbutton = _exit->GetComponent<Pg::Data::Button>();
		_exitRenderer = _exit->GetComponent<Pg::Data::ImageRenderer>();
	}

}