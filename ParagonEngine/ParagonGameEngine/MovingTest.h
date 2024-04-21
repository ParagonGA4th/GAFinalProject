#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"

/// <summary>
/// 플레이어 스크립트 테스트
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
	// 인풋 시스템으로 카메라의 이동을 체크한다
	Pg::Util::Input::InputSystem* tInput = nullptr;
	Pg::Util::Time::TimeSystem* _timeSystem = nullptr;

	Pg::Data::DynamicCollider* dynamicCol;
	Pg::Data::AudioSource* audioSource;
	Pg::Data::NavMeshAgent* navMeshAgent;

	bool _isLocate = false;
};

