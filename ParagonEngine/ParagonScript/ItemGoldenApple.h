#pragma once
#include "ScriptInterface.h"
#include "IConfinedArea.h"

namespace Pg::DataScript
{
	class ItemGoldenApple : public ScriptInterface<ItemGoldenApple>, public IConfinedArea
	{
		DEFINE_PARAGON_SCRIPT(ItemGoldenApple);

	public:
		ItemGoldenApple(Pg::Data::GameObject* obj);

		virtual void ResetAll() override;
		virtual unsigned int GetDesignatedAreaIndex() override;
		//�̴� Base�� ����� �Ǿ�������, �ϴ���. Ȱ��ȭ����, ������ ����.
		virtual void SetActivate(bool val) override;

	};
}


