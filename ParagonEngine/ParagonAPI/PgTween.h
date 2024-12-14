#pragma once
#include "../ParagonUtil/Tween.h"

namespace Pg::Engine
{
	class TweenSystem;
}

/// <summary>
/// Util->TweenSystem�� �̿��� PgTween
/// </summary>

namespace Pg::API::Tween
{
	class PgTween
	{
	public:
		//Initialize.
		void Initialize();

		//���� API���� ���.
		Pg::Util::Tween* CreateTween();

		void KillAllTweens();
		
	private:
		Pg::Engine::TweenSystem* _tweenSystem;
	};
}

