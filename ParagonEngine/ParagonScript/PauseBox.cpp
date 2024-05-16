#include "PauseBox.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/ImageRenderer.h"
#include <cassert>

namespace Pg::DataScript
{
	PauseBox::PauseBox(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgScene = &singleton<Pg::API::PgScene>();
	}

	void PauseBox::Awake()
	{
		//일시정지 창 닫기.
		auto btnObj = _object->GetScene()->FindObjectWithName("PauseExit");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(btnObj);

		//메인메뉴로 가기.
		auto menuObj = _object->GetScene()->FindObjectWithName("GotoMenu");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(menuObj);

		//메인메뉴로 가기.
		auto tutorialObj = _object->GetScene()->FindObjectWithName("TutorialButton");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(tutorialObj);

		//메인메뉴로 가기.
		auto optionObj = _object->GetScene()->FindObjectWithName("OptionButton");
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(optionObj);

		btnObj->GetComponent<Pg::Data::Button>()->SetOnClickEvent([this]
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

		menuObj->GetComponent<Pg::Data::Button>()->SetOnClickEvent([this]
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
}
