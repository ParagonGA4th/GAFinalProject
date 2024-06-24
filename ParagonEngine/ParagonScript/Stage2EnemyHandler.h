#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "IEnemyBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Scene.h"

#include <unordered_map>

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
		virtual void TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func) override;
	private:
		// Monster TAG / Instance. 
		// Tag별로 나누어서 관리한다.
		std::unordered_map<std::string, std::vector<EnemyAggregate>> _managedMonstersList;
	};
}


