#include "Anim_Idle.h"
#include "SkinnedMeshRenderer.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Idle::tick()
	{
		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "_00001");
			bool isChange = config().blackboard->get<bool>("ISCHANGE");

			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00001.pganim");

			if (isChange || tMeshRenderer->GetAnimation() == animId)
			{
				return BT::NodeStatus::FAILURE;
			}
			else if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, true);
				config().blackboard->set<bool>("ISCHANGE", false);

				std::string objName = this->GetGameObject()->GetName();
				objName = objName.substr(0, objName.rfind("_"));
				if (objName.find("Golem") != std::string::npos) objName.append("_Crtstal");
				else objName.append("_Wing");

				auto tchild = this->GetGameObject()->_transform.GetChild(objName);
				auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

				animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_10001.pganim");
				tcMeshRenderer->SetAnimation(animId, false);

				return BT::NodeStatus::SUCCESS;
			}
		}

		return BT::NodeStatus::SUCCESS;
	}
}