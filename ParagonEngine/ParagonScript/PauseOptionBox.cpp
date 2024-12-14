#include "PauseOptionBox.h"
#include "PauseBox.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/Slider.h"
#include "../ParagonData/Handle.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgAudio.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonUtil/Log.h"
#include <cassert>

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	PauseOptionBox::PauseOptionBox(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgAudio = &singleton<Pg::API::Sound::PgAudio>();
	}

	void PauseOptionBox::GrabManagedObjects()
	{
		//일시정지 창 닫기.
		btnObj = _object->GetScene()->FindObjectWithName("PauseOptionExit");
		_exitBtn = btnObj->GetComponent<Pg::Data::Button>();
		//_object->GetComponent<Pg::Data::Transform>()->AddChild(btnObj);

		//인게임 브금
		ingameSoundObj = _object->GetScene()->FindObjectWithName("SoundManager");
		_ingameSound = ingameSoundObj->GetComponent<Pg::Data::AudioSource>();

		_pauseBox = _object->GetScene()->FindObjectWithName("PauseBox");
		_pauseBoxComponent = _pauseBox->GetComponent<PauseBox>();
		assert(_pauseBoxComponent != nullptr);

		Pg::Data::GameObject* _soundBarObj = _object->GetScene()->FindObjectWithName("SoundBar");
		_soundBar = _soundBarObj->GetComponent<Pg::Data::Slider>();

		Pg::Data::GameObject* _soundHandleObj = _object->GetScene()->FindObjectWithName("SoundHandle");
		_soundHandle = _soundHandleObj->GetComponent<Pg::Data::Handle>();
		_soundHandleRenderer = _soundHandleObj->GetComponent<Pg::Data::ImageRenderer>();
	}

	void PauseOptionBox::Awake()
	{
		//일시정지 창 활성화 및 비활성화
		btnObj->GetComponent<Pg::Data::Button>()->SetOnClickDownEvent([this]
			{

				_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);
				_soundBar->SetActive(false);
				_soundHandle ->SetActive(false);
				_soundHandleRenderer->SetActive(false);
				_exitBtn->SetActive(false);

				for (auto& iter : _object->_transform.GetChildren())
				{

					for (auto& iter : _object->_transform.GetChildren())
					{
						Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

						im->SetActive(false);
					}
				}

				//for (auto& iter : _soundBar->_object->_transform.GetChildren())
				//{
				//	Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

				//	im->SetActive(false);
				//}

				_pauseBox->GetComponent<Pg::Data::ImageRenderer>()->SetActive(true);

				for (auto& iter : _pauseBox->_transform.GetChildren())
				{
					Pg::Data::Button* btn = iter->_object->GetComponent<Pg::Data::Button>();
					Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

					btn->SetActive(true);
					im->SetActive(true);
				}
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

		_soundBar->SetValueEvent([this](float vol)
			{
				PG_TRACE(vol);
				_pgAudio->SetBGMVolume(vol);
				_pgAudio->SetEffectVolume(vol);
			});
	}

	void PauseOptionBox::Start()
	{
		//처음엔 일시정지 창이 꺼져있어야 함.
		_object->GetComponent<Pg::Data::ImageRenderer>()->SetActive(false);
		_soundBar->SetActive(false);
		_soundHandle->SetActive(false);
		_soundHandleRenderer->SetActive(false);
		_exitBtn->SetActive(false);

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

			im->SetActive(false);
		}

		//for (auto& iter : _soundBar->_object->_transform.GetChildren())
		//{
		//	Pg::Data::ImageRenderer* im = iter->_object->GetComponent<Pg::Data::ImageRenderer>();

		//	im->SetActive(false);
		//}
	}

	void PauseOptionBox::Update()
	{

	}

	void PauseOptionBox::ResetAll()
	{
		Awake();
		Start();

		_pauseBoxComponent->ResetAll();
	}
}
