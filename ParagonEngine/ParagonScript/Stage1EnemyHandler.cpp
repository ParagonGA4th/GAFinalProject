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
		GrabOrganizeAllEnemies();
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

		for (int i = 0; i < _entireAreaCount; i++)
		{
			auto& bMap = _managedMonstersList.at(i);
			for (auto& [bTagName, bVec] : bMap)
			{
				for (auto& tEnemyStorage : bVec)
				{
					//원래대로 Transform 바꿔주기.
					tEnemyStorage._transStorage.ToTransform(&(tEnemyStorage._enemyObj->_transform));

					//다시 원래대로 본인의 정보 돌려놓기. 내부에서 SetActive 필요해주어야 한다. 
					tEnemyStorage._enemyBehav->ReturnBaseMonsterInfo()->ResetAll();
				}
			}
		}
	}

	void Stage1EnemyHandler::TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func)
	{
		for (int i = 0; i < _entireAreaCount; i++)
		{
			auto& bMap = _managedMonstersList.at(i);
			for (auto& [bTagName, bVec] : bMap)
			{
				for (auto& tEnemyStorage : bVec)
				{
					func(tEnemyStorage._enemyBehav);
				}
			}
		}
	}

	void Stage1EnemyHandler::GrabOrganizeAllEnemies()
	{
		//모든 Enemy들의 리스트를 씬에서 회수한다.
		//AreaCount - Tag를 반영하여 필요한 컨테이너들 모두 만든다.
		for (int i = 0; i < _entireAreaCount; i++)
		{
			auto bPair = _managedMonstersList.insert(std::make_pair(i, std::unordered_map<std::string, std::vector<EnemyAggregate>>()));
			auto& bMap = bPair.first->second;
			for (auto& tSingleTag : EnemyDefines::ENEMY_TAG_LIST)
			{
				//Vector들 모든 존재하는 Tag마다 투입.
				bMap.insert(std::make_pair(tSingleTag, std::vector<EnemyAggregate>()));
			}
		}

		//Tag별로 오브젝트를 받아와서, Index 별로 우선 정렬되어 있는 멤버 컨테이너에 저장한다.
		for (auto& tSingleTag : EnemyDefines::ENEMY_TAG_LIST)
		{
			//얘는 모든 Area를 전부 포함하게 된다 : Single Tag 기준으로 반환.
			auto tAllAreaOfSingleTagGOVec = _object->GetScene()->FindObjectsWithTag(tSingleTag);
			std::vector<std::pair<Pg::Data::GameObject*, IEnemyBehaviour*>> tAllAreaOfSingleTagEnemyBehavVec;

			//오브젝트와 동일한 Size.
			tAllAreaOfSingleTagEnemyBehavVec.resize(tAllAreaOfSingleTagGOVec.size());

			//모든 GameObject를 갖고 있는 애들: -> IEnemyBehaviour를 가지고 있어야 하는데,
			//이를 담고 있는 동일 인덱스로 설정. (Vector 컴포넌트로 옮기기)
			for (int i = 0; i < tAllAreaOfSingleTagEnemyBehavVec.size(); i++)
			{
				//Enemy Behaviour 객체를 받아오기, Assert 점검 역시 해야.
				auto& bObject = tAllAreaOfSingleTagGOVec.at(i);
				IEnemyBehaviour* tEnBehav = bObject->GetComponent<IEnemyBehaviour>();
				assert((tEnBehav != nullptr) && "Enemy Tag를 가지고 있으면, 무조건 IEnemyBehaviour를 가진 Script가 컴포넌트로 있어야!");
				tAllAreaOfSingleTagEnemyBehavVec.at(i) = std::make_pair(bObject, tEnBehav);
			}

			//Index는 Area ID / 나머지는 해당하는 Vector들.
			//당연히 하나의 Tag에 소속이 될 것이다.
			std::vector<std::vector<std::pair<Pg::Data::GameObject*, IEnemyBehaviour*>>> tAreaIndex_EnemyBehavVec;
			tAreaIndex_EnemyBehavVec.resize(_entireAreaCount);

			//개별적으로 Area마다 다르게 받아야 한다.
			//Object와 IEnemyBehaviour를 따로 기록.
			for (auto& [bObj, bBehav] : tAllAreaOfSingleTagEnemyBehavVec)
			{
				unsigned int tGetIndex = bBehav->GetBelongAreaIndex();
				assert(tGetIndex < _entireAreaCount);
				auto& bAppVec = tAreaIndex_EnemyBehavVec.at(tGetIndex);
				bAppVec.push_back(std::make_pair(bObj, bBehav));
			}

			//값 넣기 : 이제 나뉘어 있으니, 이를 기점으로 관리해서 넣으면 된다.
			for (int tAreaID = 0; tAreaID < _entireAreaCount; tAreaID++)
			{
				//인덱스별 IEnemyBehaviour. - Object : 문제 X.
				auto& tBehavPairVec = tAreaIndex_EnemyBehavVec.at(tAreaID);
				auto& bSingleAreaIndexMap = _managedMonstersList.at(tAreaID);
				auto& tActualStoreVecIter = bSingleAreaIndexMap.at(tSingleTag);

				//여기에다가는 실제로 값이 저장된다.
				tActualStoreVecIter.reserve(tBehavPairVec.size());

				for (auto& [tGO, tEB] : tBehavPairVec)
				{
					if (tGO->_transform.GetParent() != nullptr)
					{
						//부모 있으면 관리 리스트에서 빠진다.
						continue;
					}

					//Enemy Storage 구조체 만들어서, 투입할 준비.
					EnemyAggregate tToInsert;
					tToInsert._enemyBehav = tEB;
					tToInsert._enemyObj = tGO;
					tToInsert._transStorage = TransformSimpleStorage(&(tGO->_transform));

					//내부적으로 원래 Transform을 보관한다.
					tActualStoreVecIter.push_back(tToInsert);
				}
			}
		}
		// 별 Enemy들 내부에서는 따로 GrabManagedObjects()가 호출되어야 하며,
		// 이는 개별 소속한 콜라이더 정보 등을 들고 있어야 한다.
		// 만약 개별 순서가 중요하다면, 별개의 함수를 차례로 호출하는 것까지 생각해야 하고.
	}

}