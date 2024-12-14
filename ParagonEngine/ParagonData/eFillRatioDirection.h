#pragma once

///FillRatio가 어디에서 시작 - 끝나는지 양상을 Enum으로 정하기 위해.
namespace Pg::Data
{
	enum eFillRatioDirection
	{
		LEFT_TO_RIGHT = 0,
		DOWN_TO_UP = 1, // 이거는 현재 못 씀.
		UP_TO_DOWN = 2
	};
}
