#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class BossStageEnemyHandler : public ScriptInterface<BossStageEnemyHandler>, public IEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageEnemyHandler);

	public:
		BossStageEnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler.
		virtual void ResetToInitialState() override;
	};
}


