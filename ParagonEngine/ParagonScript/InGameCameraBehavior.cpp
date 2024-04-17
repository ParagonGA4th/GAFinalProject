#include "InGameCameraBehavior.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/Camera.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgInput.h"

#include <cassert>
#include <FastNoiseLite/FastNoiseLite.h>

namespace Pg::DataScript
{
	InGameCameraBehavior::InGameCameraBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void InGameCameraBehavior::Awake()
	{
		using namespace Pg::Math;
		
		//자신이 속한 Camera Component를 찾아 보관한다.
		_selfCamera = _object->GetComponent<Pg::Data::Camera>();
		assert(_selfCamera != nullptr);

		//Tag를 기준으로 찾는다.
		auto plVec = _object->GetScene()->FindObjectsWithTag("TAG_Player");
		//플레이어는 한명만 있어야 한다.
		assert(plVec.size() == 1 && "플레이어 객체는 단 하나만 발견되어야!");
		//플레이어 Transform 보관.
		_playerTransform = plVec.at(0)->GetComponent<Pg::Data::Transform>();

		//현재 자기 자신의 Rotation 세팅. (밑으로 약 40도 바라봐야)
		_object->_transform._rotation = PGEulerToQuaternion({ PGConvertToRadians(-40), 0, 0 });

		//자기 자신이 속한 Camera를 MainCamera로 설정.
		_object->GetScene()->SetMainCamera(_selfCamera);
	}

	void InGameCameraBehavior::Start()
	{
		
	}

	void InGameCameraBehavior::FixedUpdate()
	{
		LerpFollowPlayer();
		IfValidShakeCamera();
		
	}

	void InGameCameraBehavior::LerpFollowPlayer()
	{
		//Target Position 설정.
		_targetCamPosition.x = _playerTransform->_position.x + camOffset.x;
		_targetCamPosition.y = _playerTransform->_position.y + camOffset.y;
		_targetCamPosition.z = _playerTransform->_position.z + camOffset.z;

		//Option1. 
		//transform.position = Vector3.SmoothDamp(transform.position, targetCamPosition, ref velocity, smoothTime);

		float interpolation = _speed * _pgTime->GetDeltaTime();

		Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
		tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _targetCamPosition, interpolation);
		_object->_transform._position = tPosition;
	}

	void InGameCameraBehavior::IfValidShakeCamera()
	{
		//일단은 임시로 KeyB가 Shake 값이 될 것.
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyB))
		{
			_shouldShake = true;
			_currentShakeTimePassed = 0.0f;
		}

		//Shake할 시간을 정하기.
		if (_shouldShake)
		{
			_currentShakeTimePassed += _pgTime->GetDeltaTime();
			if (_currentShakeTimePassed > _shouldShakeSeconds)
			{
				_shouldShake = false;
				_currentShakeTimePassed = 0.0f;
			}
		}

		if (_shouldShake)
		{
			//실제 Shake하는 부분이 여기서 들어간다.




		}


	}

	

}