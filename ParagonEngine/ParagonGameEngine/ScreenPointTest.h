#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// ScreenPointToRay 테스트하기 위해.
/// </summary>

//ParagonScript로 이동하면.
//namespace Pg::API
//{
//	namespace Raycast
//	{
//		class PgRayCast;
//	}
//}

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

		Pg::Data::Camera* sceneMainCamera{ nullptr };

	private:
		Pg::Engine::Physic::PhysicSystem* _pgPhysics{ nullptr };
	};
}

