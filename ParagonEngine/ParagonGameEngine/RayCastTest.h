#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// RayCast�� �׽�Ʈ�ϱ� ���� ��ũ��Ʈ.
/// 2024.01.25
/// </summary>

//namespace Pg::Util
//{
//	namespace Input
//	{
//		class InputSystem;
//	}
//}

namespace Pg::Engine
{
	namespace Physic
	{
		class PhysicSystem;
	}
}

namespace Pg::API
{
	namespace Raycast
	{
		class PgRayCast;
	}
}

namespace Pg::Data
{
	class AudioSource;
	class Button;
	class Collider;
}

class RayCastTest : public Pg::Data::Component
{
public:
	RayCastTest(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	//Pg::Util::Input::InputSystem* tInput = nullptr;

	Pg::Engine::Physic::PhysicSystem* tPhysic = nullptr;

	Pg::Data::AudioSource* tAudioSource;

	Pg::Data::Button* tButton;
};

 