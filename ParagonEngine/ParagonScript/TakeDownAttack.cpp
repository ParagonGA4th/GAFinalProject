#include "TakeDownAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	TakeDownAttack::TakeDownAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void TakeDownAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void TakeDownAttack::Awake()
	{

	}

	void TakeDownAttack::Start()
	{

	}

	void TakeDownAttack::Update()
	{

	}

	void TakeDownAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어한테 데미지를 주어라
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-8.f);
			}
		}
	}
}