#include "BattleArea.h"
#include "PlayerHandler.h"
#include "CombatSystem.h"
#include "EventList_PlayerRelated.h"
#include "EventList_GameFlowRelated.h"

#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/SphereCollider.h"
#include "../ParagonAPI/PgTime.h"

#include "../ParagonUtil/Log.h"

#include "TotalGameManager.h"
#include "BaseEnemyHandler.h"
#include "BaseAreaHandler.h"
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	BattleArea::BattleArea(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
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
		if (_object->GetScene()->GetSceneName() == "Stage1")
		{
			auto _killMonster = _object->GetScene()->FindObjectWithName("KillMonsterUI");
			_killMonsterUI = _killMonster->GetComponent<Pg::Data::ImageRenderer>();
			assert(_killMonsterUI != nullptr);

			auto _golemBoss = _object->GetScene()->FindObjectWithName("GolemBossUI");
			_golemBossUI = _golemBoss->GetComponent<Pg::Data::ImageRenderer>();
			assert(_golemBossUI != nullptr);

			_killMonsterUI->SetActive(false);
			_golemBossUI->SetActive(false);
		}
		//�ڽ��� ���� ���� AreaHandler / EnemyHandler�� �޾ƿ���.
		//�� ���� ���� � ���� ��.
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

		_isUI = true;

	}

	void BattleArea::Start()
	{

	}

	void BattleArea::Update()
	{
		// 1�������������� �÷��̾ ���� �������� ����� ���ϵ��� �����ִ� ������ ����
		// �÷��̾ ���� ������ �ִ� ��� ���͸� ��ġ���� ��� �������� �� ����

		if (_onTriggerStay)
		{
			if (_player != nullptr)
			{
				if (_isActivated)
				{
					if (_object->GetScene()->GetSceneName() == "Stage1" && _areaIndex == 0)
					{
						_killMonsterUI->SetActive(true);
					}
					else if (_object->GetScene()->GetSceneName() == "Stage1" && _areaIndex == 1)
					{
						if (_isUI)
						{
							_golemBossUI->SetActive(true);
						}

						dTime += _pgTime->GetDeltaTime();

						if (dTime >= 3.f)
						{
							_golemBossUI->SetActive(false);
							_isUI = false;
							dTime = 0.f;
						}
					}

					if (_object->GetScene()->GetSceneName() != "Stage2")
					{
						ConfinePlayer();
					}

					if (_object->GetScene()->GetSceneName() == "BossStage")
					{
						PG_TRACE(_object->GetScene()->GetSceneName());
						_player->SetPlayerMoveSpeed(6.f);
						ConfineBoss();
					}
				}
				else
				{
					PG_ERROR("EXITNOW");
					DeactivateArea();
					if (_object->GetScene()->GetSceneName() == "Stage1" && _areaIndex == 0)
					{
						_combatSystem->Post(Event_PlayerGetArtifact(), NULL, NULL);
						_killMonsterUI->SetActive(false);
					}
					if (_object->GetScene()->GetSceneName() == "Stage1" && _areaIndex == 0)
					{
						_golemBossUI->SetActive(false);
					}
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
				//�� ��ü�� �ٽ� �÷��̾ �� �� �ִ� ���� ���� ����.
				_onTriggerStay = true;
				_player = col->_object->_transform.GetParent()->_object->GetComponent<Pg::DataScript::PlayerHandler>();

				//Golem Boss�� ���,
				if ((_areaIndex == 1))
				{
					//Invoke. 
					_combatSystem->Post(Event_OnGolemBossGameAreaEnter(), NULL, NULL);
				}
				//Final Boss�� ���,
				if ((_areaIndex == 4))
				{
					//Invoke. 

				}
				//���� �� ��� / ��� ���� �� : �Ǵ� ������ ���� ����.
				//�̶���, Enemy�� �׾��� �� Handler���� �׾��ٰ� �˷��ִ� ������ �ʿ�.
			}

			//if (col->_object->GetTag() == "TAG_Boss")
			//{
			//	_boss = col->_object;
			//}
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
		//�ٽ� �� ��������.
		_isActivated = true;

		//Collider ���δ� Confine���� �ذ��.

		//Stay ����.
		_onTriggerStay = false;
	}

	void BattleArea::ConfinePlayer()
	{
		auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();

		//���� SetActive�� True�� �ƴϸ� Set.
		if (!dcol->GetActive())
		{
			dcol->SetActive(true);
		}

		auto& spherePos = _collider->_object->_transform._position;
		auto& boxPos = _player->_object->_transform._position;

		float sphereRadius = _collider->GetRadius();
		float boxHalfWidth = dcol->GetWidth() / 2;
		float boxHalfDepth = dcol->GetDepth() / 2;

		// ���� �߽ɿ��� �ڽ��� �߽ɱ����� ����
		float dx = boxPos.x - spherePos.x;
		float dz = boxPos.z - spherePos.z;
		float distanceSquared = dx * dx + dz * dz;

		// �ڽ��� �� �𼭸��� ���� ��踦 ���� �ʵ��� ����
		float radiusMinusHalfDiagonal = sphereRadius - sqrt(boxHalfWidth * boxHalfWidth + boxHalfDepth * boxHalfDepth);

		if (distanceSquared > radiusMinusHalfDiagonal * radiusMinusHalfDiagonal)
		{
			// �ڽ��� ���� ���η� �о� �ֱ�
			float distance = sqrt(distanceSquared);
			float overlap = distance - radiusMinusHalfDiagonal;

			// �Ÿ� ���͸� ����ȭ
			float invDistance = 1.0f / distance;
			float normX = dx * invDistance;
			float normZ = dz * invDistance;

			// �ڽ� ��ġ�� ���� ���η� �о� �ֱ�
			boxPos.x -= normX * overlap;
			boxPos.z -= normZ * overlap;
		}
	}

	void BattleArea::ConfineBoss()
	{
		auto dcol = _boss->GetComponent<Pg::Data::DynamicCollider>();

		//���� SetActive�� True�� �ƴϸ� Set.
		if (!dcol->GetActive())
		{
			dcol->SetActive(true);
		}

		auto& spherePos = _collider->_object->_transform._position;
		auto& boxPos = _boss->_transform._position;

		float sphereRadius = _collider->GetRadius();
		float boxHalfWidth = dcol->GetWidth() / 2;
		float boxHalfDepth = dcol->GetDepth() / 2;

		// ���� �߽ɿ��� �ڽ��� �߽ɱ����� ����
		float dx = boxPos.x - spherePos.x;
		float dz = boxPos.z - spherePos.z;
		float distanceSquared = dx * dx + dz * dz;

		// �ڽ��� �� �𼭸��� ���� ��踦 ���� �ʵ��� ����
		float radiusMinusHalfDiagonal = sphereRadius - sqrt(boxHalfWidth * boxHalfWidth + boxHalfDepth * boxHalfDepth);

		if (distanceSquared > radiusMinusHalfDiagonal * radiusMinusHalfDiagonal)
		{
			// �ڽ��� ���� ���η� �о� �ֱ�
			float distance = sqrt(distanceSquared);
			float overlap = distance - radiusMinusHalfDiagonal;

			// �Ÿ� ���͸� ����ȭ
			float invDistance = 1.0f / distance;
			float normX = dx * invDistance;
			float normZ = dz * invDistance;

			// �ڽ� ��ġ�� ���� ���η� �о� �ֱ�
			boxPos.x -= normX * overlap;
			boxPos.z -= normZ * overlap;
		}
	}

	void BattleArea::DeactivateArea()
	{
		//���� �ٽ� ������ �������� �� ����.
		_onTriggerStay = false;

		//�ݶ��̴� ����.
		_collider->SetActive(false);

		//���� ������.
		_object->_transform._position.y = -10000.f;
	}

	void BattleArea::SetActivate(bool val)
	{
		_isActivated = val;
	}

}

