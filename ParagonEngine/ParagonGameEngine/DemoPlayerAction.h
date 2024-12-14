#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"
#include <vector>

/// <summary>
/// �÷��̾� ��ũ��Ʈ �׽�Ʈ, ��ǥ
/// </summary>

class ArrowAction;

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


	class DemoPlayerAction : public Pg::Data::Component
	{
	public:
		DemoPlayerAction(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		// ��ǲ �ý������� ī�޶��� �̵��� üũ�Ѵ�
		Pg::Util::Input::InputSystem* _input = nullptr;

		Pg::Data::DynamicCollider* _collider;

		std::vector<ArrowAction*> _arrowObjects;
	};

