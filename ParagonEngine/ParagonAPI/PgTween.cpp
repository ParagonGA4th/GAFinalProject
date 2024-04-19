#include "PgTween.h"
#include "../ParagonUtil/TweenSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::API::Tween
{
	void PgTween::Initialize()
	{
		auto& tTweenSystem = singleton<Pg::Util::TweenSystem>();
		_tweenSystem = &tTweenSystem;
	}

	Pg::Util::Tween* PgTween::CreateTween()
	{
		return _tweenSystem->CreateTween();
	}

}