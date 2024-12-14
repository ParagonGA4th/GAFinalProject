#pragma once
#include "ScriptInterface.h"
#include "IConfinedArea.h"

namespace Pg::DataScript
{
	class ItemNormalApple : public ScriptInterface<ItemNormalApple>, public IConfinedArea
	{
		DEFINE_PARAGON_SCRIPT(ItemNormalApple);

	public:
		ItemNormalApple(Pg::Data::GameObject* obj);

		//Reset All.
		virtual void ResetAll() override;
		virtual unsigned int GetDesignatedAreaIndex() override;
		//�̴� Base�� ����� �Ǿ�������, �ϴ���. Ȱ��ȭ����, ������ ����.
		virtual void SetActivate(bool val) override;


	};
}


