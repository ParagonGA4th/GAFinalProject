#include "MovingTest.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ForceMode.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
//#include "../ParagonGameEngine/InputSystem.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

MovingTest::MovingTest(Pg::Data::GameObject* obj) :Pg::Data::Component(obj)
{

}

void MovingTest::Start()
{
	//// Input
	//auto& tInputSystem = singleton<Pg::Engine::Input::InputSystem>();
	//tInput = &tInputSystem;
	//
	//dynamicCol = _object->GetComponent<Pg::Data::DynamicCollider>();
}

void MovingTest::Update()
{

	//using namespace Pg::Engine::Input;
	//using namespace Pg::API::Input;
	//
	//if (tInput->GetKey(KeyLeft))
	//{
	//	_object->_transform._position.x -= 0.01f;
	//
	//	if (tInput->GetKey(KeyZ))
	//	{
	//		dynamicCol->AddVelocity({ -0.1f, 0.0f, 0.0f });
	//	}
	//}
	//if (tInput->GetKey(KeyRight))
	//{
	//	_object->_transform._position.x += 0.01f;
	//
	//	if (tInput->GetKey(KeyZ))
	//	{
	//		dynamicCol->AddVelocity({ 0.1f, 0.0f, 0.0f });
	//	}
	//}
	//if (tInput->GetKey(KeyUp))
	//{
	//	_object->_transform._position.z += 0.01f;
	//
	//	if (tInput->GetKey(KeyZ))
	//	{
	//		dynamicCol->AddVelocity({ 0.0f, 0.0f, 0.1f });
	//	}
	//}
	//
	//if (tInput->GetKey(KeyDown))
	//{
	//	_object->_transform._position.z -= 0.01f;
	//
	//	if (tInput->GetKey(KeyZ))
	//	{
	//		dynamicCol->AddVelocity({ 0.0f, 0.0f, -0.1f });
	//	}
	//}
	//if (tInput->GetKeyDown(Space))
	//{
	//	dynamicCol->AddForce({ 0.0f, 10.0f ,0.0f }, Pg::Data::ForceMode::eIMPULSE);
	//}
}
