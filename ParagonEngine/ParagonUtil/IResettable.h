#pragma once

//계속 New-Delete를 막기 위해, 스스로를 리셋해서 호출시킬 수 있는 시스템.

namespace Pg::Util
{
	class IResettable
	{
	public:
		virtual void ResetSelf() abstract;
	};
}
