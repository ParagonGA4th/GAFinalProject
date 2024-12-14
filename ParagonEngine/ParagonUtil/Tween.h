#pragma once

#include "../ParagonMath/PgMath.h"
#include "UtilDLLExporter.h"
#include "EasingMode.h"
#include "IResettable.h"
#include <memory>
#include <functional>

/// <summary>
/// Tween�� �䱸�Ǵ� ���� ��ü. 
/// Special Thanks To : @chloe980504
/// </summary>

namespace Pg::Util
{
	class TweenTimer;
}

namespace Pg::Engine
{
	class TweenSystem;
}

namespace Pg::Util
{
	class Tween : public IResettable
	{
		friend class Pg::Engine::TweenSystem;

	private:
		PARAGON_UTIL_DLL Tween(); //TweenSystem�� Tween�� ��ü�� ���� �� �ִ�.

	public:
		virtual void ResetSelf() override;

		PARAGON_UTIL_DLL ~Tween();
		//������ ��������.
		PARAGON_UTIL_DLL Tween& GetData(int* data);
		PARAGON_UTIL_DLL Tween& GetData(float* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGFLOAT2* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGFLOAT3* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGQuaternion* data);

		//�⺻ �ڷ��� Move.  (Tween)
		PARAGON_UTIL_DLL Tween& DoMove(const int& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const float& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(Pg::Math::PGFLOAT2& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const Pg::Math::PGFLOAT3& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const Pg::Math::PGQuaternion& destination, const float duration);

		// Addition Functions
		PARAGON_UTIL_DLL Tween& Delay(float delayTime);					//�̰� �Ǵ��� ��.
		PARAGON_UTIL_DLL Tween& OnComplete(std::function<void()> func);	//�̰� Tween�� �������� ����ϴµ� ����.

		// Easing ���� �Լ�
		PARAGON_UTIL_DLL Tween& SetEase(Enums::eEasingMode type);

		//�̴� �����Ҷ� ȣ��Ǳ⺸�ٴ�, �ڱ� �ڽ��� Kill�ϴ� �Լ�. ( == ���� ����)
		PARAGON_UTIL_DLL void Kill();
		//���� Duration�� �����ϱ� ����, Ư�� �ð��� �̻� �������� �ٷ� Kill���ֱ�. Tween�� �������� �츮�� ���� ������ ���� �� Ȱ��.
		//������ DoMove ���Ŀ� ȣ��Ǿ��.
		PARAGON_UTIL_DLL Tween& KillEarly(float ratio);

		// Easing ���� �Լ�
		static float Linear(float x);

		static float InBack(float x);
		static float OutBack(float x);
		static float InOutBack(float x);

		static float InBounce(float x);
		static float OutBounce(float x);
		static float InOutBounce(float x);

		static float InElastic(float x);
		static float OutElastic(float x);
		static float InOutElastic(float x);

		static float InSine(float x);
		static float OutSine(float x);
		static float InOutSine(float x);

		static float InExpo(float x);
		static float OutExpo(float x);
		static float InOutExpo(float x);

		static float InCirc(float x);
		static float OutCirc(float x);
		static float InOutCirc(float x);
		// ����
		static float InQuad(float x);
		static float OutQuad(float x);
		static float InOutQuad(float x);
		// ������
		static float InCubic(float x);
		static float OutCubic(float x);
		static float InOutCubic(float x);
		// ������
		static float InQuart(float x);
		static float OutQuart(float x);
		static float InOutQuart(float x);
		// �ټ�����
		static float InQuint(float x);
		static float OutQuint(float x);
		static float InOutQuint(float x);

		//���� ���ǰ� �ִ����� ��ȯ�Ѵ�.
		PARAGON_UTIL_DLL bool GetIsUsed();
	private:
		bool _isUsedRightNow{ false };

	private:
		//������ ���̺�.
		int* dataI;
		float* dataF;
		Pg::Math::PGFLOAT2* data2V;
		Pg::Math::PGFLOAT3* data3V;
		Pg::Math::PGQuaternion* dataQuat = nullptr;
		std::function<void()> _onCompleteFunc = nullptr;

		/// easing functions
		std::unique_ptr<Pg::Util::TweenTimer> _timer;

		// easing data
		static std::function<float(float)> _easings[31];
		std::function<float(float)> _myEase;
	};
}
