#include "BattleArea.h"
#include "PlayerHandler.h"

#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/SphereCollider.h"

#include "TotalGameManager.h"
#include "BaseEnemyHandler.h"
#include "BaseAreaHandler.h"

namespace Pg::DataScript
{
	BattleArea::BattleArea(Pg::Data::GameObject* obj)
		:ScriptInterface(obj)
	{
	}

	void BattleArea::GrabManagedObjects()
	{
		//자신이 속한 곳의 AreaHandler / EnemyHandler를 받아오기.
		//적 보고 로직 등에 사용될 것.
		TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
		HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
		this->_areaHandler = tHB->_areaHandler;
		assert(_areaHandler != nullptr);
		this->_enemyHandler = tHB->_enemyHandler;
		assert(_enemyHandler != nullptr);
	}

	void BattleArea::Awake()
	{
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
		if (!_onTriggerStay)
		{
			//if (_player != nullptr && _monster != 0)
			//{
			//	auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();

			//	auto& spherePos = _collider->_object->_transform._position;
			//	auto& boxPos = _player->_object->_transform._position;

			//	float sphereRadius = _collider->GetRadius();
			//	float boxHalfWidth = dcol->GetWidth() / 2;
			//	float boxHalfDepth = dcol->GetDepth() / 2;

			//	// 구의 중심에서 박스의 중심까지의 벡터
			//	float dx = boxPos.x - spherePos.x;
			//	float dz = boxPos.z - spherePos.z;
			//	float distanceSquared = dx * dx + dz * dz;

			//	// 박스의 각 모서리가 구의 경계를 넘지 않도록 조정
			//	float radiusMinusHalfDiagonal = sphereRadius - sqrt(boxHalfWidth * boxHalfWidth + boxHalfDepth * boxHalfDepth);

			//	if (distanceSquared > radiusMinusHalfDiagonal * radiusMinusHalfDiagonal)
			//	{
			//		// 박스를 구의 내부로 밀어 넣기
			//		float distance = sqrt(distanceSquared);
			//		float overlap = distance - radiusMinusHalfDiagonal;

			//		// 거리 벡터를 정규화
			//		float invDistance = 1.0f / distance;
			//		float normX = dx * invDistance;
			//		float normZ = dz * invDistance;

			//		// 박스 위치를 구의 내부로 밀어 넣기
			//		boxPos.x -= normX * overlap;
			//		boxPos.z -= normZ * overlap;
			//	}
			//} 

			//else if (_monster == 0)
			//{
			//	_object->SetActive(false);
			//	_collider->SetActive(false);

			//	_object->_transform._position.y = -10000.f;
			//}
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

				//밑은 적 등록 / 모두 죽일 시 : 판단 로직을 마련 위함.
				//이때는, Enemy가 죽었을 때 Handler에게 죽었다고 알려주는 로직이 필요.
			}
		}
	}

	void BattleArea::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];
			if (col->_object->GetName() == "PlayerSensor")
			{
				_onTriggerStay = false;
			}
		}
	}

	unsigned int BattleArea::GetDesignatedAreaIndex()
	{
		return _areaIndex;
	}

	void BattleArea::ResetAll()
	{
		_isActivated = true;
	}



}

