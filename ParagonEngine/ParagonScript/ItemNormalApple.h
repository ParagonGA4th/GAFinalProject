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
		//이는 Base로 나눴어도 되었겠지만, 일단은. 활성화할지, 말지를 결정.
		virtual void SetActivate(bool val) override;


	};
}


