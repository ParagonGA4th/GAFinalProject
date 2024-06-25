#pragma once

#include "ScriptInterface.h"
#include "IEnemyHandler.h"
#include "TransformSimpleStorage.h"
#include "IEnemyBehaviour.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <vector>
#include <unordered_map>
#include <map>

/// <summary>
/// Ex. Wasp Body - Wings라면,
// Body가 IEnemyBehaviour를 가지고,Wings는 Body를 부모로 둬야 한다.
// Mimic - Box도 Mimic의 자식이 Box가 되어야 하고,
// GolemBody - GolemCrystal도 마찬가지다.
// 항상 주가 되는 오브젝트가 IEnemyBehaviour를 갖고 있어야 한다.
// 해당 오브젝트는, 자식의 주도권을 쥔다.
/// </summary>

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
		virtual void TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func) override;
		
	private:
		void GrabOrganizeAllEnemies();

	private:
		const unsigned int _entireAreaCount{ 3 };

		// Monster TAG / Instance. 
		// Tag별로 나누어서 관리한다.
		// Area도 나누어서 보관.
		std::map<unsigned int, 
			std::unordered_map<std::string, std::vector<EnemyAggregate>>> _managedMonstersList;
	};
}


