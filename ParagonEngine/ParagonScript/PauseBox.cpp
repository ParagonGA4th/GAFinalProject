#include "PauseBox.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Slider.h"
#include "../ParagonData/Handle.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"
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
		//�Ͻ����� â �ݱ�.
		btnObj = _object->GetScene()->FindObjectWithName("PauseExit");
		_exitBtn = btnObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(btnObj);

		//���θ޴��� ����.
		 menuObj = _object->GetScene()->FindObjectWithName("GotoMenu");
		_menuBtn = menuObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(menuObj);

		//Ʃ�丮��� ����.
		 tutorialObj = _object->GetScene()->FindObjectWithName("TutorialButton");
		_tutorialBtn = tutorialObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(tutorialObj);

		//�������� ����.
		 optionObj = _object->GetScene()->FindObjectWithName("OptionButton");
		_optionBtn = optionObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(optionObj);

		//�ΰ��� ���
		 ingameSoundObj = _object->GetScene()->FindObjectWithName("SoundManager");
		_ingameSound = ingameSoundObj->GetComponent<Pg::Data::AudioSource>();

		//�ɼ�â
		_optionBox = _object->GetScene()->FindObjectWithName("PauseOptionBox");
		
		Pg::Data::GameObject* _tutorial = _object->GetScene()->FindObjectWithName("PauseBoxTutorial");
		_tutorialRenderer = _tutorial->GetComponent<Pg::Data::ImageRenderer>();
		_tutorialRenderer->SetActive(false);

		Pg::Data::GameObject* _tutorialExit = _object->GetScene()->FindObjectWithName("PauseTutorialExit");
		_tutorialExitRenderer = _tutorialExit->GetComponent<Pg::Data::ImageRenderer>();
		_tutorialExitButton = _tutorialExit->GetComponent<Pg::Data::Button>();
		_tutorialExitRenderer->SetActive(false);
		_tutorialExitButton->SetActive(false);

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
		_tutorialExitRenderer->SetActive(false);
		_tutorialExitButton->SetActive(false);

		_tutorialRenderer->SetActive(false);

		//�Ͻ����� â Ȱ��ȭ �� ��Ȱ��ȭ
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

		//���θ޴��� ���ư��� �̺�Ʈ
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
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_menuBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_menuBtn->SetNotHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_menuBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_exitBtn->SetHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_exitBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_exitBtn->SetNotHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_exitBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_tutorialBtn->SetOnClickDownEvent([this]
			{
				//�Ͻ����� â ����
				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

				for (auto& iter : _object->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(false);
					im->SetActive(false);
				}

				PG_TRACE("Tutorial");
				_tutorialRenderer->SetActive(true);
				_tutorialExitRenderer->SetActive(true);
				_tutorialExitButton->SetActive(true);
			});
		_tutorialBtn->SetHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_tutorialBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_tutorialBtn->SetNotHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_tutorialBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_optionBtn->SetOnClickDownEvent([this]
			{
				//�Ͻ����� â ����
				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);

				for (auto& iter : _object->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(false);
					im->SetActive(false);
				}

				//�ɼ� â �ѱ�
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
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_optionBtn->GetImageRenderer()->SetImageIndex(1);
			});
		_optionBtn->SetNotHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_optionBtn->GetImageRenderer()->SetImageIndex(0);
			});

		_tutorialExitButton->SetOnClickDownEvent([this]
			{
				_tutorialRenderer->SetActive(false);
				_tutorialExitRenderer->SetActive(false);
				_tutorialExitButton->SetActive(false);

				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(true);

				for (auto& iter : _object->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(true);
					im->SetActive(true);
				}
			});
		_tutorialExitButton->SetHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_tutorialExitButton->GetImageRenderer()->SetImageIndex(1);
			});
		_tutorialExitButton->SetNotHover([this]
			{
				//��Ȱ��ȭ Ȱ��ȭ�� ���� �̺�Ʈ
				_tutorialExitButton->GetImageRenderer()->SetImageIndex(0);
			});
	}

	void PauseBox::Start()
	{
		//ó���� �Ͻ����� â�� �����־�� ��.
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

		//ESC ������ ����.
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
