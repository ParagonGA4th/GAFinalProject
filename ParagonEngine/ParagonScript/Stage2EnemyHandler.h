#pragma once

#include "ScriptInterface.h"
#include "BaseEnemyHandler.h"
#include "IEnemyBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"

#include <unordered_map>

namespace Pg::DataScript
{
	class Stage2EnemyHandler : public ScriptInterface<Stage2EnemyHandler>, public BaseEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2EnemyHandler);

	public:
		Stage2EnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

	};
}


