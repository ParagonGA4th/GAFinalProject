#pragma once
/// <summary>
/// TrapArea / BattleArea ���� ���� �� ������ ����,
/// + ������ ���� ������� �������̽�.
/// </summary>

namespace Pg::DataScript
{
	class IConfinedArea
	{
	public:
		virtual void ResetAll() abstract;
		virtual unsigned int GetDesignatedAreaIndex() abstract;
		//�̴� Base�� ����� �Ǿ�������, �ϴ���. Ȱ��ȭ����, ������ ����.
		virtual void SetActivate(bool val) abstract;


	};
}
