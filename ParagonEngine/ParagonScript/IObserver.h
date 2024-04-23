#pragma once
//#include "IEvent.h"
#include "ScriptEventHelper.h"

/// <summary>
/// 무조건 모든 옵저버들은 해당 인터페이스를 상속받아야 한다.
/// 플레이어 같은 개별적인 요소들은 분리해서 처리한다.
/// </summary>

namespace Pg::DataScript
{
	class IObserver
	{
	public:
		//해당 함수를 무조건 하위 클래스에서 구현해야 한다. 정보를 전달할 수 있는 두 개의 Variant를 열어둔 채.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) abstract;
	};
}
