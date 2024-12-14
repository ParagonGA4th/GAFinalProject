#include "isPase_1.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPase_1::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{
			std::string state = monHelper->_bossFlag._bossStateListByEnum[monHelper->_bossFlag._bossState];
			if (state.find("BAttack") != std::string::npos ||
				state.find("Cast") != std::string::npos)
			{
				return BT::NodeStatus::SUCCESS;
			}

			if (monHelper->_bossFlag._bossPase == Pg::Data::BossPase::PASE_1)
			{
				if (monHelper->_isPlayerinHitSpace &&
					monHelper->_bossFlag._bossState == Pg::Data::BossState::CHASE)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::CAST;
				}

				if (!_isInit)
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::CAST;
					_isInit = true;
				}

				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}