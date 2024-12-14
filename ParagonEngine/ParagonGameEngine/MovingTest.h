#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"

/// <summary>
/// �÷��̾� ��ũ��Ʈ �׽�Ʈ
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
	class AudioSource;
	class NavMeshAgent;
}

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

class MovingTest : public Pg::Data::Component
{
public:
	MovingTest(Pg::Data::GameObject* obj);

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	// ��ǲ �ý������� ī�޶��� �̵��� üũ�Ѵ�
	Pg::Util::Input::InputSystem* tInput = nullptr;
	Pg::Util::Time::TimeSystem* _timeSystem = nullptr;

	Pg::Data::DynamicCollider* dynamicCol;
	Pg::Data::AudioSource* audioSource;
	Pg::Data::NavMeshAgent* navMeshAgent;

	bool _isLocate = false;
};

