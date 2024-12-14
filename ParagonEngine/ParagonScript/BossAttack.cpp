#include "BossAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	BossAttack::BossAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void BossAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void BossAttack::Awake()
	{

	}

	void BossAttack::Start()
	{
	}

	void BossAttack::Update()
	{

	}

	void BossAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//�÷��̾����� �������� �־��
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-4.f);
			}
		}
	}
}