///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

// public
#include "../ParagonData/Anim_Delay.h"
#include "../ParagonData/isPlayerInSight.h"
#include "../ParagonData/isDistanceClose.h"
#include "../ParagonData/isHitSpace.h"
#include "../ParagonData/isChase.h"
#include "../ParagonData/isDead.h"


// floating Monster
#include "../ParagonData/Anim_DeadFloating.h"
#include "../ParagonData/Anim_FloatingIdle.h"
#include "../ParagonData/Anim_FloatingWalk.h"

// Ground Monster
#include "../ParagonData/Anim_DeadGround.h"
#include "../ParagonData/Anim_GroundIdle.h"

//golem
#include "../ParagonData/Anim_Dash.h"
#include "../ParagonData/Anim_gAttack.h"
#include "../ParagonData/isDash.h"

//wasp
#include "../ParagonData/Anim_Shoot.h"
#include "../ParagonData/Anim_Stab.h"

//Trent
#include "../ParagonData/Anim_tAttack.h"
#include "../ParagonData/Anim_PoisonGas.h"