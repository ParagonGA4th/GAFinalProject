#include "isPase_3.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPase_3::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_3)
			{
				if (!_isInit)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FLY_ATTACK_PREPARE_1;
					_isInit = true;
				}
				
				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}