#pragma once
#include "EnemyDefinesAndStructs.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>

//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
//Enemy Handler들의 인터페이스.

namespace Pg::DataScript
{
	class HandlerBundle3D;

	class BaseEnemyHandler
	{
	public:
		BaseEnemyHandler(unsigned int entireAreaCount);

		//더 이상 Virtual이 아니다.
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		void ResetToInitialState();

		//만약 안 죽은 상태라면 무시하고, 안 죽었으면 Reset을 시킨다. 
		void ResetIfNotDead();

		//std::transform처럼, 각 함수에게 해당 함수를 호출시킨다.
		//IEnemyBehaviour를 대상으로 호출되는 것.
		void TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func);

		//개별 Enemy가 사망했을 떄, Handler에게 알려줘야 한다.
		//전체가 사망했을 때, BattleArea 같은 요소들에게 알려주기는 해야 한다.
		//단적인 호출이 되어야.
		void FromEnemyNotifyDead(const std::string& tagName, IEnemyBehaviour* behav);

	protected:
		//Derived Class들이 GrabManagedObject 부분에서 호출하는 부분.
		void GrabOrganizeAllEnemies(Pg::Data::GameObject* obj);

	public:
		//자신이 속한 HandlerBundle3D를 알아야 한다.
		HandlerBundle3D* _belongHandlerBundle3D{ nullptr };
	
	protected:
		unsigned int _entireAreaCount;

		//Area Index // <Tag Name / EnemyAgg>
		std::map<unsigned int,
			std::unordered_map<std::string, std::vector<EnemyAggregate>>> _managedMonstersList;
	};
}

