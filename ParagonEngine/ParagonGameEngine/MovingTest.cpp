#include "MovingTest.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/ForceMode.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/NavMeshAgent.h"
#include "../ParagonMath/PgMath.h"


// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonUtil/TimeSystem.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

MovingTest::MovingTest(Pg::Data::GameObject* obj) :Pg::Data::Component(obj)
{

}

void MovingTest::Start()
{
	// Input
	auto& tInputSystem = singleton<Pg::Util::Input::InputSystem>();
	tInput = &tInputSystem;
	
	auto& tPgTime = singleton<Pg::Util::Time::TimeSystem>();
	this->_timeSystem = &tPgTime;

	dynamicCol = _object->GetComponent<Pg::Data::DynamicCollider>();

	audioSource = _object->GetComponent<Pg::Data::AudioSource>();

	navMeshAgent = _object->GetComponent<Pg::Data::NavMeshAgent>();

}

void MovingTest::Update()
{
	using namespace Pg::API::Input;
	using namespace Pg::Util::Input;

	if (tInput->GetKey(KeyLeft))
	{
		_object->_transform._position.x -= _timeSystem->GetDeltaTime() * 3.0f;

		//if (tInput->GetKey(KeyZ))
		//{
		//	dynamicCol->AddVelocity({ -0.1f, 0.0f, 0.0f });
		//}
	}
	if (tInput->GetKey(KeyRight))
	{
		_object->_transform._position.x += _timeSystem->GetDeltaTime() * 3.0f;

		//if (tInput->GetKey(KeyZ))
		//{
		//	dynamicCol->AddVelocity({ 0.1f, 0.0f, 0.0f });
		//}
	}
	if (tInput->GetKey(KeyUp))
	{
		_object->_transform._position.z += _timeSystem->GetDeltaTime() * 3.0f;

		//if (tInput->GetKey(KeyZ))
		//{
		//	dynamicCol->AddVelocity({ 0.0f, 0.0f, 0.1f });
		//}
		
		//navMeshAgent->SetRelocate({ -10.0f,5.0f,0.0f });
	}

	if (tInput->GetKey(KeyDown))
	{
		_object->_transform._position.z -= _timeSystem->GetDeltaTime() * 3.0f;

		//if (tInput->GetKey(KeyZ))
		//{
		//	dynamicCol->AddVelocity({ 0.0f, 0.0f, -0.1f });
		//}
		
		//navMeshAgent->SetRelocate({ -5.0f,1.0f,0.0f });
	}
	if (tInput->GetKeyDown(Space))
	{
		//dynamicCol->SetRotationOffset(Pg::Math::PGQuaternionMultiply(dynamicCol->GetRotationOffset(), Pg::Math::PGEulerToQuaternion({1.f, 0.f, 0.f})));
		// dynamicCol->SetRotationOffset({})
		//_isLocate = true;

		//if (!_isLocate)
		//{
		//	navMeshAgent->SetRelocate({ -10.0f,5.0f,0.0f });
		//	_isLocate = false;
		//}

		//_object->_transform._position = { -10.0f, 5.0f, 0.0f };
		//navMeshAgent->SetDestination({ 0.0f,0.0f,0.0f });
		dynamicCol->AddForce({ 0.0f, 5.0f ,0.0f }, Pg::Data::ForceMode::eIMPULSE);
		//audioSource->Play();
	}
	if (tInput->GetKey(ShiftR))
	{
		_object->_transform._position.y -= _timeSystem->GetDeltaTime() * 3.0f;
	}
}
