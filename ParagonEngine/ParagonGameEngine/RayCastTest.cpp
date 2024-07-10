#include "RayCastTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/Button.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
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
	////사운드 테스트.
	//tAudioSource = _object->GetComponent<Pg::Data::AudioSource>();
	//tButton = _object->GetComponent<Pg::Data::Button>();
}

void RayCastTest::Update()
{
	//using namespace Pg::Engine::Input;
	//using namespace Pg::API::Input;
	
	float sphereRadius = 2.f; // 구체 반지름
	float maxDistance = 1.0f; // 최대 감지 거리
	const int maxColliders = 100; // 최대 충돌 객체 수
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

	//// 충돌한 객체들 처리
	//	for (int i = 0; i < maxColliders; ++i)
	//	{
	//		if (colliderHits[i])
	//		{
	//			Pg::Data::Collider* hitCollider = *colliderHits[i];
	//
	//			// 충돌한 객체 처리 로직
	//		}
	//	}
	
	////사운드 테스트.
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
