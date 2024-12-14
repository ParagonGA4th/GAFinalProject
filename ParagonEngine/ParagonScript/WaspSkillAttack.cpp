#include "WaspSkillAttack.h"
#include "WaspBehaviour.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	WaspSkillAttack::WaspSkillAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void WaspSkillAttack::BeforePhysicsAwake()
	{
		_col = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void WaspSkillAttack::Awake()
	{

		_corn = _object->GetScene()->FindObjectWithName(_cornName);
		_cornRenderer = _corn->GetComponent<Pg::Data::StaticMeshRenderer>();
	}

	void WaspSkillAttack::Update()
	{
		_corn->_transform._position = _object->_transform.GetWorldPosition();
		_corn->_transform._rotation = _object->_transform.GetWorldRotation();
	}

	void WaspSkillAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			//�÷��̾����� �������� �־��
			if (col->_object->GetTag() == "TAG_Player")
			{
				PG_TRACE("Player Hit!");
				_combatSystem->ChangePlayerHealth(-3.f);
				_isPlayerHit = true;
			}
		}
	}

	void WaspSkillAttack::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void WaspSkillAttack::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

}