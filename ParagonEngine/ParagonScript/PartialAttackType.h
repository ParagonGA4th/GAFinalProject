#pragma once

namespace Pg::DataScript
{
	// 불 / 얼음을 빼고 기록한다.
	// 문제 X.
	enum class ePartialAttackType
	{
		eNORMAL_HIT,
		eICE_HIT,
		eFIRE_HIT,
		eSTRONG_HIT,
		eULTIMATE_HIT
	};
}
