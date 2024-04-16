#pragma once
#include "ScriptInterface.h"

namespace Pg::DataScript
{
	class CombatSystem : public ScriptInterface<CombatSystem>
	{
		DEFINE_PARAGON_SCRIPT(CombatSystem);

	public:
		CombatSystem(Pg::Data::GameObject* obj);

		virtual void Start() override;
		virtual void Update() override;
	};
}



