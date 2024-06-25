#pragma once

#include "ScriptInterface.h"
#include "IAreaHandler.h"
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

namespace Pg::DataScript
{
	class Stage1AreaHandler : public ScriptInterface<Stage1AreaHandler>, public IAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1AreaHandler);

	public:
		Stage1AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler
		virtual void ResetToInitialState() override;
		virtual void ResetAreaWithIndex(unsigned int index) override;
		virtual void SetCurrentAreaIndex(unsigned int index) override;
		virtual void OnPlayerHitDeathPlane() override;

	private:
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

		//자신이 속한 Stage의 Area 전체 개수.
		const unsigned int _stageAreaCount{ 3 };

		//현재 Player가 속한 Area Index를 보관.
		unsigned int _currentAreaIndex{ 0 };

	};
}


