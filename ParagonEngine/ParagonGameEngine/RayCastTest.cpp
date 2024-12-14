#include "RayCastTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/Button.h"
#include "../ParagonMath/PgMath.h"

// ������Ʈ���� �ӽ÷� ��ǲ�� ������ ����...
//#include "../ParagonGameEngine/InputSystem.h"
#include "../ParagonGameEngine/PhysicSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>
#include <cassert>
#include <array>
#include <algorithm>

RayCastTest::RayCastTest(Pg::Data::GameObject* obj) :
	Component(obj)
{

}

void RayCastTest::Start()
{
	//// Input
	//auto& tInputSystem = singleton<Pg::Engine::Input::InputSystem>();
	//tInput = &tInputSystem;
	//
	//// Physic
	auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
	tPhysic = &tPhysicSystem;
	//
	////���� �׽�Ʈ.
	//tAudioSource = _object->GetComponent<Pg::Data::AudioSource>();
	//tButton = _object->GetComponent<Pg::Data::Button>();
}

void RayCastTest::Update()
{
	//using namespace Pg::Engine::Input;
	//using namespace Pg::API::Input;
	
	float sphereRadius = 2.f; // ��ü ������
	float maxDistance = 1.0f; // �ִ� ���� �Ÿ�
	const int maxColliders = 100; // �ִ� �浹 ��ü ��
	Pg::Math::PGFLOAT3 outHitPoint;

	//Pg::Data::Collider** colliderHits[maxColliders] = { 0, 0, 0 };
	std::array<Pg::Data::Collider*,10> colVec;
	std::fill(colVec.begin(), colVec.end(), nullptr);
	
	
	int type = 0;
	tPhysic->MakeRayCast({_object->_transform._position.x + 0.8f,
						_object->_transform._position.y - 0.5f,
						_object->_transform._position.z + 0.8f }, { 0.0f,0.0f,1.0f }, 10.0f, outHitPoint, &type, true);

	//tPhysic->MakeSphereCast({ _object->_transform._position.x,
	//					_object->_transform._position.y,
	//					_object->_transform._position.z }, { 1.0f,0.0f,0.0f }, sphereRadius, maxDistance, colVec.size(), colVec.data());

	//// �浹�� ��ü�� ó��
	//	for (int i = 0; i < maxColliders; ++i)
	//	{
	//		if (colliderHits[i])
	//		{
	//			Pg::Data::Collider* hitCollider = *colliderHits[i];
	//
	//			// �浹�� ��ü ó�� ����
	//		}
	//	}
	
	////���� �׽�Ʈ.
	//if (tInput->GetKeyDown(Space))
	//{
	//	/*tButton->SetOnClickEvent([]()
	//		{
	//			PG_TRACE("Click!");
	//		});
	//	tButton->Click();*/
	//	if (tAudioSource != nullptr)
	//	{
	//		//tAudioSource->Play();
	//		//tAudioSource->Stop();
	//	}
	//}
}
