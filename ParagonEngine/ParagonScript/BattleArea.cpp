#include "BattleArea.h"
#include "PlayerHandler.h"
#include "CombatSystem.h"
#include "EventList_PlayerRelated.h"
#include "EventList_GameFlowRelated.h"

#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/SphereCollider.h"

#include "../ParagonUtil/Log.h"

#include "TotalGameManager.h"
#include "BaseEnemyHandler.h"
#include "BaseAreaHandler.h"

namespace Pg::DataScript
{
	BattleArea::BattleArea(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void BattleArea::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void BattleArea::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}


	void BattleArea::GrabManagedObjects()
	{
		_combatSystem = CombatSystem::GetInstance(nullptr);
		_boss = _object->GetScene()->FindObjectWithName("Boss");
	}

	void BattleArea::Awake()
	{
		//자신이 속한 곳의 AreaHandler / EnemyHandler를 받아오기.
		//적 보고 로직 등에 사용될 것.
		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_areaHandler = tHB->_areaHandler;
			assert(_areaHandler != nullptr);
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}

		_collider = _object->GetComponent<Pg::Data::SphereCollider>();
		assert(_collider != nullptr);
	}

	void BattleArea::Start()
	{

	}

	void BattleArea::Update()
	{
		// 1스테이지에서는 플레이어가 전투 구역에서 벗어나지 못하도록 막아주는 투명벽이 있음
		// 플레이어가 전투 구역에 있는 모든 몬스터를 해치웠을 경우 빠져나갈 수 있음

		if (_onTriggerStay)
		{
			if (_player != nullptr)
			{
				if (_isActivated)
				{
					if (_object->GetScene()->GetSceneName() != "Stage2")
					{
						ConfinePlayer();

						if (_object->GetScene()->GetSceneName() == "BossStage")
						{
							ConfineBoss();
						}
					}
				}
				else
				{
					PG_ERROR("EXITNOW");
					DeactivateArea();
					if (_object->GetScene()->GetSceneName() == "Stage1" && _areaIndex == 0)
						_combatSystem->Post(Event_PlayerGetArtifact(), NULL, NULL);
				}
			}
		}
	}

	void BattleArea::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Sensor")
			{
				//이 자체는 다시 플레이어가 갈 수 있는 영역 한정 위함.
				_onTriggerStay = true;
				_player = col->_object->_transform.GetParent()->_object->GetComponent<Pg::DataScript::PlayerHandler>();

				//Golem Boss일 경우,
				if ((_areaIndex == 1))
				{
					//Invoke. 
					_combatSystem->Post(Event_OnGolemBossGameAreaEnter(), NULL, NULL);
				}
				//Final Boss일 경우,
				if ((_areaIndex == 4))
				{
					//Invoke. 

				}
				//밑은 적 등록 / 모두 죽일 시 : 판단 로직을 마련 위함.
				//이때는, Enemy가 죽었을 때 Handler에게 죽었다고 알려주는 로직이 필요.
			}

			if (col->_object->GetTag() == "TAG_Boss")
			{
				auto dCol = _boss->GetComponent<Pg::Data::DynamicCollider>();
				dCol = col->_object->GetComponent<Pg::Data::DynamicCollider>();
			}

		}
	}

	void BattleArea::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
		//for (int i = 0; i < count; i++)
		//{
		//	Pg::Data::Collider* col = _colArr[i];
		//	if (col->_object->GetName() == "PlayerSensor")
		//	{
		//		_onTriggerStay = false;
		//	}
		//}
	}

	unsigned int BattleArea::GetDesignatedAreaIndex()
	{
		return _areaIndex;
	}

	void BattleArea::ResetAll()
	{
		//다시 값 돌려놓기.
		_isActivated = true;

		//Collider 여부는 Confine에서 해결됨.

		//Stay 끄기.
		_onTriggerStay = false;
	}

	void BattleArea::ConfinePlayer()
	{
		auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();

		//만약 SetActive가 True가 아니면 Set.
		if (!dcol->GetActive())
		{
			dcol->SetActive(true);
		}

		auto& spherePos = _collider->_object->_transform._position;
		auto& boxPos = _player->_object->_transform._position;

		float sphereRadius = _collider->GetRadius();
		float boxHalfWidth = dcol->GetWidth() / 2;
		float boxHalfDepth = dcol->GetDepth() / 2;

		// 구의 중심에서 박스의 중심까지의 벡터
		float dx = boxPos.x - spherePos.x;
		float dz = boxPos.z - spherePos.z;
		float distanceSquared = dx * dx + dz * dz;

		// 박스의 각 모서리가 구의 경계를 넘지 않도록 조정
		float radiusMinusHalfDiagonal = sphereRadius - sqrt(boxHalfWidth * boxHalfWidth + boxHalfDepth * boxHalfDepth);

		if (distanceSquared > radiusMinusHalfDiagonal * radiusMinusHalfDiagonal)
		{
			// 박스를 구의 내부로 밀어 넣기
			float distance = sqrt(distanceSquared);
			float overlap = distance - radiusMinusHalfDiagonal;

			// 거리 벡터를 정규화
			float invDistance = 1.0f / distance;
			float normX = dx * invDistance;
			float normZ = dz * invDistance;

			// 박스 위치를 구의 내부로 밀어 넣기
			boxPos.x -= normX * overlap;
			boxPos.z -= normZ * overlap;
		}
	}

	void BattleArea::ConfineBoss()
	{
		auto dcol = _boss->GetComponent<Pg::Data::DynamicCollider>();

		//만약 SetActive가 True가 아니면 Set.
		if (!dcol->GetActive())
		{
			dcol->SetActive(true);
		}

		auto& spherePos = _collider->_object->_transform._position;
		auto& boxPos = _player->_object->_transform._position;

		float sphereRadius = _collider->GetRadius();
		float boxHalfWidth = dcol->GetWidth() / 2;
		float boxHalfDepth = dcol->GetDepth() / 2;

		// 구의 중심에서 박스의 중심까지의 벡터
		float dx = boxPos.x - spherePos.x;
		float dz = boxPos.z - spherePos.z;
		float distanceSquared = dx * dx + dz * dz;

		// 박스의 각 모서리가 구의 경계를 넘지 않도록 조정
		float radiusMinusHalfDiagonal = sphereRadius - sqrt(boxHalfWidth * boxHalfWidth + boxHalfDepth * boxHalfDepth);

		if (distanceSquared > radiusMinusHalfDiagonal * radiusMinusHalfDiagonal)
		{
			// 박스를 구의 내부로 밀어 넣기
			float distance = sqrt(distanceSquared);
			float overlap = distance - radiusMinusHalfDiagonal;

			// 거리 벡터를 정규화
			float invDistance = 1.0f / distance;
			float normX = dx * invDistance;
			float normZ = dz * invDistance;

			// 박스 위치를 구의 내부로 밀어 넣기
			boxPos.x -= normX * overlap;
			boxPos.z -= normZ * overlap;
		}
	}

	void BattleArea::DeactivateArea()
	{
		//이제 다시 들어오기 전까지는 못 오게.
		_onTriggerStay = false;

		//콜라이더 끄기.
		_collider->SetActive(false);

		//감옥 보내기.
		_object->_transform._position.y = -10000.f;
	}

	void BattleArea::SetActivate(bool val)
	{	
		_isActivated = val;
	}

}

