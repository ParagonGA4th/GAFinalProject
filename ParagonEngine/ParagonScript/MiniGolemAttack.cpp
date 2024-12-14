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
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//�÷��̾����� �������� �־��
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-1.f);
			}
		}
	}
}