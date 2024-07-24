#include "Anim_SpinAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_SpinAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._bossStateListByEnum[monHelper->_bGolemFlag._bossState].find("SAttack") == std::string::npos)
				return BT::NodeStatus::FAILURE;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isAnimationEnd = false;
				monHelper->_isAnimChange = false;
				
				if(monHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK_2)
					monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::BASIC_ATTACK;

				if (monHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK_1)
					monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::SKILL_ATTACK_2;

				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00009.pganim");

				if (!monHelper->_isAnimChange &&
					(tMeshRenderer->GetAnimation() != animId
						|| monHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK_2))
				{
					tMeshRenderer->SetAnimation(animId, false);
					monHelper->_isAnimChange = true;

					std::string objName = this->GetGameObject()->GetName();
					objName = objName.substr(0, objName.rfind("_"));
					objName.append("_Crtstal");

					auto tchild = this->GetGameObject()->_transform.GetChild(objName);
					auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

					animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
					animId.append("_10009.pganim");
					tcMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}