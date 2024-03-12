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
}

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
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
	//Pg::Engine::Input::InputSystem* tInput = nullptr;

	Pg::Data::DynamicCollider* dynamicCol;
};

