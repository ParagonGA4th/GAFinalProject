#pragma once

///자체적인 클래스X,
///추가되는 모든 Custom Behavior Tree 노드들을 일괄적으로 하나의 헤더에 추가해서 나중에 추가하는 것이 가능하게 하려고.

//Condition Node
#include "DemoConditionComp1.h"
#include "Test_CCond_CheckInBound.h"

//SyncActionNode
#include "Test_CSync_AddBumpCount.h"
#include "Test_CSync_ChooseNewDir.h"
#include "Test_CSync_JumpAtBumpLimit.h"
#include "Test_CSync_MoveToNewDir.h"
#include "Test_CSync_RecordCurrentPos.h"
#include "Test_CSync_ReturnToCenter.h"


