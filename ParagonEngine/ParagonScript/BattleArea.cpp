#include "BattleArea.h"

#include "../ParagonData/StaticBoxCollider.h"
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
}

void Pg::DataScript::BattleArea::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		// 1스테이지일 경우만 필요함.
		// 1스테이지에서는 플레이어가 전투 구역에서 벗어나지 못하도록 막아주는 투명벽이 있음

		PG_TRACE("Trigger");
		_isInit = true;
	}
}

void Pg::DataScript::BattleArea::OnTriggerExit(Pg::Data::Collider* col)
{
	// 플레이어가 전투 구역에 있는 모든 몬스터를 해치웠을 경우 빠져나갈 수 있음
	if(_isInit/*&& 구역 내에 있는 모든 몬스터 수가 0일 경우*/)
	{
	}
}
