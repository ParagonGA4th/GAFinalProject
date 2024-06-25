#include "TrapArea.h"

//#include "PlayerMovementSector.h"
#include "PlayerHandler.h"

#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/CheckInBox.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	TrapArea::TrapArea(Pg::Data::GameObject* obj)
		: ScriptInterface(obj)
	{
	}

	void TrapArea::Awake()
	{
		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		assert(_collider != nullptr);

		auto& tdelta = singleton<Pg::API::Time::PgTime>();
		_deltaTime = &tdelta;
	}

	void TrapArea::Start()
	{
	}

	void TrapArea::Update()
	{
		//Exit 할 때 nullptr 할당 해줘야. CombatSystem을 통한 연결 작업 중, 일단은 보여줘야 하니 이렇게 ㄱㄱ!
		if (_playerBattleBehavior != nullptr)
		{
			auto dcol = _playerBattleBehavior->_object->GetComponent<Pg::Data::DynamicCollider>();

			if (_onTriggerStay)
			{
				// 플레이어의 체력이 계속 깎여야 함
				_playerBattleBehavior->healthPoint -= _deltaTime->GetDeltaTime() * _damage;
				auto mesh = _playerBattleBehavior->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				mesh->SetActive(!mesh->GetActive());
			}
		}
	}

	void TrapArea::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];
			if (col->_object->GetTag() == "TAG_Player")
			{
				_onTriggerStay = true;

				// 플레이어의 움직임이 느려져야 함
				_playerBattleBehavior = col->_object->GetComponent<Pg::DataScript::PlayerHandler>();
				//_playerMovement = col->_object->GetComponent<Pg::DataScript::PlayerMovementSector>();
				assert(_playerBattleBehavior != nullptr);
				//assert(_playerBattleBehavior != nullptr && _playerMovement != nullptr);

				_previousMoveSpeed = _playerBattleBehavior->GetPlayerMoveSpeed();
				_playerBattleBehavior->SetPlayerMoveSpeed(_previousMoveSpeed / 2.0f);
			}
		}
	}

	void TrapArea::OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];
			if (col->_object->GetTag() == "TAG_Player")
			{
				_onTriggerStay = false;

				// 플레이어의 속도가 돌아와야 함
				_playerBattleBehavior->SetPlayerMoveSpeed(_previousMoveSpeed);

				auto mesh = _playerBattleBehavior->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				mesh->SetActive(true);
			}
		}
	}

	unsigned int TrapArea::GetDesignatedAreaIndex()
	{
		return _areaIndex;
	}

	void TrapArea::ResetAll()
	{
		_isActivated = true;
	}

}
