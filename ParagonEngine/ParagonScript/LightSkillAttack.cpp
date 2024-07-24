#include "LightSkillAttack.h"
#include "CombatSystem.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	LightSkillAttack::LightSkillAttack(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{

	}

	void LightSkillAttack::GrabManagedObjects()
	{
		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
	}

	void LightSkillAttack::BeforePhysicsAwake()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void LightSkillAttack::Awake()
	{
		_renderer->SetAnimation("bosspillar_0.pganim", false);
		_renderer->PauseAnim();
	}

	void LightSkillAttack::Start()
	{
	}

	void LightSkillAttack::Update()
	{

	}

	void LightSkillAttack::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
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

	void LightSkillAttack::OnAnimationEnd(const std::string& justEndedAnimation)
	{

	}
}