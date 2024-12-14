#pragma once

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

/// <summary>
/// TimeSystem�� �̿��� PgTime
/// </summary>
namespace Pg::API::Time
{
	class PgTime
	{
	public:

		void Initialize();
		void SetPauseGame(bool val);

		float GetDeltaTime();
		float GetFrameRate();

	private:
		Pg::Util::Time::TimeSystem* _timeSystem;
	};
}

