#include "TotalGameManager.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgScene.h"
#include <algorithm>
#include <cassert>
#include <singleton-cpp/singleton.h>

//Handler 시리즈.
#include "IAreaHandler.h"
#include "IEnemyHandler.h"
#include "IGUIHandler.h"
#include "PlayerBattleBehavior.h"

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
		
		//반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");
		
		//게임이 시작되고, 자신이 있을 수 있는 SceneList들을 알아야 한다.
		_pgScene = &singleton<Pg::API::PgScene>();

		//개별 씬에 독립적으로 존재하나, 
		//이미 지금 이 시점에서는: 먼저 GrabManagedObject가 호출되어,
		//값을 가져올 수 있어야 한다.
		//ProjectLoad -> EngineMain의 업데이트이기에, 가능한 로직일 것. 
		for (auto& bScene : _pgScene->GetProjectSceneList())
		{
			//3D Scene만 받아서, 관리 및 호출을 한다.
			if (!(bScene->GetIs3D()))
			{
				//다만, Title/Credits의 경우 없는 것이 맞다. 
				//공통점 : 둘 다 2D Scene이라는 것.
				//2D 씬들은 Handler가 관리하지 않게 할 것이다.
				continue;
			}

			Pg::DataScript::IAreaHandler* tAreaHandler = nullptr;
			Pg::DataScript::IEnemyHandler* tEnemyHandler = nullptr;
			Pg::DataScript::IGUIHandler* tGUIHandler = nullptr;
			Pg::DataScript::PlayerBattleBehavior* tPlayerBattleBehavior = nullptr;

			//값 받아서 따로 저장하기. (Handler 시리즈)
			for (auto& bObj : bScene->GetObjectList())
			{
				if (tAreaHandler == nullptr)
				{
					tAreaHandler = bObj->GetComponent<IAreaHandler>();
				}
				
				if (tEnemyHandler == nullptr)
				{
					tEnemyHandler = bObj->GetComponent<IEnemyHandler>();
				}
				
				if (tGUIHandler == nullptr)
				{
					tGUIHandler = bObj->GetComponent<IGUIHandler>();
				}
				
				if (tPlayerBattleBehavior == nullptr)
				{
					tPlayerBattleBehavior = bObj->GetComponent<PlayerBattleBehavior>();
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
			HandlerBundle tHandlerBundle;
			tHandlerBundle._areaHandler = tAreaHandler;
			tHandlerBundle._enemyHandler = tEnemyHandler;
			tHandlerBundle._guiHandler = tGUIHandler;
			tHandlerBundle._playerBehavior = tPlayerBattleBehavior;

			//값을 설정한 다음에 투입.
			_scene3dHandlerBundleMap.insert(std::make_pair(bScene, tHandlerBundle));
		}
	}

	void TotalGameManager::Start()
	{

	}

	void TotalGameManager::Update()
	{

	}

	void TotalGameManager::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		// 내부적으로 모든 걸 컨트롤해야 한다.

		// 해당 Prop / 바뀐 배경 / 바뀐 적 오브젝트들을 모두 리셋할 수 있어야 하고,

		// 플레이어의 스탯 및 위치를 리셋해야 하며.

		// AreaHandler / EnemyHandler / GUIHandler (Stage1 / Stage2 맵 관리 여부 - 상관없이 인터페이스로 빼서 관리하자)
		// 개별 Stage 별로 다르게 관리를 해야 하기에, 인터페이스로 빼는 것이 필수이다.
		// 개별적으로 다른 UI 오브젝트들을 모으는 등, 미리 관리를 하고 있어야 한다. 이를 위해.
		// 이를 위해 GrabManagedObjects() 가 마련되었다.
		// Player Object까지 전체 관리.


	}


}
