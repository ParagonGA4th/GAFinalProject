#include "PlayerBehavior.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgRayCast.h"

#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	PlayerBehavior::PlayerBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		//Input 객체 받아오기.
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();
	}

	void PlayerBehavior::Awake()
	{
		
		
	}

	void PlayerBehavior::Start()
	{
		//다른 스크립트의 Awake에서 새롭게 인게임 메인카메라를 설정해야 한다.
		_mainCam = _object->GetScene()->GetMainCamera();
		_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		_selfCol->FreezeAxisX(true);
		_selfCol->FreezeAxisZ(true);
		_selfCol->SetMass(2.0f);
	}

	void PlayerBehavior::Update()
	{
		//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
		Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(_object->_transform.GetForward(), {0,0,1});
		tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

		//로직과 상관없는 거
		Pg::Math::PGFLOAT3 outHitPoint;
		float tFloat = 2.0f;
		_pgRayCast->MakeRay({ _object->_transform._position.x + tShouldShootDir.x * tFloat,
						_object->_transform._position.y + tShouldShootDir.y * tFloat,
						_object->_transform._position.z + tShouldShootDir.z * tFloat },
			tShouldShootDir, 10.0f, outHitPoint, nullptr);

		UpdateWASD();
		UpdateFacingDirection(0); //Plane Y-Level 입력해야.
	}

	void PlayerBehavior::UpdateWASD()
	{
		//Camera -> GameObject를 바라보는 방향이 Forward여야 한다!
		Pg::Math::PGFLOAT3 relativeForward = this->_object->_transform._position - _mainCam->_object->_transform._position;
		//Y Vector 캔슬 + 정규화.
		relativeForward.y = 0.0f;
		relativeForward = Pg::Math::PGFloat3Normalize(relativeForward);

		//Y축이 항상 Global Y-Up을 가리키고 있을 테니, Cross하면 Left Vector.
		Pg::Math::PGFLOAT3 relativeLeft = Pg::Math::PGFloat3Cross(relativeForward, Pg::Math::PGFLOAT3::GlobalUp());

		relativeForward = { relativeForward.x * moveSpeed, relativeForward.y * moveSpeed, relativeForward.z * moveSpeed };
		relativeLeft = { relativeLeft.x * moveSpeed, relativeLeft.y * moveSpeed, relativeLeft.z * moveSpeed };

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyUp))
		{
			_selfCol->AddForce(relativeForward, Pg::Data::ForceMode::eFORCE);
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyDown))
		{
			_selfCol->AddForce(-relativeForward, Pg::Data::ForceMode::eFORCE);
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyLeft))
		{
			_selfCol->AddForce(relativeLeft, Pg::Data::ForceMode::eFORCE);
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::KeyRight))
		{
			_selfCol->AddForce(-relativeLeft, Pg::Data::ForceMode::eFORCE);
		}

		if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::KeyUp) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::KeyDown) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::KeyLeft) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::KeyRight))
		{
			//멈췄다가 다시.
			_isJustSetRestraint = true;
			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisY(true);
			_selfCol->FreezeAxisZ(true);
		}

		//PhysX 업데이트를 1차례 거친 후, 다시 리셋.
		if (_isJustSetRestraint)
		{
			//다시 호출 안되게.
			_isJustSetRestraint = false;
		
			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisZ(true);
		}
	}

	void PlayerBehavior::UpdateFacingDirection(float yLevelPlane)
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
		{
			_targetPos = _mainCam->ScreenPointToWorldPlanePoint({ _pgInput->GetMouseX(), _pgInput->GetMouseY() },
				Pg::Math::PGFLOAT3::GlobalUp(), yLevelPlane);
			//Ex.
			//0,1,0 노말에 비해 -1만큼 Distance가 있어야 y=1에 위치할 수 있다.

			//뺄 때 y축 차이를 없애기 위해서.
			_targetPos.y = _object->_transform._position.y;
			//Pg::Math::PGFLOAT3 lookPos = _targetPos - _object->_transform._position;
			Pg::Math::PGFLOAT3 lookPos = _object->_transform._position - _targetPos;
			_targetRotation = PGLookRotation(lookPos, Pg::Math::PGFLOAT3::GlobalUp());

			//업데이트할 값 정하고 Update 루프에서 처리하도록.
			_rotBeginRatio = 0.0f;
			_shouldRotate = true;
		}

		if (_shouldRotate)
		{
			//Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
			//유니티와의 차이.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
			//_selfCol->MoveRotation(currentTargetRotation);
			_object->_transform._rotation = currentTargetRotation;

			_rotBeginRatio += _pgTime->GetDeltaTime() * rotateMultiplier;
			if (_rotBeginRatio > 1.0f)
			{
				_shouldRotate = false;
				_rotBeginRatio = 0.0f;
			}
		}

	}

}