#pragma once
#include <functional>
#include "IResettable.h"
#include "UtilDLLExporter.h"

namespace Pg::Util
{
	class Tween;
}

namespace Pg::Engine
{
	class TweenSystem;
}

namespace Pg::Util
{
	class TweenTimer : public IResettable
	{
		friend class Tween;
		friend class Pg::Engine::TweenSystem;

	public:
		PARAGON_UTIL_DLL TweenTimer();
		PARAGON_UTIL_DLL ~TweenTimer();

	public:
		void Start();
		PARAGON_UTIL_DLL void Update();

		virtual void ResetSelf() override;

	private:
		// object has each thier timer
		float _time;
		float _delayTime;
		float _duration;
		std::function<void()> _play;

		// delayed Active
		bool _isActive;

		//DeltaTimePointer
		const float* _deltaTimePointer{ nullptr };

		//일찍 동작을 끝내는 KillEarly가 사용되는가?
		bool _isUseKillEarly{ false };
		float _floatKillEarlyDuration{ 0.f };
	};
}