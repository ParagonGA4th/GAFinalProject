#include "GolemBossSkillAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{

	GolemBossSkillAttack::GolemBossSkillAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void GolemBossSkillAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void GolemBossSkillAttack::Awake()
	{

	}

	void GolemBossSkillAttack::Update()
	{

	}

	void GolemBossSkillAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//�÷��̾����� �������� �־��
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-3.f);
			}
		}
	}
}