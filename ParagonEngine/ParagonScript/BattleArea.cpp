#include "BattleArea.h"
#include "PlayerBattleBehavior.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/DynamicCollider.h"

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
	// 1스테이지에서는 플레이어가 전투 구역에서 벗어나지 못하도록 막아주는 투명벽이 있음
	// 플레이어가 전투 구역에 있는 모든 몬스터를 해치웠을 경우 빠져나갈 수 있음
	if (!_onTriggerStay)
	{
		if (_player != nullptr && _monster != 0)
		{
			auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();

			auto& colPos = _collider->_object->_transform._position;
			auto& playerPos = _player->_object->_transform._position;
			
			if (colPos.x - (_collider->_width / 2) > playerPos.x + (dcol->GetWidth() / 2))
				playerPos.x = colPos.x - (_collider->_width / 2);

			if (colPos.x + (_collider->_width / 2) < playerPos.x - (dcol->GetWidth() / 2))
				playerPos.x = colPos.x + (_collider->_width / 2);

			if (colPos.z - (_collider->_depth / 2) > playerPos.z + (dcol->GetDepth() / 2))
				playerPos.z = colPos.z - (_collider->_depth / 2);

			if (colPos.z + (_collider->_depth / 2) < playerPos.z - (dcol->GetDepth() / 2))
				playerPos.z = colPos.z + (_collider->_depth / 2);
		}
	}
}

void Pg::DataScript::BattleArea::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
{
	for (int i = 0; i < count; i++)
	{
		Pg::Data::Collider* col = _colArr[i];

		if (col->_object->GetTag() == "TAG_Player")
		{
			_onTriggerStay = true;
			_player = col->_object->GetComponent<Pg::DataScript::PlayerBattleBehavior>();
		}
	}
}

void Pg::DataScript::BattleArea::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
{
	for (int i = 0; i < count; i++)
	{
		Pg::Data::Collider* col = _colArr[i];
		if (col->_object->GetTag() == "TAG_Player")
		{
			_onTriggerStay = false;
		}
	}
}
