#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <vector>
#include <unordered_map>

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

	private:
		// Monster TAG / Instance. 
		// Tag별로 나누어서 관리한다.
		std::unordered_map<std::string, std::vector<EnemyAggregate>> _managedMonstersList;
	};
}


