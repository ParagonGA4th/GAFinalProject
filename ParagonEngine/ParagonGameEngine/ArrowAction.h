#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"
#include <vector>

/// <summary>
/// 플레이어 스크립트 테스트, 발표
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}


class ArrowAction : public Pg::Data::Component
{
public:
	ArrowAction(Pg::Data::GameObject* obj);

public:
	virtual void Start() override;
	virtual void Update() override;

	bool _restrainer = true;

private:
	// 인풋 시스템으로 카메라의 이동을 체크한다
	Pg::Util::Input::InputSystem* _input = nullptr;

	Pg::Data::DynamicCollider* _collider;

};