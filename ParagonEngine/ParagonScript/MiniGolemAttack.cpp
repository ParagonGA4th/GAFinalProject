#include "MiniGolemAttack.h"
#include "CombatSystem.h"
#include "MiniGolemAttack.h"
#include "../ParagonData/Collider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{

	MiniGolemAttack::MiniGolemAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void MiniGolemAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void MiniGolemAttack::Awake()
	{

	}

	void MiniGolemAttack::Update()
	{

	}

	void MiniGolemAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		PG_TRACE("Player Hit!");
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어한테 데미지를 주어라
			if (col->_object->GetTag() == "TAG_Player")
			{
				_combatSystem->ChangePlayerHealth(-1.f);
			}
		}
	}
}