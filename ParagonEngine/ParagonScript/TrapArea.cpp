#include "TrapArea.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>

Pg::DataScript::TrapArea::TrapArea(Pg::Data::GameObject* obj)
	: ScriptInterface(obj)
{
}

void Pg::DataScript::TrapArea::Awake()
{
	_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	assert(_collider != nullptr);

	auto& tdelta = singleton<Pg::API::Time::PgTime>();
	_deltaTime = &tdelta;
}

void Pg::DataScript::TrapArea::Start()
{
}

void Pg::DataScript::TrapArea::Update()
{
}

void Pg::DataScript::TrapArea::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		// 플레이어의 움직임이 느려져야 함
		// deltaTime * moveSpeed
		// 플레이어가 계속 빠져야 함
		// deltaTime * force..?
		// 플레이어의 체력이 계속 깎여야 함
		// deltaTime * playerHP

		PG_TRACE("Trigger");
		_isInit = true;
	}
}

void Pg::DataScript::TrapArea::OnTriggerExit(Pg::Data::Collider* col)
{
}
