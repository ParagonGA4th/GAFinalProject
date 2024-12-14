#include "PlayerMove.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ForceMode.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/KeyCodeType.h"
#include "../ParagonData/LayerMask.h"

#include <singleton-cpp/singleton.h>
#include <cassert>

namespace Pg::DataScript
{
	PlayerMove::PlayerMove(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		//SCRIPT_FACTORY_INIT;
	}

	void PlayerMove::Start()
	{
		dynamicCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		dynamicCol->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PLAYER); 
		dynamicCol->FreezeAxisX(true);
		dynamicCol->FreezeAxisY(true);
		dynamicCol->FreezeAxisZ(true);
	}

	void PlayerMove::Update()
	{
		/*using namespace Pg::API::Input;

		if (_pgInput->GetKey(KeyLeft))
		{
			_object->_transform._position.x -= 0.01f;

			if (_pgInput->GetKey(KeyZ))
			{
				dynamicCol->AddVelocity({ -0.1f, 0.0f, 0.0f });
			}
		}
		if (_pgInput->GetKey(KeyRight))
		{
			_object->_transform._position.x += 0.01f;

			if (_pgInput->GetKey(KeyZ))
			{
				dynamicCol->AddVelocity({ 0.1f, 0.0f, 0.0f });
			}
		}
		if (_pgInput->GetKey(KeyUp))
		{
			_object->_transform._position.z += 0.01f;

			if (_pgInput->GetKey(KeyZ))
			{
				dynamicCol->AddVelocity({ 0.0f, 0.0f, 0.1f });
			}
		}

		if (_pgInput->GetKey(KeyDown))
		{
			_object->_transform._position.z -= 0.01f;

			if (_pgInput->GetKey(KeyZ))
			{
				dynamicCol->AddVelocity({ 0.0f, 0.0f, -0.1f });
			}
		}
		if (_pgInput->GetKeyDown(Space))
		{
			dynamicCol->AddForce({ 0.0f, 10.0f ,0.0f }, Pg::Data::ForceMode::eIMPULSE);
		}*/
	}



}
