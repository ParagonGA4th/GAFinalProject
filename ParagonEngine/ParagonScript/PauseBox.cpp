#include "PauseBox.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"
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
	}

	void PauseBox::GrabManagedObjects()
	{
		//일시정지 창 닫기.
		btnObj = _object->GetScene()->FindObjectWithName("PauseExit");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(btnObj);

		//메인메뉴로 가기.
		 menuObj = _object->GetScene()->FindObjectWithName("GotoMenu");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(menuObj);

		//튜토리얼로 가기.
		 tutorialObj = _object->GetScene()->FindObjectWithName("TutorialButton");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(tutorialObj);

		//설정으로 가기.
		 optionObj = _object->GetScene()->FindObjectWithName("OptionButton");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(optionObj);

		//인게임 브금
		 ingameSoundObj = _object->GetScene()->FindObjectWithName("SoundManager");
		_ingameSound = ingameSoundObj->GetComponent<Pg::Data::AudioSource>();
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

				_isPaused = false;
			});

		//메인메뉴로 돌아가는 이벤트
		menuObj->GetComponent<Pg::Data::Button>()->SetOnClickDownEvent([this]
			{
				_ingameSound->Stop();
			});
		menuObj->GetComponent<Pg::Data::Button>()->SetOnClickUpEvent([this]
			{
				_pgScene->SetCurrentScene("TitleScene");
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
			_isPaused = false;
		}
	}

	void PauseBox::ResetAll()
	{
		Awake();
		Start();
	}
	

}
