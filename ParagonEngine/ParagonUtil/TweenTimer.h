#pragma once
#include <functional>

namespace Pg::Util
{
	class Tween;
}
namespace Pg::Util
{
	class TweenTimer
	{
		friend class Tween;

	public:
		TweenTimer();
		~TweenTimer();

	public:
		void Start();
		void Update();

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


