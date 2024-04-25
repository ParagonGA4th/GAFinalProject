#include "BattleArea.h"
#include "PlayerBattleBehavior.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonUtil/CheckInBox.h"
#include "../ParagonUtil/Log.h"

Pg::DataScript::BattleArea::BattleArea(Pg::Data::GameObject* obj)
	:ScriptInterface(obj)
{
}

void Pg::DataScript::BattleArea::Awake()  
{
	_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	assert(_collider != nullptr);
}

void Pg::DataScript::BattleArea::Start()
{
}

void Pg::DataScript::BattleArea::Update()
{
	if (_player != nullptr)
	{
		auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();

		//if (Pg::Util::CheckInBox::IsIn3DBox(
		//	_collider->_object->_transform._position, _collider->_width, _collider->_height, _collider->_depth,
		//	_player->_object->_transform._position, dcol->GetWidth(), dcol->GetHeight(), dcol->GetDepth()))		
			
			if (Pg::Util::CheckInBox::IsIn3DBox(
				_collider->_object->_transform._position, _object->_transform._scale.x, _object->_transform._scale.y, _object->_transform._scale.z,
				_player->_object->_transform._position, dcol->GetWidth(), dcol->GetHeight(), dcol->GetDepth()))
		{

			PG_TRACE("Stay");
		}
		else
		{
			// 1스테이지에서는 플레이어가 전투 구역에서 벗어나지 못하도록 막아주는 투명벽이 있음
			// 플레이어가 전투 구역에 있는 모든 몬스터를 해치웠을 경우 빠져나갈 수 있음
			
			if (_monster != 0)
			{
				auto& colPos = _collider->_object->_transform._position;
				float width = _object->_transform._scale.x;
				float depth = _object->_transform._scale.z;

				auto& playerPos = _player->_object->_transform._position;

				//if (colPos.x - (_collider->_width / 2) > playerPos.x + (dcol->GetWidth() / 2))
				//{
				//	playerPos.x = colPos.x - (_collider->_width / 2);
				//}
				//if (colPos.x + (_collider->_width / 2) < playerPos.x - (dcol->GetWidth() / 2))
				//{
				//	playerPos.x = colPos.x + (_collider->_width / 2);
				//}
				//if (colPos.z - (_collider->_depth / 2) > playerPos.z + (dcol->GetDepth() / 2))
				//{
				//	playerPos.z = colPos.z - (_collider->_depth / 2);
				//}
				//if (colPos.z + (_collider->_depth / 2) < playerPos.z - (dcol->GetDepth() / 2))
				//{
				//	playerPos.z = colPos.z + (_collider->_depth / 2);
				//}
				
				if (colPos.x - (width / 2) > playerPos.x + (dcol->GetWidth() / 2))
				{
					playerPos.x = colPos.x - (width / 2);
				}
				if (colPos.x + (width / 2) < playerPos.x - (dcol->GetWidth() / 2))
				{
					playerPos.x = colPos.x + (width / 2);
				}
				if (colPos.z - (depth / 2) > playerPos.z + (dcol->GetDepth() / 2))
				{
					playerPos.z = colPos.z - (depth / 2);
				}
				if (colPos.z + (depth / 2) < playerPos.z - (dcol->GetDepth() / 2))
				{
					playerPos.z = colPos.z + (depth / 2);
				}
			}
		}
	}
}

void Pg::DataScript::BattleArea::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		_player = col->_object->GetComponent<Pg::DataScript::PlayerBattleBehavior>();

		PG_TRACE("Trigger");
	}
}