#include "RayCastTest.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Button.h"
#include "../ParagonMath/PgMath.h"

#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/KeyCodeType.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonUtil/Log.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

RayCastTest::RayCastTest(Pg::Data::GameObject* obj) :
	Script(obj)
{

}

void RayCastTest::Start()
{

}

void RayCastTest::Update()
{
	using namespace Pg::API::Input;

	int type = 0;

	if (_pgInput->GetKey(CtrlL))
	{
		_pgRayCast->MakeRay({ _object->_transform._position.x + 0.8f,
						_object->_transform._position.y,
						_object->_transform._position.z + 0.8f }, { 1.0f,0.0f,0.0f }, 10.0f, &type);
	}
}
