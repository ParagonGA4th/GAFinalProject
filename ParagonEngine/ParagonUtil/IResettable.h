#pragma once

//��� New-Delete�� ���� ����, �����θ� �����ؼ� ȣ���ų �� �ִ� �ý���.

namespace Pg::Util
{
	class IResettable
	{
	public:
		virtual void ResetSelf() abstract;
	};
}
