///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

// public
#include "../ParagonData/Anim_Delay.h"
#include "../ParagonData/isPlayerInSight.h"
#include "../ParagonData/isDistanceClose.h"
#include "../ParagonData/isHitSpace.h"
#include "../ParagonData/isChase.h"
#include "../ParagonData/isDead.h"

// Boss
#include "../ParagonData/Anim_bDead.h"
#include "../ParagonData/Anim_bIdle.h"
#include "../ParagonData/Anim_bWalk.h"
#include "../ParagonData/Anim_bDash.h"
#include "../ParagonData/Anim_Cast.h"
#include "../ParagonData/Anim_LeftArmAttack.h"
#include "../ParagonData/Anim_RightArmAttack.h"
#include "../ParagonData/Anim_TwoArmAttack.h"
#include "../ParagonData/Anim_DownStart.h"
#include "../ParagonData/Anim_Down.h"
#include "../ParagonData/Anim_DownEnd.h"
#include "../ParagonData/Anim_Evasion.h"
#include "../ParagonData/Anim_FeatherAttackStart.h"
#include "../ParagonData/Anim_FeatherAttack.h"
#include "../ParagonData/Anim_FeatherAttackEnd.h"
#include "../ParagonData/Anim_FlyAttack.h"
#include "../ParagonData/Anim_FlyAttackPrePare.h"
#include "../ParagonData/Anim_Evasion.h"
#include "../ParagonData/isPase_1.h"
#include "../ParagonData/isPase_2.h"
#include "../ParagonData/isPase_3.h"
#include "../ParagonData/isDownStart.h"
#include "../ParagonData/isDown.h"
#include "../ParagonData/isDownEnd.h"
#include "../ParagonData/isbDash.h"

//golem Boss
#include "../ParagonData/Anim_gbTwoArmAttack.h"
#include "../ParagonData/Anim_SpinAttack.h"
#include "../ParagonData/Anim_SpinDashAttack.h"
#include "../ParagonData/Anim_gDown.h"
#include "../ParagonData/isgPase_1.h"
#include "../ParagonData/isgPase_2.h"
#include "../ParagonData/isgSkill.h"
#include "../ParagonData/isgDown.h"

// floating Monster
#include "../ParagonData/Anim_fDead.h"
#include "../ParagonData/Anim_fIdle.h"
#include "../ParagonData/Anim_fWalk.h"

// Ground Monster
#include "../ParagonData/Anim_gDead.h"
#include "../ParagonData/Anim_gIdle.h"
#include "../ParagonData/isCoolDown.h"

//golem
#include "../ParagonData/Anim_Dash.h"
#include "../ParagonData/Anim_gAttack.h"
#include "../ParagonData/isDash.h"

//wasp
#include "../ParagonData/Anim_Shoot.h"
#include "../ParagonData/Toggle.h"

//Trent
#include "../ParagonData/Anim_tAttack.h"
#include "../ParagonData/Anim_PoisonGas.h"


//Mimic
#include "../ParagonData/Anim_mAttack.h"
#include "../ParagonData/Anim_mChase.h"
#include "../ParagonData/Anim_ThrowCoin.h"