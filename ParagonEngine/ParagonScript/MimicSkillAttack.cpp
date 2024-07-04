#include "MimicSkillAttack.h"
#include "MimicBehaviour.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	MimicSkillAttack::MimicSkillAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void MimicSkillAttack::BeforePhysicsAwake()
	{
		_col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	}

	void MimicSkillAttack::Awake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);

		_coin = _object->GetScene()->FindObjectWithName(_coinName);
		_coinRenderer = _coin->GetComponent<Pg::Data::StaticMeshRenderer>();
	}

	void MimicSkillAttack::Update()
	{
		_coin->_transform._position = _object->_transform.GetWorldPosition();
	}

	void MimicSkillAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//플레이어한테 데미지를 주어라
			if (col->_object->GetTag() == "TAG_Player")
			{
				 PG_TRACE("Player Hit!");

				_combatSystem->ChangePlayerHealth(-2.f);
				_isPlayerHit = true;
			}
		}
	}

	void MimicSkillAttack::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void MimicSkillAttack::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

}