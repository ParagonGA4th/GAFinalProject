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

//������ Perlin Noise�� Ŭ���̾�Ʈ���� (Shaking) Ȱ���Ѵ�.

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


		//������ ī�޶� ���� �� Trigger.
		void CauseShake(float amount);


	private:
		void IfCausedShakeCamera();

	private:
		const float _traumaExponent{ 1.0f };	// �� �ö� ���� Smoother Falloff.
		const float _recoverySpeed{ 5.0f };		// Trauma�κ��� ȸ���ϴ� ����/s
		const float _frequency{ 25.0f };			// �� ������ ���� ���� ��鸲.
		const Pg::Math::PGFLOAT3 _maximumTranslationShake{ 1,1,1 };		//��鸱 �� �̵��ϴ� Translation �ִ밪.	//��鸱 �� ���� Degree ���� Rotation �ִ밪.


	private:
		//Perlin Noise ���ø� �õ�
		float _noiseSeed;
		//���� �󸶳� ���� "��Ʈ����"�� Ʈ�������� �ߵ�� �ִ°� (Shake�ϸ鼭 ���� �پ��)
		float _trauma{ 0.0f };

		FastNoiseLite _noiseSrc;

		//0�� 100���� ���� ���� ��ȯ.
		std::uniform_int_distribution<int> _distribution{ 0, 100 };
		std::mt19937 _generator{ (std::random_device())() };

	private:
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::Data::Camera* _mainCam{ nullptr };
	};
}
