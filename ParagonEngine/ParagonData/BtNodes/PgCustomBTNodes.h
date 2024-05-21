///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

// Monster
#include "../ParagonData/Anim_Idle.h"
#include "../ParagonData/Anim_Walk.h"
#include "../ParagonData/Anim_Delay.h"
#include "../ParagonData/DefaultAnim_Delay.h"

#include "../ParagonData/isPlayerInSight.h"
#include "../ParagonData/isHitSpace.h"

//golem
#include "../ParagonData/Anim_Dash.h"
#include "../ParagonData/Anim_gAttack.h"
//#include "../ParagonData/Anim_gbArmAttack.h"
//#include "../ParagonData/Anim_gbSpinAttack.h"
#include "../ParagonData/isDistanceClose.h"