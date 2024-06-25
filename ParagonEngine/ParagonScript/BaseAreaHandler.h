#pragma once
#include "MovingObjectDefinesAndStructs.h"
#include "IConfinedArea.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

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
	class HandlerBundle3D;

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

		//외부에서 호출됨. 특정 Area에 있는 IConfinedArea들의 Activate 설정.
		void SetActivateConfinedAreaIndex(unsigned int index, bool val);

		//...

	protected:
		//Derived Class들이 GrabManagedObject 부분에서 호출하는 부분.
		void GrabOrganizeAllAreas(Pg::Data::GameObject* obj);

	public:
		//자신이 속한 HandlerBundle3D를 알아야 한다.
		HandlerBundle3D* _belongHandlerBundle3D{ nullptr };

	protected:
		unsigned int _entireAreaCount;

		// MovingObject 관리를 위해.
		// ObjectName / MovingObject
		// Area별로 나누어서 기록하기.
		std::map<unsigned int,
			std::unordered_map<std::string,
			MovingObjectAggregate>>  _managedMovingObjectList;

		//일단, BattleArea는 따로 관리해야 한다.
		//IConfinedArea라는 인터페이스를 두고, 
		//해당 따로 관리. Event Trigger 때문에 그렇다.
		//벡터 인덱스 : Area Index.
		//내용물 : 인덱스에 종속된 IConfinedArea들.
		std::vector<std::unique_ptr<std::vector<IConfinedArea*>>> _confinedAreaList;

		//떨어지면 죽는 Death Plane들 관리. 
		std::vector<DeathPlane*> _deathPlaneList;

		//Area 영역 구분하는 Trigger들 따로 관리. Trigger / Area Index
		std::map<AreaPassingTrigger*, unsigned int> _areaTriggerMap;

		

		//현재 Player가 속한 Area Index를 보관.
		unsigned int _currentAreaIndex{ 0 };


	};
}


