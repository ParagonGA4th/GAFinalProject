#pragma once
#include "../ParagonCore/IUtil.h"

/// <summary>
/// ParagonUtil의 체계를 관리하는 UtilMain.
/// </summary>
namespace Pg::Util
{
	class UtilMain : public Pg::Core::IUtil
	{
	public:
		virtual void Initialize() override;
		virtual void Finalize() override;
	};
}

