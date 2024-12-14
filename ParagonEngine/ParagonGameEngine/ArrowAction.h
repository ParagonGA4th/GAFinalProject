#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"
#include <vector>

/// <summary>
/// �÷��̾� ��ũ��Ʈ �׽�Ʈ, ��ǥ
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
	// ��ǲ �ý������� ī�޶��� �̵��� üũ�Ѵ�
	Pg::Util::Input::InputSystem* _input = nullptr;

	Pg::Data::DynamicCollider* _collider;

};