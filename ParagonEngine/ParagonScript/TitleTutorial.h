#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Button;
	class GameObject;
	class AudioSource;
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class TitleTutorial : public ScriptInterface<TitleTutorial>
	{
		DEFINE_PARAGON_SCRIPT(TitleTutorial);

	public:
		TitleTutorial(Pg::Data::GameObject* obj);

	public:
		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* _exitbutton;
		Pg::Data::Button* _leftbutton;
		Pg::Data::Button* _rightbutton;

		Pg::Data::AudioSource* _audioSource;

		Pg::Data::ImageRenderer* _renderer;
		Pg::Data::ImageRenderer* _leftRenderer;
		Pg::Data::ImageRenderer* _rightRenderer;
		Pg::Data::ImageRenderer* _exitRenderer;

		std::string _nextScene;

		int _currentIndex; // ���� �ε����� �����ϴ� ����
		const int maxIndex = 6; // �ִ� �ε��� ��
		const int minIndex = 0; // �ּ� �ε��� ��
	};
}

