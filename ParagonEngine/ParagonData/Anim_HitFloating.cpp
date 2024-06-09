#include "Anim_HitFloating.h"
#include "SkinnedMeshRenderer.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus Anim_HitFloating::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_isHit)
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

				auto tMeshRenderer = this->GetGameObject()->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				if (tMeshRenderer != nullptr)
				{
					std::string animId = tMeshRenderer->GetAnimation().substr(0, tMeshRenderer->GetAnimation().find("_"));
					animId.append("_00003.pganim");

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
						animId.append("_10003.pganim");
						tcMeshRenderer->SetAnimation(animId, false);
					}
				}
			}
			else
			{
				return BT::NodeStatus::FAILURE;
			}
		}

		if (_isAnimEnd) return BT::NodeStatus::FAILURE;
		else return BT::NodeStatus::SUCCESS;
	}
}