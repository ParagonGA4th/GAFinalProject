#include "ComboSystem.h"
#include "../ParagonAPI/PgTime.h"
#include <singleton-cpp/singleton.h>

Pg::DataScript::ComboSystem::ComboSystem(Pg::Data::GameObject* obj)
	: ScriptInterface(obj),
	_prevCombo(0, 0.f), _comboCount(0), _time(0)
{
	auto& tdelta = singleton<Pg::API::Time::PgTime>();
	_deltaTime = &tdelta;
}

void Pg::DataScript::ComboSystem::Start()
{

}

void Pg::DataScript::ComboSystem::Update()
{
	_time += _deltaTime->GetDeltaTime();
}

void Pg::DataScript::ComboSystem::ReSetCombo()
{
	// 모든 것을 초기화
	_comboCount = 0;
	_prevCombo.first = 0;
	_prevCombo.second = 0;
	_time = 0;
}

void Pg::DataScript::ComboSystem::HitObject(bool isHit)
{
	_comboCount++;
//
//			if (counting % 3 == 0)
//			{
//				if (prevComboData.first == 0)
//				{
//					attack = comboRoutine.at(1);
//					prevComboData.first = 1;
//				}
//				else
//				{
//					attack = comboRoutine.at(3);
//					prevComboData.first = 3;
//				}
//			}
//			else
//			{
//				if (prevComboData.first == 0)
//				{
//					attack = comboRoutine.at(1);
//					prevComboData.first = 1;
//				}
//
//				else if (prevComboData.first == 1)
//				{
//					attack = comboRoutine.at(2);
//					prevComboData.first = 2;
//				}
//
//				else if (prevComboData.first == 3)
//				{
//					attack = comboRoutine.at(1);
//					prevComboData.first = 1;
//				}
//			}
}

int Pg::DataScript::ComboSystem::GetComboCount()
{
	return _comboCount;
}