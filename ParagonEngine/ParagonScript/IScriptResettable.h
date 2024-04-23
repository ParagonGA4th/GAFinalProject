#pragma once

/// <summary>
/// 리셋해서 재사용 가능하게 해야 하는 스크립트 내부의 클래스들일 경우,
/// 이를 일괄적으로 실행할 수 있게 해주는 Reset용 인터페이스.
/// </summary>

namespace Pg::DataScript
{
	class IScriptResettable
	{
	public:
		virtual void ResetAll() abstract;
	};
}
