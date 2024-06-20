#include "PgTween.h"
#include "../ParagonGameEngine/TweenSystem.h"
#include <singleton-cpp/singleton.h>

namespace Pg::API::Tween
{
	void PgTween::Initialize()
	{
		auto& tTweenSystem = singleton<Pg::Engine::TweenSystem>();
		_tweenSystem = &tTweenSystem;
	}

	Pg::Util::Tween* PgTween::CreateTween()
	{
		return _tweenSystem->CreateTween();
	}

	void PgTween::KillAllTweens()
	{
		_tweenSystem->KillAllTweens();
	}

}