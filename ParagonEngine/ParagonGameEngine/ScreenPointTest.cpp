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
		using Pg::API::Input::eKeyCode;
		if (_pgInput->GetKeyDown(eKeyCode::MouseLeft))
		{
			_object->_transform._position = _sceneMainCamera->ScreenPointToWorldPlanePoint({ _pgInput->GetMouseX(), _pgInput->GetMouseY() },
				{0,1,0}, 0);
			//Ex.
			//0,1,0 노말에 비해 -1만큼 Distance가 있어야 y=1에 위치할 수 있다.
		}
		
	}

}