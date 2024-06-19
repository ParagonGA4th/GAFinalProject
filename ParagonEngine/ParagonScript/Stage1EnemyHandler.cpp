#include "Stage1EnemyHandler.h"

#include "../ParagonData/Scene.h"

#include <algorithm>

namespace Pg::DataScript
{
	Stage1EnemyHandler::Stage1EnemyHandler(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void Stage1EnemyHandler::GrabManagedObjects()
	{
		//모든 Enemy들의 리스트를 씬에서 회수한다.
		for (auto& tSingleTag : EnemyDefines::ENEMY_TAG_LIST)
		{
			//값 넣기.
			auto itPair = _managedMonstersList.insert(std::make_pair(tSingleTag, std::vector<EnemyAggregate>()));
			auto& tVecIter = itPair.first->second;
			
			auto tGOVec = _object->GetScene()->FindObjectsWithTag(tSingleTag); 
			tVecIter.reserve(tGOVec.size());
			for (auto& tGO : tGOVec)
			{
				IEnemyBehaviour* tEB = tGO->GetComponent<IEnemyBehaviour>();
				assert(tEB != nullptr && "Enemy는 무조건 IEnemyBehaviour의 하부 구현체가 컴포넌트로 있어야 한다.");
				
				//Enemy Storage 구조체 만들어서, 투입할 준비.
				EnemyAggregate tToInsert;
				tToInsert._enemyBehav = tEB;
				tToInsert._enemyObj = tGO;
				tToInsert._transStorage = TransformSimpleStorage(&(tGO->_transform));

				//내부적으로 원래 Transform을 보관한다.
				tVecIter.push_back(tToInsert);
			}
		}

		// 별개로, Enemy들 내부에서는 따로 GrabManagedObjects()가 호출되어야 하며,
		// 이는 개별 소속한 콜라이더 정보 등을 들고 있어야 한다.
		// 만약 개별 순서가 중요하다면, 별개의 함수를 차례로 호출하는 것까지 생각해야 하고.
	}

	void Stage1EnemyHandler::Start()
	{

	}

	void Stage1EnemyHandler::Update()
	{

	}

	void Stage1EnemyHandler::ResetToInitialState()
	{
		//IEnemy의 ResetAll Function은 원래 자신의 포지션으로 돌아가는 기능이 있어야 한다.
		//하지만, Transform 한정으로 Handler에서 원래대로 돌려보내주게 한다.
		//IEnemyBehaviour의 자식들은 자기 자신의 정보만 돌려보내주게.

		for (auto& [bTagName, bVec] : _managedMonstersList)
		{
			for (auto& tEnemyStorage : bVec)
			{
				//원래대로 Transform 바꿔주기.
				tEnemyStorage._transStorage.ToTransform(&(tEnemyStorage._enemyObj->_transform));

				//다시 원래대로 본인의 정보 돌려놓기.
				tEnemyStorage._enemyBehav->ReturnBaseMonsterInfo()->ResetAll();
			}
		}
	}

}