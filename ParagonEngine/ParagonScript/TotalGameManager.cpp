#include "TotalGameManager.h"
#include "../ParagonData/Scene.h"
#include "../ParagonAPI/PgScene.h"
#include <algorithm>
#include <cassert>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	void TotalGameManager::Awake()
	{
		//반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		//게임이 시작되고, 자신이 있을 수 있는 SceneList들을 알아야 한다.
		_pgScene = &singleton<Pg::API::PgScene>();

		//개별 씬에 독립적으로 존재하나, 
		//이미 지금 이 시점에서는: GrabManagedObject가 호출되어,
		//값을 가져올 수 있어야 한다.
		//ProjectLoad -> EngineMain의 업데이트이기에, 가능한 로직일 것. 
		for (auto& it : _pgScene->GetProjectSceneList())
		{
			//값 받아서 따로 저장하기.
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

		// AreaController / EnemySpawner / UIManager (Stage1 / Stage2 맵 관리 여부 - 상관없이 인터페이스로 빼서 관리하자)
		// 개별 Stage 별로 다르게 관리를 해야 하기에, 인터페이스로 빼는 것이 필수이다.
		// 개별적으로 다른 UI 오브젝트들을 모으는 등, 미리 관리를 하고 있어야 한다. 이를 위해.
		// 이를 위해 GrabManagedObjects() 가 마련되었다.


	}


}
