#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class Stage1EnemyHandler : public ScriptInterface<Stage1EnemyHandler>, public IEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1EnemyHandler);

	public:
		Stage1EnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler.
		virtual void ResetToInitialState() override;
	};
}


