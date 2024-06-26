#pragma once

#include "ScriptInterface.h"
#include "BaseEnemyHandler.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <vector>
#include <unordered_map>

namespace Pg::DataScript
{
	class BossStageEnemyHandler : public ScriptInterface<BossStageEnemyHandler>, public BaseEnemyHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageEnemyHandler);

	public:
		BossStageEnemyHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;
	};
}


