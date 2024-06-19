#pragma once

#include "ScriptInterface.h"
#include "IAreaHandler.h"
#include "MovingObjectDefinesAndStructs.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Pg::DataScript
{
	class Stage2AreaHandler : public ScriptInterface<Stage2AreaHandler>, public IAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2AreaHandler);

	public:
		Stage2AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler
		virtual void ResetToInitialState() override;

	private:
		//MovingObject 관리를 위해.
		//ObjectName / MovingObject
		std::unordered_map<std::string, MovingObjectAggregate> _managedMovingObjectList;
	};
}


