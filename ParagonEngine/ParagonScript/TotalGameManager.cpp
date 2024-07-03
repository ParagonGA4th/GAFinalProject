#include "TotalGameManager.h"

#include "../ParagonData/Scene.h"

#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTween.h"

#include <algorithm>
#include <cassert>
#include <singleton-cpp/singleton.h>

//Handler 시리즈.
#include "BaseAreaHandler.h"
#include "BaseEnemyHandler.h"
#include "BaseGUIHandler.h"
#include "PlayerHandler.h"

//다른 매니저들, etc.
#include "InGameManager.h"
#include "CombatSystem.h"

namespace Pg::DataScript
{
	//이거 실행 전에 GrabManagedObject가 호출되어야 한다.
	void TotalGameManager::Awake()
	{
		{
			static bool tAssureNoDoubleCall = true;
			assert(tAssureNoDoubleCall && "한번만 전 게임 플로우 내부에서 호출되어야");
			tAssureNoDoubleCall = false;
		}
	}

	void TotalGameManager::Start()
	{

	}

	void TotalGameManager::Update()
	{
		// '\' 누르면 치트 고를 수 있을 것
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyNum0))
		{
			SelectCheatCodeWithin();
		}

		//하부 함수 호출 로직 실행.
		if (!_subFunctionStorageVector.empty())
		{
			//비지 않았을 때만 실행.
			for (auto& it : _subFunctionStorageVector)
			{
				//지금까지 하부 컴포넌트에서 호출한 함수를 일괄적으로 실행.
				it();
			}

			_subFunctionStorageVector.clear();
		}

		//그외 추가 기능들.
	}

	void TotalGameManager::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		// 내부적으로 모든 걸 컨트롤해야 한다.
		// 우선 필요한 정보 Initialize.
		//이런 식으로 실질적인 Awake 등 Function 수행.
		if (!_isManagingInitializeCalled)
		{
			Initialize(changedScene);
			_isManagingInitializeCalled = true;
		}

		_pgTween->KillAllTweens();

		//테스트 목적으로 만들어진 Scene이 아니라면.
		if ((changedScene->GetSceneName().compare("ToRemove_GraphicsTest") != 0) &&
			(changedScene->GetSceneName().compare("ToRemove_GraphicsTest.pgscene") != 0))
		{
			//현재 Handler Bundle 받는다. 2D Scene일 경우 nullptr.
			SetCurrentHandlerBundle(changedScene);

			// 해당 Prop / 바뀐 배경 / 바뀐 적 오브젝트들을 모두 리셋할 수 있어야 하고,
			// 플레이어의 스탯 및 위치를 리셋해야 하며.
			// AreaHandler / EnemyHandler / GUIHandler (Stage1 / Stage2 맵 관리 여부 - 상관없이 인터페이스로 빼서 관리하자)
			// 개별 Stage 별로 다르게 관리를 해야 하기에, 인터페이스로 빼는 것이 필수이다.
			// 개별적으로 다른 UI 오브젝트들을 모으는 등, 미리 관리를 하고 있어야 한다. 이를 위해.
			// 이를 위해 GrabManagedObjects() 가 마련되었다.
			// Player Object까지 전체 관리.

			// 초기 상태로 다시 되돌려 놓기. 씬 시작시 시점.
			// 이런 식으로, Flow Control을 담당한다.
			Internal_CallForEntireSceneReset(changedScene, NULL, nullptr);

			//리셋을 명시적으로 호출해줘야.
			//옵저버 클리어만으로 전에 있던 Player / Monster / Projectile 전부 상태 관리에서 리셋된다.
			//이미 Clear된 상태였으면 무시.
			_combatSystem->ResetAll();

			//현재 Handler가 다시 셋된 다음, CombatSystem에 레지스터.
			if (_currentHandlerBundle3d != nullptr)
			{
				// == 3D일때만,
				RegisterCombatSystemUnits();
			}
		}
	}

	void TotalGameManager::Initialize(Pg::Data::Scene* changedScene)
	{
		// 게임이 시작되고, 자신이 있을 수 있는 SceneList들을 알아야 한다.
		//// PgScene 받기.
		_pgScene = &singleton<Pg::API::PgScene>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();


		// 반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		// Scene별로 Bundle들을 받는다.
		SetupBundlesForAllScenes();

		//2D / 3D 따라서 나눠서 관리.
		//게임 매니저를 켤지 말지도 TotalGameManager가 관리한다.

		//다른 매니저들 보관. 같은 오브젝트에 보관하려고 하고 있다.
		//_inGameManager = InGameManager::GetInstance(nullptr);
		
		//Combat System 등, 인 게임 매니저 시스템 받아오기.
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void TotalGameManager::SetupBundlesForAllScenes()
	{
		//개별 씬에 독립적으로 존재하나, 
		//이미 지금 이 시점에서는: 먼저 GrabManagedObject가 호출되어,
		//값을 가져올 수 있어야 한다.
		//ProjectLoad -> EngineMain의 업데이트이기에, 가능한 로직일 것. 
		for (auto& bScene : _pgScene->GetProjectSceneList())
		{
			//이름이 Sample Scene이면 스킵.
			if ((bScene->GetSceneName().compare("SampleScene") == 0) ||
				(bScene->GetSceneName().compare("SampleScene.pgscene") == 0) ||
				(bScene->GetSceneName().compare("ToRemove_GraphicsTest") == 0) ||
				(bScene->GetSceneName().compare("ToRemove_GraphicsTest.pgscene") == 0))
			{
				//SampleScene이면 Handler들을 받지 않는다.
				continue;
			}

			//3D Scene만 받아서, 관리 및 호출을 한다.
			if (!(bScene->GetIs3D()))
			{
				//다만, Title/Credits의 경우 '전체 세트'는 없는 것이 맞다. 
				//공통점 : 둘 다 2D Scene이라는 것.
				//2D 씬들은 GUI만을 세팅하면 될 것이다.
				//GUIHandler만을 투입!
				Pg::DataScript::BaseGUIHandler* tGUIHandler = nullptr;

				for (auto& bObj : bScene->GetObjectList())
				{
					if (tGUIHandler == nullptr)
					{
						tGUIHandler = bObj->GetComponent<BaseGUIHandler>();
					}

					if (tGUIHandler != nullptr)
					{
						break;
					}
				}

				//Scene 별 GUI Handler만 셋업.
				_scene2dHandlerBundleMap.insert(std::make_pair(bScene, tGUIHandler));
			}
			else
			{
				Pg::DataScript::BaseAreaHandler* tAreaHandler = nullptr;
				Pg::DataScript::BaseEnemyHandler* tEnemyHandler = nullptr;
				Pg::DataScript::BaseGUIHandler* tGUIHandler = nullptr;
				Pg::DataScript::PlayerHandler* tPlayerBattleBehavior = nullptr;

				//값 받아서 따로 저장하기. (Handler 시리즈)
				for (auto& bObj : bScene->GetObjectList())
				{
					if (tAreaHandler == nullptr)
					{
						tAreaHandler = bObj->GetComponent<BaseAreaHandler>();
					}

					if (tEnemyHandler == nullptr)
					{
						tEnemyHandler = bObj->GetComponent<BaseEnemyHandler>();
					}

					if (tGUIHandler == nullptr)
					{
						tGUIHandler = bObj->GetComponent<BaseGUIHandler>();
					}

					if (tPlayerBattleBehavior == nullptr)
					{
						tPlayerBattleBehavior = bObj->GetComponent<PlayerHandler>();
					}

					if (tAreaHandler != nullptr &&
						tEnemyHandler != nullptr &&
						tGUIHandler != nullptr &&
						tPlayerBattleBehavior != nullptr)
					{
						//다 있다라고 한다면 비로소 나갈 수 있다.
						break;
					}
				}

				assert((tAreaHandler != nullptr) && "Area Handler가 걸리면 3D 씬인데 없음");
				assert((tEnemyHandler != nullptr) && "Enemy Handler가 걸리면 3D씬인데 없음");
				assert((tGUIHandler != nullptr) && "GUI Handler가 걸리면 3D씬인데 없음");
				assert((tPlayerBattleBehavior != nullptr) && "Player가 걸리면 3D씬인데 없음");

				//이 시점에서는 Handler들 모두 존재.
				std::unique_ptr<HandlerBundle3D> tHandlerBundle = std::make_unique<HandlerBundle3D>();
				tHandlerBundle->_areaHandler = tAreaHandler;
				tHandlerBundle->_enemyHandler = tEnemyHandler;
				tHandlerBundle->_guiHandler = tGUIHandler;
				tHandlerBundle->_playerBehavior = tPlayerBattleBehavior;

				//추후 로직을 위해 소속한 HandlerBundle3D 주소 개별적으로 저장.
				// Area & Enemy.
				tHandlerBundle->_areaHandler->_belongHandlerBundle3D = tHandlerBundle.get();
				tHandlerBundle->_enemyHandler->_belongHandlerBundle3D = tHandlerBundle.get();


				//초기 Player Position 기록.
				tHandlerBundle->_originalPlayerTransStorage = TransformSimpleStorage(&(tPlayerBattleBehavior->_object->_transform));

				//값을 설정한 다음에 투입.
				_scene3dHandlerBundleMap.insert(std::make_pair(bScene, std::move(tHandlerBundle)));
			}
		}
	}

	void TotalGameManager::SetCurrentHandlerBundle(Pg::Data::Scene* changedScene)
	{
		if (changedScene->GetIs3D())
		{
			//3D :현재 Scene에 대응하는 Handler Bundle을 보관.
			_currentHandlerBundle3d = _scene3dHandlerBundleMap.at(changedScene).get();
			_currentGUIHander2d = nullptr;
		}
		else
		{
			//2D : 재 Scene에 대응하는 GUI Handle을 보관.
			_currentGUIHander2d = _scene2dHandlerBundleMap.at(changedScene);
			_currentHandlerBundle3d = nullptr;
		}
	}

	void TotalGameManager::RegisterCombatSystemUnits()
	{
		_combatSystem->RegisterPlayer(_currentHandlerBundle3d->_playerBehavior);
		
		//내부적으로 std::transform처럼 값이 호출될 것. std::function<void(IEnemyBehaviour*)>의 형태일 것이다.
		_currentHandlerBundle3d->_enemyHandler->TransformEachEnemy(std::bind(&CombatSystem::RegisterSingleEnemy, _combatSystem, std::placeholders::_1));
	
		//Projectile은 외적으로 자신이 CombatSystem한테 등록을 해야 할 것이다.
	}

	void TotalGameManager::SelectCheatCodeWithin()
	{
		//백슬래쉬가 입력되었을 때만 내부에 들어온다.
		// '\' + NUM 순서인 것.
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum1))
		{
			_pgScene->SetCurrentScene("Stage1");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum2))
		{
			_pgScene->SetCurrentScene("Stage2");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum3))
		{
			_pgScene->SetCurrentScene("BossStage");
		}
		else if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyNum9))
		{
			_pgScene->SetCurrentScene("ToRemove_GraphicsTest");
		}

		//
	}

	void TotalGameManager::CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer)
	{
		//나중에 일괄적 호출할 수 있도록, 함수 포인터 저장._pgSc3ne->GetKeyDown
		_subFunctionStorageVector.push_back([this, targetScene, potValue, potPointer]()
			{
				Internal_CallForEntireSceneReset(targetScene, potValue, potPointer);
			});
	}

	void TotalGameManager::Internal_CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer)
	{
		//어쨌든 2D / 3D 여부 구분해서 관리해야 한다.
		if (_currentHandlerBundle3d == nullptr)
		{
			//2D 로직.
			assert(_currentGUIHander2d != nullptr);
			_currentGUIHander2d->ResetToInitialState();
			_currentGUIHander2d->AdditionalReset();
		}
		else
		{
			//3D 로직.
			_currentHandlerBundle3d->_areaHandler->ResetToInitialState();
			_currentHandlerBundle3d->_enemyHandler->ResetToInitialState();
			_currentHandlerBundle3d->_guiHandler->ResetToInitialState();
			_currentHandlerBundle3d->_guiHandler->AdditionalReset();

			//Player 위치 원위치.
			_currentHandlerBundle3d->_originalPlayerTransStorage.ToTransform(&(_currentHandlerBundle3d->_playerBehavior->_object->_transform));
			_currentHandlerBundle3d->_playerBehavior->ResetAll();
		}
	}

	Pg::DataScript::HandlerBundle3D* TotalGameManager::GetCurrentHandlerBundle()
	{
		return _currentHandlerBundle3d;
	}

	Pg::DataScript::HandlerBundle3D* TotalGameManager::GetHandlerBundleByScene(Pg::Data::Scene* scene)
	{
		if (scene->GetIs3D())
		{
			return _scene3dHandlerBundleMap.at(scene).get();
		}
		else
		{
			return nullptr;
		}
	}
	

}
