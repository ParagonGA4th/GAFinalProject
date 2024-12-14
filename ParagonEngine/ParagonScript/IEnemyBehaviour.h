#pragma once
#include "BaseMonster.h"
//#include "IScriptResettable.h"

/// <summary>
/// ��� ������ BehaviourScript�� ��ӹ��� �������̽�.
/// ���� GetBaseMonsterInfo�� Player���� �Ѱ��ֱ� �����̴�.
/// 2024.06.11
/// </summary>

namespace Pg::DataScript
{
	class IEnemyBehaviour
	{
		//�÷��̾ �Ϲݰ������� ���͸� Ÿ�� �� �����Ǵ� ����
		//inline static const float INCREMENT_MANA_AMT = 5.f;
	public:
		virtual BaseMonsterInfo* ReturnBaseMonsterInfo() abstract;
		virtual unsigned int GetBelongAreaIndex() abstract; //�ڽ��� ���� �ִ� Area Index�� ��ȯ�Ѵ�.
		virtual void ResetAll() abstract;
	};
}

