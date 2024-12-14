#pragma once

///������ �߰��Ǵ� ��� Script���� Compile-Time Initialization�� �Ǳ� ����, 
///Script�� �ڽĵ��� ��� #include�Ǿ�� �ϴ� ��.
/// 
///Paragon Script�� �䱸 ����:
///Script Class�� : ScriptInterface<�ڽ�>�� ��ӹ޾ƾ� �Ѵ�.
///Class ����ü "���" ���ο� DEFINE_PARAGON_SCRIPT(T);�� ����Ǿ� �־�� �ϰ�,
///ParagonScriptList.h�� ���ο� �ش� ����� ���ԵǾ� �־�� �Ѵ�!

#include "PlayerMove.h"
#include "EventTest.h"
#include "RayCastTest.h"
#include "AddEffectorTest.h"
//���� ���� Content�� Script��.
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
#include "StairHelper.h"
#include "Stage1_Bridge.h"
#include "Stage2_Bridge.h"
#include "BridgeHelper.h"
#include "Grating_1.h"
#include "Grating_2.h"

//UI ���� (����)
#include "TitleScene.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "PauseBox.h"
#include "PauseOptionBox.h"
#include "NavAgent.h"
#include "TitleTutorial.h"
#include "GUI_PercentageFillBar.h"
#include "GUI_CoolDownWhiteFill.h"
#include "GUI_StrongAttackStatus.h"
#include "SetInGameSortingLayer.h"

//���� ����
#include "SoundManager.h"

//���� ����
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

//�۷ι� �Ŵ�����
#include "TotalGameManager.h"
#include "InGameManager.h"
#include "CombatSystem.h"
#include "GlobalSoundManager.h"
//���� �Ŵ�����.

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

//GameOverScene - 2D
#include "GameOverSceneGUIHandler.h"
