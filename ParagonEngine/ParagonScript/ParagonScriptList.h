#pragma once

///앞으로 추가되는 모든 Script들이 Compile-Time Initialization이 되기 위해, 
///Script의 자식들이 모두 #include되어야 하는 곳.
/// 
///Paragon Script의 요구 조건:
///Script Class가 : ScriptInterface<자신>을 상속받아야 한다.
///Class 구현체 "헤더" 내부에 DEFINE_PARAGON_SCRIPT(T);가 선언되어 있어야 하고,
///ParagonScriptList.h의 내부에 해당 헤더가 포함되어 있어야 한다!

#include "PlayerMove.h"
#include "EventTest.h"
#include "RayCastTest.h"
#include "AddEffectorTest.h"
//실제 사용될 Content용 Script들.
//#include "PlayerMovementSector.h"
#include "InGameCameraBehavior.h"
#include "CameraShake.h"
#include "ComboSystem.h"
#include "ArrowLogic.h"
#include "UltimateArrowLogic.h"
#include "ArrowShooterBehavior.h"
#include "BattleArea.h"
#include "TrapArea.h"
#include "EnemyBehaviour.h"
#include "EnemySight.h"
#include "Portal.h"
#include "AreaPassingTrigger.h"
#include "DeathPlane.h"
#include "ArtifactBox.h"
#include "ArtifactBoxAnim.h"
#include "MushroomTrampoline.h"
#include "StrongAttackLogic.h"
#include "Stair.h"
#include "Stage1_Bridge.h"
#include "Stage2_Bridge.h"
#include "BridgeHelper.h"
#include "Grating.h"

//UI 관련 (개별)
#include "TitleScene.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "PauseBox.h"
#include "PauseOptionBox.h"
#include "NavAgent.h"
#include "GUI_PercentageFillBar.h"
#include "GUI_CoolDownWhiteFill.h"
#include "GUI_StrongAttackStatus.h"
#include "SetInGameSortingLayer.h"

//사운드 관련
#include "SoundManager.h"

//몬스터 관련
#include "BossBehaviour.h"
#include "EnemyAttack.h"
#include "MiniGolemBehaviour.h"
#include "GolemBossBehaviour.h"
#include "StubBehaviour.h"
#include "MimicBehaviour.h"
#include "WaspBehaviour.h"
#include "MiniGolemBehaviour.h"
#include "MiniGolemAttack.h"
#include "TrentAttack.h"
#include "TrentSkillAttack.h"
#include "MimicAttack.h"
#include "MimicSkillAttack.h"
#include "MimicActive.h"
#include "WaspAttack.h"
#include "BossAttack.h"
#include "WindBlastAttack.h"
#include "LightSkillAttack.h"
#include "TakeDownAttack.h"
#include "GolemBossAttack.h"
#include "GolemBossSkillAttack.h"

//글로벌 매니저용
#include "TotalGameManager.h"
#include "InGameManager.h"
#include "CombatSystem.h"
#include "GlobalSoundManager.h"
//로컬 매니저용.

//TitleScene - 2D
#include "TitleSceneGUIHandler.h"

//Stage1Scene - 3D
#include "Stage1AreaHandler.h"
#include "Stage1EnemyHandler.h"
#include "Stage1GUIHandler.h"

//Stage2Scene - 3D
#include "Stage2AreaHandler.h"
#include "Stage2EnemyHandler.h"
#include "Stage2GUIHandler.h"

//BossStage - 3D
#include "BossStageAreaHandler.h"
#include "BossStageEnemyHandler.h"
#include "BossStageGUIHandler.h"

//CreditsScene - 2D
#include "CreditsSceneGUIHandler.h"
