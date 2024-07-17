#include "TrentSkillAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	TrentSkillAttack::TrentSkillAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void TrentSkillAttack::BeforePhysicsAwake()
	{

		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void TrentSkillAttack::Awake()
	{

	}

	void TrentSkillAttack::Update()
	{

	}

	void TrentSkillAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어한테 데미지를 주어라
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-2.f);
			}
		}
	}
}