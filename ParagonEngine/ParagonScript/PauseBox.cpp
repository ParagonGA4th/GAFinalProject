#include "PauseBox.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Slider.h"
#include "../ParagonData/Handle.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonData/ImageRenderer.h"
#include <cassert>

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	PauseBox::PauseBox(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
	}

	void PauseBox::GrabManagedObjects()
	{
		//일시정지 창 닫기.
		btnObj = _object->GetScene()->FindObjectWithName("PauseExit");
		_exitBtn = btnObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(btnObj);

		//메인메뉴로 가기.
		 menuObj = _object->GetScene()->FindObjectWithName("GotoMenu");
		_menuBtn = menuObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(menuObj);

		//튜토리얼로 가기.
		 tutorialObj = _object->GetScene()->FindObjectWithName("TutorialButton");
		_tutorialBtn = tutorialObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(tutorialObj);

		//설정으로 가기.
		 optionObj = _object->GetScene()->FindObjectWithName("OptionButton");
		_optionBtn = optionObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(optionObj);

		//인게임 브금
		 ingameSoundObj = _object->GetScene()->FindObjectWithName("SoundManager");
		_ingameSound = ingameSoundObj->GetComponent<Pg::Data::AudioSource>();

		//옵션창
		_optionBox = _object->GetScene()->FindObjectWithName("PauseOptionBox");

		Pg::Data::GameObject* _soundBarObj = _object->GetScene()->FindObjectWithName("SoundBar");
		_soundBar = _soundBarObj->GetComponent<Pg::Data::Slider>();

		Pg::Data::GameObject* _soundHandleObj = _object->GetScene()->FindObjectWithName("SoundHandle");
		_soundHandle = _soundHandleObj->GetComponent<Pg::Data::Handle>();
		_soundHandleRenderer = _soundHandleObj->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _optionExitObj = _object->GetScene()->FindObjectWithName("PauseOptionExit");
		_optionExitBtn = _optionExitObj->GetComponent<Pg::Data::Button>();
	}

	void PauseBox::Awake()
	{
		//일시정지 창 활성화 및 비활성화
		btnObj->GetComponent<Pg::Data::Button>()->SetOnClickDownEvent([this]
			{
				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

				for (auto& iter : _object->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(false);
					im->SetActive(false);
				}

				_pgTime->SetPauseGame(false);
				_isPaused = false;
			});

		//메인메뉴로 돌아가는 이벤트
		_menuBtn->SetOnClickDownEvent([this]
			{
				_ingameSound->Stop();
			});
		_menuBtn->SetOnClickUpEvent([this]
			{
				_pgScene->SetCurrentScene("TitleScene");
			});
		_menuBtn->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_menuBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_menuBtn->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_menuBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_exitBtn->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_exitBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_exitBtn->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_exitBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_tutorialBtn->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_tutorialBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_tutorialBtn->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_tutorialBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_optionBtn->SetOnClickDownEvent([this]
			{
				//일시정시 창 끄기
				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

				for (auto& iter : _object->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(false);
					im->SetActive(false);
				}

				//옵션 창 켜기
				_optionBox->GetComponent<Pg::Data::ImageRenderer>()->SetActive(true);
				_soundBar->SetActive(true);
				_soundHandle->SetActive(true);
				_soundHandleRenderer->SetActive(true);
				_optionExitBtn->SetActive(true);

				for (auto& iter : _optionBox->_transform.GetChildren())
				{
						Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

						im->SetActive(true);
				}

				//for (auto& iter : _soundBar->_object->_transform.GetChildren())
				//{
				//	Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

				//	im->SetActive(true);
				//}
			});
		_optionBtn->SetHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_optionBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_optionBtn->SetNotHover([this]
			{
				//비활성화 활성화를 위한 이벤트
				_optionBtn->GetImageRenderer()->SetImageIndex(0);
			});
	}

	void PauseBox::Start()
	{
		//처음엔 일시정지 창이 꺼져있어야 함.
		_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
			Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

			btn->SetActive(false);
			im->SetActive(false);
		}
	}

	void PauseBox::Update()
	{
		using namespace Pg::API::Input;

		//ESC 누르면 켜짐.
		if (_pgInput->GetKeyDown(eKeyCode::Esc) && _isPaused == false)
		{
			_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(true);

			for (auto& iter : _object->_transform.GetChildren())
			{
				Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
				Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

				btn->SetActive(true);
				im->SetActive(true);
			}
			
			_pgTime->SetPauseGame(true);
			_isPaused = true;
		}
		else if (_pgInput->GetKeyDown(eKeyCode::Esc) && _isPaused == true)
		{
			_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

			for (auto& iter : _object->_transform.GetChildren())
			{
				Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
				Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

				btn->SetActive(false);
				im->SetActive(false);
			}

			_pgTime->SetPauseGame(false);
			_isPaused = false;
		}
	}

	void PauseBox::ResetAll()
	{
		Awake();
		Start();
	}
	

}
