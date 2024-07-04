#include "WindBlastAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	WindBlastAttack::WindBlastAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void WindBlastAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void WindBlastAttack::Awake()
	{
		_wind = _object->GetScene()->FindObjectWithName("BossWindBlastEffect");
		_windRenderer = _wind->GetComponent<Pg::Data::SkinnedMeshRenderer>();
	}

	void WindBlastAttack::Start()
	{

	}

	void WindBlastAttack::Update()
	{
		_wind->_transform._position = _object->_transform.GetWorldPosition();
		_wind->_transform._rotation = _object->_transform.GetWorldRotation();
	}

	void WindBlastAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
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