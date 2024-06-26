#pragma once
/// <summary>
/// TrapArea / BattleArea 같은 공간 안 한정을 위해,
/// + 관리를 위해 만들어진 인터페이스.
/// </summary>

namespace Pg::DataScript
{
	class IConfinedArea
	{
	public:
		virtual void ResetAll() abstract;
		virtual unsigned int GetDesignatedAreaIndex() abstract;
		//이는 Base로 나눴어도 되었겠지만, 일단은. 활성화할지, 말지를 결정.
		virtual void SetActivate(bool val) abstract;


	};
}
