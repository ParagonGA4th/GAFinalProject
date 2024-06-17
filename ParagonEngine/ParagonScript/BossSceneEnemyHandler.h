#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class BossSceneEnemyHandler : public ScriptInterface<BossSceneEnemyHandler>, public IEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(BossSceneEnemyHandler);

	public:
		BossSceneEnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler.
		virtual void ResetToInitialState() override;
	};
}


