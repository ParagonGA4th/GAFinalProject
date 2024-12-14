#include "BaseEnemyHandler.h"
#include "BaseAreaHandler.h"
#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"
#include "HandleBundle3D.h"

namespace Pg::DataScript
{
	BaseEnemyHandler::BaseEnemyHandler(unsigned int entireAreaCount) : _entireAreaCount(entireAreaCount)
	{
		//
	}

	void BaseEnemyHandler::GrabOrganizeAllEnemies(Pg::Data::GameObject* obj)
	{
		Pg::Data::GameObject* _object = obj;

		//��� Enemy���� ����Ʈ�� ������ ȸ���Ѵ�.
		//AreaCount - Tag�� �ݿ��Ͽ� �ʿ��� �����̳ʵ� ��� �����.
		for (int i = 0; i < _entireAreaCount; i++)
		{
			auto bPair = _managedMonstersList.insert(std::make_pair(i, std::unordered_map<std::string, std::vector<EnemyAggregate>>()));
			auto& bMap = bPair.first->second;
			for (auto& tSingleTag : EnemyDefines::ENEMY_TAG_LIST)
			{
				//Vector�� ��� �����ϴ� Tag���� ����.
				bMap.insert(std::make_pair(tSingleTag, std::vector<EnemyAggregate>()));
			}
		}

		//Tag���� ������Ʈ�� �޾ƿͼ�, Index ���� �켱 ���ĵǾ� �ִ� ��� �����̳ʿ� �����Ѵ�.
		for (auto& tSingleTag : EnemyDefines::ENEMY_TAG_LIST)
		{
			//��� ��� Area�� ���� �����ϰ� �ȴ� : Single Tag �������� ��ȯ.
			auto tAllAreaOfSingleTagGOVec = _object->GetScene()->FindObjectsWithTag(tSingleTag);
			std::vector<std::pair<Pg::Data::GameObject*, IEnemyBehaviour*>> tAllAreaOfSingleTagEnemyBehavVec;

			//������Ʈ�� ������ Size.
			//tAllAreaOfSingleTagEnemyBehavVec.resize(tAllAreaOfSingleTagGOVec.size());

			// ��� GameObject�� ���� �ִ� �ֵ�: -> IEnemyBehaviour�� ������ �־�� �ϴµ�,
			// �̸� ��� �ִ� ���� �ε����� ����. (Vector ������Ʈ�� �ű��)
			for (int i = 0; i < tAllAreaOfSingleTagGOVec.size(); i++)
			{
				//Enemy Behaviour ��ü�� �޾ƿ���, Assert ���� ���� �ؾ�.
				auto& bObject = tAllAreaOfSingleTagGOVec.at(i);

				if (bObject->_transform.HasParent())
				{
					// Parent�� ������ ���� �ʴ´�.
					continue;
				}

				IEnemyBehaviour* tEnBehav = bObject->GetComponent<IEnemyBehaviour>();
				assert((tEnBehav != nullptr) && "Enemy Tag�� ������ ������, ������ IEnemyBehaviour�� ���� Script�� ������Ʈ�� �־��!");
				tAllAreaOfSingleTagEnemyBehavVec.push_back(std::make_pair(bObject, tEnBehav));
			}

			//Index�� Area ID / �������� �ش��ϴ� Vector��.
			//�翬�� �ϳ��� Tag�� �Ҽ��� �� ���̴�.
			std::vector<std::vector<std::pair<Pg::Data::GameObject*, IEnemyBehaviour*>>> tAreaIndex_EnemyBehavVec;
			tAreaIndex_EnemyBehavVec.resize(_entireAreaCount);

			//���������� Area���� �ٸ��� �޾ƾ� �Ѵ�.
			//Object�� IEnemyBehaviour�� ���� ���.
			for (auto& [bObj, bBehav] : tAllAreaOfSingleTagEnemyBehavVec)
			{
				unsigned int tGetIndex = bBehav->GetBelongAreaIndex();
				assert(tGetIndex < _entireAreaCount);
				auto& bAppVec = tAreaIndex_EnemyBehavVec.at(tGetIndex);
				bAppVec.push_back(std::make_pair(bObj, bBehav));
			}

			//�� �ֱ� : ���� ������ ������, �̸� �������� �����ؼ� ������ �ȴ�.
			for (int tAreaID = 0; tAreaID < _entireAreaCount; tAreaID++)
			{
				//�ε����� IEnemyBehaviour. - Object : ���� X.
				auto& tBehavPairVec = tAreaIndex_EnemyBehavVec.at(tAreaID);
				auto& bSingleAreaIndexMap = _managedMonstersList.at(tAreaID);
				auto& tActualStoreVecIter = bSingleAreaIndexMap.at(tSingleTag);

				//���⿡�ٰ��� ������ ���� ����ȴ�.
				tActualStoreVecIter.reserve(tBehavPairVec.size());

				for (auto& [tGO, tEB] : tBehavPairVec)
				{
					if (tGO->_transform.GetParent() != nullptr)
					{
						//�θ� ������ ���� ����Ʈ���� ������.
						continue;
					}

					//Enemy Storage ����ü ����, ������ �غ�.
					EnemyAggregate tToInsert;
					tToInsert._enemyBehav = tEB;
					tToInsert._enemyObj = tGO;
					tToInsert._transStorage = TransformSimpleStorage(&(tGO->_transform));
					tToInsert._isAlive = true; //������ ��� �ְ� ����.

					//���������� ���� Transform�� �����Ѵ�.
					tActualStoreVecIter.push_back(tToInsert);
				}
			}
		}

		// �� Enemy�� ���ο����� ���� GrabManagedObjects()�� ȣ��Ǿ�� �ϸ�,
		// �̴� ���� �Ҽ��� �ݶ��̴� ���� ���� ��� �־�� �Ѵ�.
		// ���� ���� ������ �߿��ϴٸ�, ������ �Լ��� ���ʷ� ȣ���ϴ� �ͱ��� �����ؾ� �ϰ�.
	}

	void BaseEnemyHandler::ResetToInitialState()
	{
		//IEnemy�� ResetAll Function�� ���� �ڽ��� ���������� ���ư��� ����� �־�� �Ѵ�.
		//������, Transform �������� Handler���� ������� ���������ְ� �Ѵ�.
		//IEnemyBehaviour�� �ڽĵ��� �ڱ� �ڽ��� ������ ���������ְ�.

		for (int i = 0; i < _entireAreaCount; i++)
		{
			auto& bMap = _managedMonstersList.at(i);
			for (auto& [bTagName, bVec] : bMap)
			{
				for (auto& tEnemyStorage : bVec)
				{
					//������� Transform �ٲ��ֱ�.
					tEnemyStorage._transStorage.ToTransform(&(tEnemyStorage._enemyObj->_transform));

					//�ٽ� ������� ������ ���� ��������. ���ο��� SetActive �ʿ����־�� �Ѵ�. 
					tEnemyStorage._enemyBehav->ReturnBaseMonsterInfo()->ResetAll();
					tEnemyStorage._enemyBehav->ResetAll();

					//�ٽ� ��� �ְ� _isAlive ������� ���.
					tEnemyStorage._isAlive = true;
				}
			}
		}
	}

	void BaseEnemyHandler::TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func)
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

	void BaseEnemyHandler::FromEnemyNotifyDead(const std::string& tagName, IEnemyBehaviour* behav)
	{
		//�ϴ� �������� �� ���.
		unsigned int tAreaIndex = behav->GetBelongAreaIndex();
		auto& bBelongAreaMap = _managedMonstersList.at(tAreaIndex);
		auto& bBelongTagVector = bBelongAreaMap.at(tagName);

		PG_ERROR("called dead");

		for (auto& it : bBelongTagVector)
		{
			if (it._enemyBehav == behav)
			{
				//�׾��ٰ� ���. ���߿� ������ �� �̺�Ʈ�� ���¿� ���� ����� ��.
				it._isAlive = false;

				std::string tMSG = it._enemyObj->GetName() + std::string("called dead");
				PG_ERROR(tMSG);
			}
		}
		
		//����, AreaMap �Ľ��ؼ�, �� Area �ȿ��� ��� ���� �������� Ȯ��. 
		//���� �׷� ��� -> BaseAreaHandler�� ȣ���� ��.
		bool tIsAtLeastOneAlive = false;
		for (auto& [bTagName, bAggVec] : bBelongAreaMap)
		{
			for (auto& it : bAggVec)
			{
				if (it._isAlive)
				{
					tIsAtLeastOneAlive = true;
					//Nested For Goto Escape.
					goto gFoundPoint;
				}
			}
		}

	gFoundPoint:
		//�� �ϳ��� ��� �ִ� ��ü�� ���ٸ�, �ش� ��Ȳ�� AreaHandler�� �Լ��� ȣ��.
		if (!tIsAtLeastOneAlive)
		{
			_belongHandlerBundle3D->_areaHandler->SetActivateConfinedAreaIndex(tAreaIndex, false);
		}
	}

	void BaseEnemyHandler::ResetIfNotDead()
	{
		//���� �ʾҴٸ� �� �����ϱ�. 
		//if ()
	}

}
