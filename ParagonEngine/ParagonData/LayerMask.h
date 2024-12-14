#pragma once

namespace Pg::Data::Enums
{
	enum eLayerMask : unsigned int
	{
		LAYER_DEFAULT = 0,			// 기본 등장.
		LAYER_PLAYER = 1,			// 플레이어의 레이어
		LAYER_MONSTER = 2,			// 몬스터, 적들의 레이어
		LAYER_PROJECTILES = 3,		// 날아다니는 투사체들의 Area.
		LAYER_MAP = 4,				// 맵 기믹(함정, 전투 등)
		LAYER_BACKGROUND = 5,		// 맵 배경
		LAYER_MOVABLE_OBJECTS = 6,	// 움직일 수 있는 맵 배경.
		LAYER_BOSS = 7,

		SELECT_NONE = 16 //이는 실제로 사용되어서는 안된다. Raycast Filtering을 위해.
	};
}