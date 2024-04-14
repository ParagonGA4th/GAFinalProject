#include "ScreenPointTest.h"
#include "PhysicSystem.h"
#include "../ParagonUtil/InputSystem.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"

#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	ScreenPointTest::ScreenPointTest(Pg::Data::GameObject* owner) : Component(owner)
	{

	}

	ScreenPointTest::~ScreenPointTest()
	{

	}

	void ScreenPointTest::Start()
	{
		_sceneMainCamera = _object->GetScene()->GetMainCamera();

		auto& tPhysSys = singleton<Pg::Engine::Physic::PhysicSystem>();
		_pgPhysics = &tPhysSys;

		auto& tInput = singleton<Pg::Util::Input::InputSystem>();
		_pgInput = &tInput;
	}

	void ScreenPointTest::Update()
	{
		//자기 자신의 Transform을 이동시키기.
		//PgRaycast의 ScreenPointToRay 구현체와 동일.
		//참조자로 값을 받기.

		///Y축으로만 뒤집었더니 원하는대로 Raycast가 나아갔다. 시도 중.
		//Raycast가 ScreenPointToRayInfo를 통해 그려진 값 이미 그려놓기.
		//Pg::Math::PGFLOAT3 tTrashValue;
		//
		//static float distance = 0.f;
		//distance += 0.1;
		//if (distance > 50.0f)
		//{
		//	distance = 0.f;
		//}
		//
		//Pg::Math::PGFLOAT3 tDir1 = { -0.023249, 0.305389, -0.951944 };
		//Pg::Math::PGFLOAT3 tDir2 = { -0.728838, 0.616882, 0.297072 };
		//
		//_pgPhysics->MakeRayCast({ 0.000000, 3.000000, -10.000000 }, { tDir1.x, tDir1.y, -tDir1.z
		//	}, distance, tTrashValue, nullptr);
		////_pgPhysics->MakeRayCast({ 0.000000, 3.000000, -10.000000 }, tDir1, distance, tTrashValue, nullptr);
		////
		////_pgPhysics->MakeRayCast({ -13.943266, 12.250350, 5.574352 }, tDir2, distance, tTrashValue, nullptr);

		using Pg::API::Input::eKeyCode;
		if (_pgInput->GetKeyDown(eKeyCode::MouseLeft))
		{
			Pg::Math::PGFLOAT3 tRayOrig;
			Pg::Math::PGFLOAT3 tRayDir;
			_sceneMainCamera->ScreenPointToRayInfo({ _pgInput->GetMouseX(), _pgInput->GetMouseY() }, tRayOrig, tRayDir);
			
			//For Logging.
			{
				std::string tVal = std::to_string(tRayOrig.x);
				tVal.append(", ");
				tVal.append(std::to_string(tRayOrig.y));
				tVal.append(", ");
				tVal.append(std::to_string(tRayOrig.z));
				tVal.append(" Dir : ");
				tVal.append(std::to_string(tRayDir.x));
				tVal.append(", ");
				tVal.append(std::to_string(tRayDir.y));
				tVal.append(", ");
				tVal.append(std::to_string(tRayDir.z));

				//std::string tVal = std::to_string(_pgInput->GetMouseX());
				//tVal.append(", ");
				//tVal.append(std::to_string(_pgInput->GetMouseY()));
				PG_TRACE(tVal.c_str());
			}

			Pg::Math::PGFLOAT3 outHitPoint;

			//그냥 값 투입.
			int tColVal;

			//24.04.13 : 디버깅해본 결과, 뒤집혀 있었다. 이를 마이너스화 하자! -> 하지마, 현재 정확한 법칙을 발견하지 못하는 중.
			//tRayDir.x *= -1.0f;
			//tRayDir.y *= -1.0f;
			//tRayDir.z *= -1.0f;

			if (_pgPhysics->MakeRayCast(tRayOrig, tRayDir, 100.0f, outHitPoint, &tColVal))
			{								 
				PG_TRACE("RAYCAST HIT");
				_object->_transform._position = outHitPoint;
			}
		}
		
	}

}