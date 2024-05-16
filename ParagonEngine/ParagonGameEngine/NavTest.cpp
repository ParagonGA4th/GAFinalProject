#include "NavTest.h"
#include "Navigation.h"
#include "../ParagonData/GameObject.h"

#include <singleton-cpp/singleton.h>

namespace Pg::Engine
{
	NavTest::NavTest(Pg::Data::GameObject* owner) :
		Component(owner)
	{
		auto& tNavSystem = singleton<Pg::Engine::Navigation>(); 
		_navSystem = &tNavSystem;
	}

	void NavTest::Start()
	{
		_navSystem->SetAgent(0, 2.f, 45.f, 2.f, 0.9f);

		_navSystem->SyncAgent(0, _object->_transform._position);
	}

	void NavTest::Update()
	{
		_navSystem->SetSEpos(0, {_object->_transform._position.x, 0.f, _object->_transform._position.z},
			{ 0.0f, 0.0f, -10.0f });

		_navSystem->FindStraightPath(0);
	}
}