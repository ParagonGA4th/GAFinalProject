#pragma once

#include "../ParagonMath/PgMath.h"
#include "UtilDLLExporter.h"
#include "EasingMode.h"
#include "IResettable.h"
#include <memory>
#include <functional>

/// <summary>
/// Tween에 요구되는 개별 객체. 
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
		PARAGON_UTIL_DLL Tween(); //TweenSystem만 Tween의 객체를 만들 수 있다.

	public:
		virtual void ResetSelf() override;

		PARAGON_UTIL_DLL ~Tween();
		//데이터 가져오기.
		PARAGON_UTIL_DLL Tween& GetData(int* data);
		PARAGON_UTIL_DLL Tween& GetData(float* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGFLOAT2* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGFLOAT3* data);
		PARAGON_UTIL_DLL Tween& GetData(Pg::Math::PGQuaternion* data);

		//기본 자료형 Move.  (Tween)
		PARAGON_UTIL_DLL Tween& DoMove(const int& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const float& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(Pg::Math::PGFLOAT2& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const Pg::Math::PGFLOAT3& destination, const float duration);
		PARAGON_UTIL_DLL Tween& DoMove(const Pg::Math::PGQuaternion& destination, const float duration);

		// Addition Functions
		PARAGON_UTIL_DLL Tween& Delay(float delayTime);					//이건 되는지 모름.
		PARAGON_UTIL_DLL Tween& OnComplete(std::function<void()> func);	//이건 Tween이 끝났을시 등록하는데 쓰임.

		// Easing 세팅 함수
		PARAGON_UTIL_DLL Tween& SetEase(Enums::eEasingMode type);

		//이는 세팅할때 호출되기보다는, 자기 자신을 Kill하는 함수. ( == 실행 중지)
		PARAGON_UTIL_DLL void Kill();
		//최종 Duration에 도달하기 전에, 특정 시간이 이상 지났으면 바로 Kill해주기. Tween의 움직임은 살리되 일찍 끝내고 싶을 때 활용.
		//무조건 DoMove 이후에 호출되어야.
		PARAGON_UTIL_DLL Tween& KillEarly(float ratio);

		// Easing 수학 함수
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
		// 제곱
		static float InQuad(float x);
		static float OutQuad(float x);
		static float InOutQuad(float x);
		// 세제곱
		static float InCubic(float x);
		static float OutCubic(float x);
		static float InOutCubic(float x);
		// 네제곱
		static float InQuart(float x);
		static float OutQuart(float x);
		static float InOutQuart(float x);
		// 다섯제곱
		static float InQuint(float x);
		static float OutQuint(float x);
		static float InOutQuint(float x);

		//현재 사용되고 있는지만 반환한다.
		PARAGON_UTIL_DLL bool GetIsUsed();
	private:
		bool _isUsedRightNow{ false };

	private:
		//데이터 세이브.
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
