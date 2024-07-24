#include "isgPase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isgPase_1::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			std::string state = monHelper->_bGolemFlag._bossStateListByEnum[monHelper->_bGolemFlag._bossState];
			if (state.find("BAttack") != std::string::npos)
			{
				return BT::NodeStatus::SUCCESS;
			}

			if (monHelper->_bGolemFlag._bossPase == Pg::Data::BossPase::PASE_1)
			{
				if (!_isInit)
				{
					monHelper->_bGolemFlag._bossState = Pg::Data::GolemBossState::BASIC_ATTACK;
					_isInit = true;
				}
				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}