#include "EnemySight.h"
#include "PlayerHandler.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticCollider.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonUtil/CheckInBox.h"
#include "../ParagonUtil/Log.h"


namespace Pg::DataScript
{
	EnemySight::EnemySight(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void EnemySight::Start()
	{
		//_collider = _object->GetComponent<Pg::Data::StaticCollider>();
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
	}

	void EnemySight::Update()
	{
		if (_playerBattleBehavior != nullptr)
		{
			auto dcol = _playerBattleBehavior->_object->GetComponent<Pg::Data::DynamicCollider>();

			//플레이어가 시야 안에 들어왔는지 검사
			if (Pg::Util::CheckInBox::IsIn3DBox(
				_object->_transform._position, _collider->_width, _collider->_height, _collider->_depth,
				_playerBattleBehavior->_object->_transform._position, dcol->GetWidth(), dcol->GetHeight(), dcol->GetDepth()))
			{
				PG_TRACE("Stay");
			}
			else
			{

			}
		}
	}

	void EnemySight::OnTriggerEnter(Pg::Data::Collider* c)
	{
		//PG_TRACE("충돌함");
		//PG_TRACE(c->_object->GetName().c_str());

		if (c->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
		{
			_playerDetected = true;
		}
	}

	void EnemySight::OnTriggerExit(Pg::Data::Collider* c)
	{
		if (c->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
		{
			_playerDetected = false;
		}
	}

}