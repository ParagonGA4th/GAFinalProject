#pragma once

///APIMain을 관리하기 위한 인터페이스.

namespace Pg::Core
{
	class IAPI abstract
	{
	public:
		virtual void Initialize() abstract;
		virtual void Finalize() abstract;
	};
}
