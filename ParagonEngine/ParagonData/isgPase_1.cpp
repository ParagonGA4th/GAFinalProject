#include "isgPase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isgPase_1::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			if (monHelper->_bGolemFlag._isPase_1)
			{
				monHelper->_bGolemFlag._bossPase = Pg::Data::BossPase::PASE_1;
				if (!_isInit)
				{
					monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::BASIC_ATTACK_1;
					_isInit = true;
				}
				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}