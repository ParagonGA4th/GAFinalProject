#pragma once
#include "MovingObjectDefinesAndStructs.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>


namespace Pg::DataScript
{
	class DeathPlane;
	class AreaPassingTrigger;
}
//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
// Area Handler들의 인터페이스.

namespace Pg::DataScript
{
	class BaseAreaHandler
	{
	public:
		BaseAreaHandler(unsigned int entireAreaCount);

		//더 이상 Virtual이 아니다.
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		void ResetToInitialState();

		//구역별 리셋만이 필요할 때. 
		//virtual void ResetAreaUntil(int index) abstract;
		void ResetAreaWithIndex(unsigned int index);

		// AreaPassingTrigger에 의해 :
		// 현재 Player가 어디 있는지 전달될 수 있게.
		void SetCurrentAreaIndex(unsigned int index);
		
		//Player가 Death Plane에 도달했을 때.
		void OnPlayerHitDeathPlane();
		//...

	protected:
		//Derived Class들이 GrabManagedObject 부분에서 호출하는 부분.
		void GrabOrganizeAllAreas(Pg::Data::GameObject* obj);

	protected:
		unsigned int _entireAreaCount;

		// MovingObject 관리를 위해.
		// ObjectName / MovingObject
		// Area별로 나누어서 기록하기.
		std::map<unsigned int,
			std::unordered_map<std::string,
			MovingObjectAggregate>>  _managedMovingObjectList;

		//떨어지면 죽는 Death Plane들 관리. 
		std::vector<DeathPlane*> _deathPlaneList;

		//Area 영역 구분하는 Trigger들 따로 관리.
		std::map<AreaPassingTrigger*, unsigned int> _areaTriggerMap;

		//현재 Player가 속한 Area Index를 보관.
		unsigned int _currentAreaIndex{ 0 };


	};
}


