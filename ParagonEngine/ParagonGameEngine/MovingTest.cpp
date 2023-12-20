#include "MovingTest.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Camera.h"
#include "../ParagonMath/PgMath.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonGameEngine/InputSystem.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

MovingTest::MovingTest(Pg::Data::GameObject* obj) :Pg::Data::Script(obj)
{

}

void MovingTest::Start()
{
	// Input
	auto& tInputSystem = singleton<Pg::Engine::Input::InputSystem>();
	tInput = &tInputSystem;
}

void MovingTest::Update()
{
	using namespace Pg::Engine::Input;
	using namespace Pg::API::Input;

	auto tTransPos = _object->_transform.GetPosition();

	if (tInput->GetKey(KeyLeft))
	{
		_object->_transform.SetPosition(tTransPos.x - 0.1f, tTransPos.y, tTransPos.z);
	}
	if (tInput->GetKey(KeyRight))
	{
		_object->_transform.SetPosition(tTransPos.x + 0.1f, tTransPos.y, tTransPos.z);
	}
	if (tInput->GetKey(KeyUp))
	{
		_object->_transform.SetPosition(tTransPos.x, tTransPos.y + 0.1f, tTransPos.z);
	}
	if (tInput->GetKey(KeyDown))
	{
		_object->_transform.SetPosition(tTransPos.x, tTransPos.y - 0.1f, tTransPos.z);
	}
}
