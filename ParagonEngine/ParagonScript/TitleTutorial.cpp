#include "TitleTutorial.h"
#include "../ParagonData/Button.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/CheckInBox.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	TitleTutorial::TitleTutorial(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void TitleTutorial::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _left = _object->GetScene()->FindObjectWithName("LeftArrow");
		_leftbutton = _left->GetComponent<Pg::Data::Button>();
		_leftRenderer = _left->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _right = _object->GetScene()->FindObjectWithName("RightArrow");
		_rightbutton = _right->GetComponent<Pg::Data::Button>();
		_rightRenderer = _right->GetComponent<Pg::Data::ImageRenderer>();

		Pg::Data::GameObject* _exit = _object->GetScene()->FindObjectWithName("TutorialExit");
		_exitbutton = _exit->GetComponent<Pg::Data::Button>();
		_exitRenderer = _exit->GetComponent<Pg::Data::ImageRenderer>();

		_audioSource = _object->GetComponent<Pg::Data::AudioSource>();

		_renderer->SetActive(false);

		_leftbutton->SetActive(false);
		_leftRenderer->SetActive(false);

		_rightbutton->SetActive(false);
		_rightRenderer->SetActive(false);

		_exitbutton->SetActive(false);
		_exitRenderer->SetActive(false);
	}

	void TitleTutorial::Awake()
	{
		_renderer->SetActive(false);

		_leftbutton->SetActive(false);
		_leftRenderer->SetActive(false);

		_rightbutton->SetActive(false);
		_rightRenderer->SetActive(false);

		_exitbutton->SetActive(false);
		_exitRenderer->SetActive(false);

		_renderer->SetImageIndex(0);
	}

	void TitleTutorial::Start()
	{
		_currentIndex = 0; // 초기 인덱스 값을 0으로 설정

		_exitbutton->SetOnClickDownEvent([this]
			{
				_audioSource->Play();

				_renderer->SetActive(false);

				_leftbutton->SetActive(false);
				_leftRenderer->SetActive(false);

				_rightbutton->SetActive(false);
				_rightRenderer->SetActive(false);

				_exitbutton->SetActive(false);
				_exitRenderer->SetActive(false);
			});
		_exitbutton->SetHover([this]
			{
				_exitRenderer->SetImageIndex(1);
			});
		_exitbutton->SetNotHover([this]
			{
				_exitRenderer->SetImageIndex(0);
			});

		_rightbutton->SetOnClickDownEvent([this]
			{
				_audioSource->Play();

				if (_currentIndex < maxIndex)
				{
					_currentIndex++;
				}
				_renderer->SetImageIndex(_currentIndex);
			});
		_rightbutton->SetHover([this]
			{
				_rightRenderer->SetImageIndex(1);
			});
		_rightbutton->SetNotHover([this]
			{
				_rightRenderer->SetImageIndex(0);
			});

		_leftbutton->SetOnClickDownEvent([this]
			{
				_audioSource->Play();

				if (_currentIndex > minIndex)
				{
					_currentIndex--;
				}
				_renderer->SetImageIndex(_currentIndex);
			});
		_leftbutton->SetHover([this]
			{
				_leftRenderer->SetImageIndex(1);
			});
		_leftbutton->SetNotHover([this]
			{
				_leftRenderer->SetImageIndex(0);
			});
	}

	void TitleTutorial::Update()
	{

	}
}