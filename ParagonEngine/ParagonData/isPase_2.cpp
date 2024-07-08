#include "isPase_2.h"
#include "MonsterHelper.h"

namespace Pg::Data::BTree::Node
{
	BT::NodeStatus isPase_2::tick()
	{
		auto monHelper = this->GetGameObject()->GetComponent<Pg::Data::MonsterHelper>();
		if (monHelper != nullptr)
		{

			if (monHelper->_bossFlag._isPase_2)
			{
				monHelper->_bossFlag._bossPase = Pg::Data::BossPase::PASE_2;
				if(_isInit)
				{
					if (monHelper->_bossFlag._bossState == Pg::Data::BossState::EVASION)
						monHelper->_bossFlag._bossState = Pg::Data::BossState::SKILL_FEATHER_ATTACK;
				}
				else
				{
					monHelper->_bossFlag._bossState = Pg::Data::BossState::EVASION;
					_isInit = true;
				}
				return BT::NodeStatus::SUCCESS;
			}
		}

		if (_isInit) _isInit = !_isInit;
		return BT::NodeStatus::FAILURE;
	}
}