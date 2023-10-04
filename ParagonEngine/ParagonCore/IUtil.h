#pragma once

/// <summary>
/// ParagonUtil (UtilMain)을 제어하기 위한 인터페이스.
/// </summary>

namespace Pg::Core
{
	class IUtil abstract
	{
	public:
		virtual void Initialize() abstract;
		virtual void Finalize() abstract;
	};
}
