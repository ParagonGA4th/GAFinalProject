#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// ������ ���� �̺�Ʈ���� �׽�Ʈ�� ��ũ��Ʈ.
/// </summary>

namespace Pg::Data
{
	class Button;
	class AudioSource;
}

namespace Pg::Engine
{

	class EventTest : public Pg::Data::Component
	{
	public:
		EventTest(Pg::Data::GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* tButton;
		Pg::Data::AudioSource* audioSource;
	};
}

