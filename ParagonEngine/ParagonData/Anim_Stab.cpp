#include "Anim_Stab.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_Stab::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isAnimationEnd)
			{
				_isAnimEnd = true;
				monHelper->_isAnimationEnd = false;
			}
			else
			{
				_isAnimEnd = false;
			}
		}

		auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		if (tMeshRenderer != nullptr)
		{
			config().blackboard->set<std::string>("CURRENTANIM", "_00006");
			std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
			animId.append("_00006.pganim");

			if (tMeshRenderer->GetAnimation() != animId)
			{
				tMeshRenderer->SetAnimation(animId, false);
				config().blackboard->set<bool>("ISCHANGE", true);

				std::string objName = this->GetGameObject()->GetName();
				objName = objName.substr(0, objName.rfind("_"));
				objName.append("_Wing");

				auto tchild = this->GetGameObject()->_transform.GetChild(objName);
				auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

				animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_10006.pganim");
				tcMeshRenderer->SetAnimation(animId, false);
			}
		}

		if (_isAnimEnd) config().blackboard->set<bool>("ISCHANGE", false);
		return BT::NodeStatus::SUCCESS;
	}
}