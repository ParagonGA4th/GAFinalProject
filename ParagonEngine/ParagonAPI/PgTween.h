#pragma once

namespace Pg::Util
{
	class TweenSystem;
	class Tween;
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
		


	private:
		Pg::Util::TweenSystem* _tweenSystem;
	};
}

