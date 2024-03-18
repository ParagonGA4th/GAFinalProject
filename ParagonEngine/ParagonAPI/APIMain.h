#pragma once
#include "../ParagonProcess/IAPI.h"

/// <summary>
/// API가 발동되기 위한 선 조건을 충족시키기 위한 ParagonAPI 관리 클래스
/// </summary>

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}

	namespace Time
	{
		class PgTime;
	}

	namespace Raycast
	{
		class PgRayCast;
	}

	namespace Sound
	{
		class PgAudio;
	}
}
namespace Pg::API
{
	class APIMain : public Pg::Core::IAPI
	{
	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

	private:
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::API::Time::PgTime* _pgTime = nullptr;
		//Pg::API::Sound::PgSound* _pgSound = nullptr;
		Pg::API::Sound::PgAudio* _pgAudio = nullptr;
		Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
		//PhysXTest* _test;

	};
}

