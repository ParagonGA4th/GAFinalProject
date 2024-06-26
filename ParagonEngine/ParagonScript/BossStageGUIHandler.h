#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "GUIDefinesAndStructs.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>

namespace Pg::DataScript
{
	class BossStageGUIHandler : public ScriptInterface<BossStageGUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageGUIHandler);
	public:
		BossStageGUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//필수 구현.
		virtual void AssignPointersToGUI() override;

	};
}

