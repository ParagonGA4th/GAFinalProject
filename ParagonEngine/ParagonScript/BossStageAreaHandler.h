#pragma once

#include "ScriptInterface.h"
#include "IAreaHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class BossStageAreaHandler : public ScriptInterface<BossStageAreaHandler>, public IAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageAreaHandler);

	public:
		BossStageAreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler
		virtual void ResetToInitialState() override;
	};
}


