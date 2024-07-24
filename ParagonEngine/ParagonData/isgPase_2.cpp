#include "isgPase_2.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isgPase_2::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._bossPase == Pg::Data::BossPase::PASE_2)
			{
				if (!_isInit)
				{
					monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::SKILL_DASH_ATTACK;
					_isInit = true;
				}
				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}