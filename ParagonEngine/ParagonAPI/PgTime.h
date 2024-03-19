#pragma once

namespace Pg::Util
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
		Pg::Util::Time::TimeSystem* _timeSystem;
	};
}

