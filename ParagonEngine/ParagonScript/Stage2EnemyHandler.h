#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class Stage2EnemyHandler : public ScriptInterface<Stage2EnemyHandler>, public IEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2EnemyHandler);

	public:
		Stage2EnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler.
		virtual void ResetToInitialState() override;
	};
}


