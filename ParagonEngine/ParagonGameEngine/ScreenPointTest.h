#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// ScreenPointToRay �׽�Ʈ�ϱ� ����.
/// </summary>

//ParagonScript�� �̵��ϸ�.
//namespace Pg::API
//{
//	namespace Raycast
//	{
//		class PgRayCast;
//	}
//}

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Engine
{
	namespace Physic
	{
		class PhysicSystem;
	}
}

namespace Pg::Data
{
	class Collider;
	class Camera;
}

namespace Pg::Engine
{
	class ScreenPointTest : public Pg::Data::Component
	{
	public:
		ScreenPointTest(Pg::Data::GameObject* owner);
		~ScreenPointTest();

	public:

		virtual void Start() override;
		virtual void Update() override;

		Pg::Data::Camera* _sceneMainCamera{ nullptr };

	private:
		Pg::Engine::Physic::PhysicSystem* _pgPhysics{ nullptr };
		Pg::Util::Input::InputSystem* _pgInput{ nullptr };
	};
}

