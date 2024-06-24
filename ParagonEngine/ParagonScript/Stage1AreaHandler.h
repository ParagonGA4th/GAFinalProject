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
		virtual void ResetAreaWithIndex(unsigned int index) abstract;

	

	private:
		// MovingObject 관리를 위해.
		// ObjectName / MovingObject
		// Area별로 나누어서 기록하기.
		std::map<unsigned int, 
			std::unordered_map<std::string, 
			MovingObjectAggregate>>  _managedMovingObjectList;

		//클래스. 
		const unsigned int _stageAreaCount{ 3 };

	};
}


