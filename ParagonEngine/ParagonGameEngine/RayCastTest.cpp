#include "RayCastTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Button.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
//#include "../ParagonGameEngine/InputSystem.h"
#include "../ParagonGameEngine/PhysicSystem.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

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
	//auto& tPhysicSystem = singleton<Pg::Engine::Physic::PhysicSystem>();
	//tPhysic = &tPhysicSystem;
	//
	////사운드 테스트.
	//tAudioSource = _object->GetComponent<Pg::Data::AudioSource>();
	//tButton = _object->GetComponent<Pg::Data::Button>();
}

void RayCastTest::Update()
{
	//using namespace Pg::Engine::Input;
	//using namespace Pg::API::Input;
	//
	//int type = 0;
	//
	//tPhysic->MakeRayCast({_object->_transform._position.x + 0.8f,
	//					_object->_transform._position.y,
	//					_object->_transform._position.z + 0.8f }, { 1.0f,0.0f,0.0f }, 10.0f, &type);
	//
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
