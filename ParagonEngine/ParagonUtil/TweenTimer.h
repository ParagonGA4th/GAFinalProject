#pragma once
#include <functional>
#include "IResettable.h"

namespace Pg::Util
{
	class Tween;
	class TweenSystem;
}

namespace Pg::Util
{
	class TweenTimer : public IResettable
	{
		friend class Tween;
		friend class TweenSystem;

	public:
		TweenTimer();
		~TweenTimer();

	public:
		void Start();
		void Update(); 

		virtual void ResetSelf() override;

	private:
		// object has each thier timer
		float _time;
		float _delayTime;
		float _duration;
		std::function<void()> _play;

		// delayed Active
		bool _isActive;
	};
}


