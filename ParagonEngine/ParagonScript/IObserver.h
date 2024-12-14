#pragma once
//#include "IEvent.h"
#include "ScriptEventHelper.h"

/// <summary>
/// ������ ��� ���������� �ش� �������̽��� ��ӹ޾ƾ� �Ѵ�.
/// �÷��̾� ���� �������� ��ҵ��� �и��ؼ� ó���Ѵ�.
/// </summary>

namespace Pg::DataScript
{
	class IObserver
	{
	public:
		//�ش� �Լ��� ������ ���� Ŭ�������� �����ؾ� �Ѵ�. ������ ������ �� �ִ� �� ���� Variant�� ����� ä.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) abstract;
	};
}
