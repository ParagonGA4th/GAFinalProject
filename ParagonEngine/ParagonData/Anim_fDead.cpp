#include "Anim_fDead.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_fDead::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			monHelper->_state = Pg::Data::MonsterState::DEAD;

			if (monHelper->_isAnimationEnd)
			{
				monHelper->_isDeadDelay = true;
				monHelper->_isAnimationEnd = false;
				return BT::NodeStatus::FAILURE;
			}

			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00004.pganim");

				if (tMeshRenderer->GetAnimation() != animId)
				{
					tMeshRenderer->SetAnimation(animId, false);

					std::string objName = this->GetGameObject()->GetName();
					objName = objName.substr(0, objName.rfind("_"));
					if (objName.find("Golem") != std::string::npos) objName.append("_Crtstal");
					else objName.append("_Wing");

					auto tchild = this->GetGameObject()->_transform.GetChild(objName);
					auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

					animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
					animId.append("_10004.pganim");
					tcMeshRenderer->SetAnimation(animId, false);
				}
			}
		}
		return BT::NodeStatus::SUCCESS;
	}
}