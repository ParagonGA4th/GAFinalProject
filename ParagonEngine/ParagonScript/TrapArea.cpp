#include "TrapArea.h"

#include "PlayerMovement.h"
#include "PlayerBattleBehavior.h"

#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/CheckInBox.h"

#include <singleton-cpp/singleton.h>

Pg::DataScript::TrapArea::TrapArea(Pg::Data::GameObject* obj)
	: ScriptInterface(obj)
{
}

void Pg::DataScript::TrapArea::Awake()
{
	_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
	assert(_collider != nullptr);

	auto& tdelta = singleton<Pg::API::Time::PgTime>();
	_deltaTime = &tdelta;
}

void Pg::DataScript::TrapArea::Start()
{
}

void Pg::DataScript::TrapArea::Update()
{

	//Exit 할 때 nullptr 할당 해줘야. CombatSystem을 통한 연결 작업 중, 일단은 보여줘야 하니 이렇게 ㄱㄱ!
	if (_playerBattleBehavior != nullptr)
	{

		auto dcol = _playerBattleBehavior->_object->GetComponent<Pg::Data::DynamicCollider>();


		// 플레이어의 체력이 계속 깎여야 함
		if (Pg::Util::CheckInBox::IsIn3DBox(
		
			_collider->_object->_transform._position, _collider->_width, _collider->_height, _collider->_depth,
			_player->_object->_transform._position, dcol->GetWidth(), dcol->GetHeight(), dcol->GetDepth()))
		{
			// 플레이어가 계속 빠져야 함
			dcol->SetLinearVelocity(dcol->GetLinearVelocity() * _deltaTime->GetDeltaTime() * _fallSpeed);

			// 플레이어의 체력이 계속 깎여야 함
			_playerBattleBehavior->healthPoint -= _deltaTime->GetDeltaTime() * _damage;
			PG_TRACE("Stay");
		}
		else
		{
			// 플레이어의 속도가 돌아와야 한다
			_player->moveSpeed = _previousMoveSpeed;

			// 플레이어가 계속 빠지면 안된다
			auto dcol = _player->_object->GetComponent<Pg::Data::DynamicCollider>();
			dcol->SetLinearVelocity({ 0.0f, 0.0f, 0.0f });
		}
	}
}

void Pg::DataScript::TrapArea::OnTriggerEnter(Pg::Data::Collider* col)
{
	if (col->_object->GetTag() == "TAG_Player")
	{
		// 플레이어의 움직임이 느려져야 함
		_playerBattleBehavior = col->_object->GetComponent<Pg::DataScript::PlayerBattleBehavior>();
		_playerMovement = col->_object->GetComponent<Pg::DataScript::PlayerMovement>();
		assert(_playerBattleBehavior != nullptr && _playerMovement != nullptr);

		_previousMoveSpeed = _playerMovement->moveSpeed;
		_playerMovement->moveSpeed = _previousMoveSpeed / 2;

		PG_TRACE("Trigger");
		_isInit = true;
	}
}