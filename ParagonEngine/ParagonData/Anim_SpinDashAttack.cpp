#include "Anim_SpinDashAttack.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_SpinDashAttack::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (tMeshRenderer != nullptr)
			{
				std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
				animId.append("_00009.pganim");

				if (tMeshRenderer->GetAnimation() != animId || monHelper->_isAnimationEnd)
				{
					tMeshRenderer->SetAnimation(animId, false);

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