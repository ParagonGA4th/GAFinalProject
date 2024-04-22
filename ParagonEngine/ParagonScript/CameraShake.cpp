#include "CameraShake.h"

#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"


#include <singleton-cpp/singleton.h>
#include <algorithm>
#include <limits>
#include <cmath>

namespace Pg::DataScript
{
	CameraShake::CameraShake(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();

		
	}

	CameraShake::~CameraShake()
	{

	}

	void CameraShake::Awake()
	{
		//Perlin Noise 만들기.
		_noiseSrc.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

		//0-1까지 Random Float Value 구하기.
		_noiseSeed = static_cast<float>(_distribution(_generator)) / 100.0f;
	}

	void CameraShake::Start()
	{
		_mainCam = _object->GetScene()->GetMainCamera();
	}

	void CameraShake::Update()
	{
		//일단은 셰이킹 테스트 : KeyDown으로 받자!
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyB))
		{
			CauseShake(0.25f);
		}

		IfCausedShakeCamera();
	}

	void CameraShake::CauseShake(float amount)
	{
		_trauma = std::clamp<float>(amount, 0.f, 1.0f);
	}

	void CameraShake::IfCausedShakeCamera()
	{
		float shake = powf(_trauma, _traumaExponent);
		float dt = _pgTime->GetDeltaTime();

		if (_trauma > std::numeric_limits<float>::epsilon())
		{
			_noiseSeed = static_cast<float>(_distribution(_generator)) / 100.0f;
			//PG_TRACE("RANDOMMAKING");
		}

		_mainCam->_object->_transform._position = _mainCam->_object->_transform._position + Pg::Math::PGFLOAT3(
			_maximumTranslationShake.x * (_noiseSrc.GetNoise(_noiseSeed, dt * _frequency) * 2.0f - 1.0f),
			_maximumTranslationShake.y * (_noiseSrc.GetNoise(_noiseSeed + 1.0f, dt * _frequency) * 2.0f - 1.0f),
			_maximumTranslationShake.z * (_noiseSrc.GetNoise(_noiseSeed + 2.0f, dt * _frequency) * 2.0f - 1.0f)
		) * shake;

		_trauma = std::clamp<float>((_trauma - _recoverySpeed * dt), 0.f, 1.f);
	}

}