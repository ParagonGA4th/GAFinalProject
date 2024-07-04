#include "TrentAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	TrentAttack::TrentAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void TrentAttack::BeforePhysicsAwake()
	{

		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void TrentAttack::Awake()
	{
	}

	void TrentAttack::Update()
	{

	}

	void TrentAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어한테 데미지를 주어라
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-1.f);
			}
		}
	}
}