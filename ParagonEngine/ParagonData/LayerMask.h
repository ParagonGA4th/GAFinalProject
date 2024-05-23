#pragma once

namespace Pg::Data::Enums
{
	enum eLayerMask : unsigned int
	{
		LAYER_DEFAULT = 0,		// 기본 등장.
		LAYER_PLAYER = 1,		// 플레이어의 레이어
		LAYER_MONSTER = 2,		// 몬스터, 적들의 레이어
		LAYER_PROJECTILES = 3,	// 날아다니는 투사체들의 Area.
		LAYER_MAP = 4,			// 맵 기믹(함정, 전투 등)
		LAYER_BACKGROUND = 5,	// 맵 배경
	};
}