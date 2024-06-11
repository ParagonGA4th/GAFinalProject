///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

// public
#include "../ParagonData/Anim_Delay.h"
#include "../ParagonData/Anim_HitFloating.h"
#include "../ParagonData/isPlayerInSight.h"
#include "../ParagonData/isHitSpace.h"
#include "../ParagonData/isChase.h"

// floating Monster
#include "../ParagonData/Anim_Idle.h"
#include "../ParagonData/Anim_Walk.h"

//golem
#include "../ParagonData/Anim_Dash.h"
#include "../ParagonData/Anim_gAttack.h"
#include "../ParagonData/isDash.h"

//wasp
#include "../ParagonData/Anim_Shoot.h"
#include "../ParagonData/Anim_Stab.h"