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

	namespace Tween
	{
		class PgTween;
	}

	namespace Graphics
	{
		class PgGraphics;
	}

	namespace Recast
	{
		class PgRecast;
	}
	
	class PgScene;
}
namespace Pg::API
{
	class APIMain : public Pg::Core::IAPI
	{
	public:
		virtual void Initialize() override;
		virtual void UpdateLoop() override;
		virtual void Finalize() override;

	private:
		Pg::API::Input::PgInput* _pgInput = nullptr;
		Pg::API::Time::PgTime* _pgTime = nullptr;
		Pg::API::Sound::PgAudio* _pgAudio = nullptr;
		Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
		Pg::API::Tween::PgTween* _pgTween = nullptr;
		Pg::API::Graphics::PgGraphics* _pgGraphics = nullptr;
		Pg::API::PgScene* _pgScene = nullptr;
		Pg::API::Recast::PgRecast* _pgRecast = nullptr;
		//PhysXTest* _test;

	};
}

