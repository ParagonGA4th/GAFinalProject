#pragma once
#include "../ParagonUtil/Tween.h"

namespace Pg::Engine
{
	class TweenSystem;
}

/// <summary>
/// Util->TweenSystem을 이용한 PgTween
/// </summary>

namespace Pg::API::Tween
{
	class PgTween
	{
	public:
		//Initialize.
		void Initialize();

		//실제 API에서 사용.
		Pg::Util::Tween* CreateTween();

		void KillAllTweens();
		
	private:
		Pg::Engine::TweenSystem* _tweenSystem;
	};
}

