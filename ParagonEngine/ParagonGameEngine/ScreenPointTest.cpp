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
		using Pg::API::Input::eKeyCode;
		if (_pgInput->GetKeyDown(eKeyCode::MouseLeft))
		{
			Pg::Math::PGFLOAT3 tRayOrig;
			Pg::Math::PGFLOAT3 tRayDir;
			_sceneMainCamera->ScreenPointToRayInfo({ _pgInput->GetMouseX(), _pgInput->GetMouseY() }, tRayOrig, tRayDir);
			
			Pg::Math::PGFLOAT3 outHitPoint;

			//그냥 값 투입.
			int tColVal;

			if (_pgPhysics->MakeRayCast(tRayOrig, tRayDir, 100.0f, outHitPoint, &tColVal))
			{
				PG_TRACE("RAYCAST HIT");
				_object->_transform._position = outHitPoint;
			}
		}
		
	}

}