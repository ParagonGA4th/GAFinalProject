#pragma once

namespace Pg::Engine
{
	namespace Time
	{
		class TimeSystem;
	}
}

/// <summary>
/// TimeSystem을 이용한 PgTime
/// </summary>
namespace Pg::API::Time
{
	class PgTime
	{
	public:

		void Initialize();

		float GetDeltaTime();
		float GetFrameRate();

	private:
		Pg::Engine::Time::TimeSystem* _timeSystem;
	};
}

