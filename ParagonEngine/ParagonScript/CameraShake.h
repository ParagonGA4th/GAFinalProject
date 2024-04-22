#pragma once
#include "ScriptInterface.h"
#include <memory>
#include <FastNoiseLite/FastNoiseLite.h>
#include <random>

namespace Pg::Data
{
	class Camera;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}

	namespace Time
	{
		class PgTime;
	}
}

//실제로 Perlin Noise를 클라이언트에서 (Shaking) 활용한다.

namespace Pg::DataScript
{
	class CameraShake : public ScriptInterface<CameraShake>
	{
		DEFINE_PARAGON_SCRIPT(CameraShake);
	public:
		CameraShake(Pg::Data::GameObject* obj);
		~CameraShake();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;


		//실제로 카메라를 흔드는 거 Trigger.
		void CauseShake(float amount);


	private:
		void IfCausedShakeCamera();

	private:
		const float _traumaExponent{ 1.0f };	// 더 올라갈 수록 Smoother Falloff.
		const float _recoverySpeed{ 5.0f };		// Trauma로부터 회복하는 정도/s
		const float _frequency{ 25.0f };			// 더 높아질 수록 많이 흔들림.
		const Pg::Math::PGFLOAT3 _maximumTranslationShake{ 1,1,1 };		//흔들릴 때 이동하는 Translation 최대값.	//흔들릴 때 도는 Degree 기준 Rotation 최대값.


	private:
		//Perlin Noise 샘플링 시드
		float _noiseSeed;
		//현재 얼마나 많은 "스트레스"를 트랜스폼이 견디고 있는가 (Shake하면서 내부 줄어듬)
		float _trauma{ 0.0f };

		FastNoiseLite _noiseSrc;

		//0과 100사이 랜덤 숫자 반환.
		std::uniform_int_distribution<int> _distribution{ 0, 100 };
		std::mt19937 _generator{ (std::random_device())() };

	private:
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::Data::Camera* _mainCam{ nullptr };
	};
}
