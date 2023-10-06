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

	class PhysXTest;
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
		PhysXTest* _test;

	};
}

