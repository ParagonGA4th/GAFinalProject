#pragma once

///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

//Condition Node
#include "Concrete/DemoConditionComp1.h"
#include "Concrete/Test_CCond_CheckInBound.h"

//SyncActionNode
#include "Concrete/Test_CSync_AddBumpCount.h"
#include "Concrete/Test_CSync_ChooseNewDir.h"
#include "Concrete/Test_CSync_JumpAtBumpLimit.h"
#include "Concrete/Test_CSync_MoveToNewDir.h"
#include "Concrete/Test_CSync_RecordCurrentPos.h"
#include "Concrete/Test_CSync_ReturnToCenter.h"

// Monster Test
#include "../ParagonData/Anim_Idle.h"
#include "../ParagonData/Anim_Walk.h"
//#include "../ParagonData/isHitSpace.h"

