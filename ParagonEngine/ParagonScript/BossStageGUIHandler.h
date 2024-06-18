#pragma once
#include "ScriptInterface.h"
#include "IGUIHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class BossStageGUIHandler : public ScriptInterface<BossStageGUIHandler>, public IGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageGUIHandler);
	public:
		BossStageGUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//IGUI Handler.
		virtual void ResetToInitialState() override;

	};
}

